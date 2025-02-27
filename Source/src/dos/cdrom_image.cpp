/*
 *  Copyright (C) 2002-2018  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// #define DEBUG 1
// #define BENCHMARK 1
#ifdef BENCHMARK
#include <chrono>
#endif

#include <cctype>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <limits.h> //GCC 2.95
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include "cdrom.h"
#include "drives.h"
#include "support.h"
#include "setup.h"

#if !defined(WIN32)
#include <libgen.h>
#else
#include <string.h>
#endif

using namespace std;

#define MAX_LINE_LENGTH 512
#define MAX_FILENAME_LENGTH 256

CDROM_Interface_Image::BinaryFile::BinaryFile(const char *filename, bool &error)
	:TrackFile(RAW_SECTOR_SIZE)
{
	file = new ifstream(filename, ios::in | ios::binary);
	error = (file == NULL) || (file->fail());
}

CDROM_Interface_Image::BinaryFile::~BinaryFile()
{
	delete file;
}

bool CDROM_Interface_Image::BinaryFile::read(Bit8u *buffer, int seek, int count)
{
	file->seekg(seek, ios::beg);
	file->read((char*)buffer, count);
	return !(file->fail());
}

int CDROM_Interface_Image::BinaryFile::getLength()
{
	file->seekg(0, ios::end);
	int length = (int)file->tellg();
	if (file->fail()) return -1;
	return length;
}

bool CDROM_Interface_Image::BinaryFile::seek(Bit32u offset)
{
	file->seekg(offset, ios::beg);
	return !file->fail();
}

Bit16u CDROM_Interface_Image::BinaryFile::decode(Bit8u *buffer)
{
	file->read((char*)buffer, chunkSize);
	return file->gcount();
}

CDROM_Interface_Image::AudioFile::AudioFile(const char *filename, bool &error)
	:TrackFile(4096)
{
	// Use the audio file's actual sample rate and number of channels as opposed to overriding
	Sound_AudioInfo desired = {AUDIO_S16, 0, 0};
	sample = Sound_NewSampleFromFile(filename, &desired, chunkSize);
	if (sample) {
		error = false;
		std::string filename_only(filename);
		filename_only = filename_only.substr(filename_only.find_last_of("\\/") + 1);
		LOG_MSG("CDROM: Loaded %s [%d Hz %d-channel]", filename_only.c_str(), this->getRate(), this->getChannels());
	} else
		error = true;
}

CDROM_Interface_Image::AudioFile::~AudioFile()
{
	Sound_FreeSample(sample);
}

bool CDROM_Interface_Image::AudioFile::seek(Bit32u offset)
{
	#ifdef BENCHMARK
	const auto begin = std::chrono::steady_clock::now();
	#endif

	// Convert the byte-offset to a time offset (milliseconds)
	const bool result = Sound_Seek(sample, lround(offset/176.4f));

	#ifdef BENCHMARK
	const auto end = std::chrono::steady_clock::now();
	LOG_MSG("CDROM: seek(%u) took %f ms", offset, chrono::duration <double, milli> (end - begin).count());
	#endif

	return result;
}

Bit16u CDROM_Interface_Image::AudioFile::decode(Bit8u *buffer)
{
	const Bit16u bytes = Sound_Decode(sample);
	memcpy(buffer, sample->buffer, bytes);
	return bytes;
}

Bit32u CDROM_Interface_Image::AudioFile::getRate()
{
	Bit32u rate(0);
	if (sample) {
		rate = sample->actual.rate;
	}
	return rate;
}

Bit8u CDROM_Interface_Image::AudioFile::getChannels()
{
	Bit8u channels(0);
	if (sample) {
		channels = sample->actual.channels;
	}
	return channels;
}

int CDROM_Interface_Image::AudioFile::getLength()
{
	int length(-1);

	// GetDuration returns milliseconds ... but getLength needs Red Book bytes.
	const int duration_ms = Sound_GetDuration(sample);
	if (duration_ms > 0) {
		// ... so convert ms to "Red Book bytes" by multiplying with 176.4f,
		// which is 44,100 samples/second * 2-channels * 2 bytes/sample
		// / 1000 milliseconds/second
		length = round(duration_ms * 176.4f);
	}
	return length;
}

// initialize static members
int CDROM_Interface_Image::refCount = 0;
CDROM_Interface_Image* CDROM_Interface_Image::images[26];
CDROM_Interface_Image::imagePlayer CDROM_Interface_Image::player = {
	NULL,  // CDROM_Interface_Image*
	NULL,  // MixerChannel*
	{0},   // buffer[]
	0,     // startFrame
	0,     // currFrame
	0,     // numFrames
	false, // isPlaying
	false, // isPaused
	false, // ctrlUsed
	{0},   // ctrlData struct
	NULL,  // activeTrack
	NULL,  // addSamples
	0,     // playbackTotal
	0,     // playbackRemaining
	0,     // bufferPos
	0      // bufferConsumed
};
	
CDROM_Interface_Image::CDROM_Interface_Image(Bit8u subUnit)
{
	images[subUnit] = this;
	if (refCount == 0) {
		if (player.channel == NULL) {
			// channel is kept dormant except during cdrom playback periods
			player.channel = MIXER_AddChannel(&CDAudioCallBack, 0, "CDAUDIO");
			player.channel->Enable(false);
			// LOG_MSG("CDROM: Initialized with %d-byte circular buffer", AUDIO_DECODE_BUFFER_SIZE);
		}
	}
	refCount++;
}

CDROM_Interface_Image::~CDROM_Interface_Image()
{
	refCount--;
	if (player.cd == this) player.cd = NULL;
	ClearTracks();
	if (refCount == 0) {
		StopAudio();
		MIXER_DelChannel(player.channel);
		player.channel = NULL;
		// LOG_MSG("CDROM: Audio channel freed");
	}
}

void CDROM_Interface_Image::InitNewMedia()
{
}

bool CDROM_Interface_Image::SetDevice(char* path, int forceCD)
{
	if (LoadCueSheet(path)) return true;
	if (LoadIsoFile(path)) return true;
	
	// print error message on dosbox console
	char buf[MAX_LINE_LENGTH];
	snprintf(buf, MAX_LINE_LENGTH, "Could not load image file: %s\n", path);
	Bit16u size = (Bit16u)strlen(buf);
	DOS_WriteFile(STDOUT, (Bit8u*)buf, &size);
	return false;
}

bool CDROM_Interface_Image::GetUPC(unsigned char& attr, char* upc)
{
	attr = 0;
	strcpy(upc, this->mcn.c_str());
	LOG_MSG("CDROM: GetUPC=%s", upc);
	return true;
}

bool CDROM_Interface_Image::GetAudioTracks(int& stTrack, int& end, TMSF& leadOut)
{
	stTrack = 1;
	end = (int)(tracks.size() - 1);
	FRAMES_TO_MSF(tracks[tracks.size() - 1].start + 150, &leadOut.min, &leadOut.sec, &leadOut.fr);

	#ifdef DEBUG
	LOG_MSG("CDROM: GetAudioTracks, stTrack=%d, end=%d, leadOut.min=%d, leadOut.sec=%d, leadOut.fr=%d",
	  stTrack,
	  end,
	  leadOut.min,
	  leadOut.sec,
	  leadOut.fr);
	#endif

	return true;
}

bool CDROM_Interface_Image::GetAudioTrackInfo(int track, TMSF& start, unsigned char& attr)
{
	if (track < 1 || track > (int)tracks.size()) return false;
	FRAMES_TO_MSF(tracks[track - 1].start + 150, &start.min, &start.sec, &start.fr);
	attr = tracks[track - 1].attr;

	#ifdef DEBUG
	LOG_MSG("CDROM: GetAudioTrackInfo track=%d MSF %02d:%02d:%02d, attr=%u",
	  track,
	  start.min,
	  start.sec,
	  start.fr,
	  attr
	);
	#endif

	return true;
}

bool CDROM_Interface_Image::GetAudioSub(unsigned char& attr, unsigned char& track, unsigned char& index, TMSF& relPos, TMSF& absPos)
{
	int cur_track = GetTrack(player.currFrame);
	if (cur_track < 1) return false;
	track = (unsigned char)cur_track;
	attr = tracks[track - 1].attr;
	index = 1;
	FRAMES_TO_MSF(player.currFrame + 150, &absPos.min, &absPos.sec, &absPos.fr);
	FRAMES_TO_MSF(player.currFrame - tracks[track - 1].start + 150, &relPos.min, &relPos.sec, &relPos.fr);

	#ifdef DEBUG
	LOG_MSG("CDROM: GetAudioSub attr=%u, track=%u, index=%u", attr, track, index);

	LOG_MSG("CDROM: GetAudioSub absoute  offset (%d), MSF=%d:%d:%d",
	  player.currFrame + 150,
	  absPos.min,
	  absPos.sec,
	  absPos.fr);
	LOG_MSG("CDROM: GetAudioSub relative offset (%d), MSF=%d:%d:%d",
	  player.currFrame - tracks[track - 1].start + 150,
	  relPos.min,
	  relPos.sec,
	  relPos.fr);
	#endif

	return true;
}

bool CDROM_Interface_Image::GetAudioStatus(bool& playing, bool& pause)
{
	playing = player.isPlaying;
	pause = player.isPaused;

	#ifdef DEBUG
	LOG_MSG("CDROM: GetAudioStatus playing=%d, paused=%d", playing, pause);
	#endif

	return true;
}

bool CDROM_Interface_Image::GetMediaTrayStatus(bool& mediaPresent, bool& mediaChanged, bool& trayOpen)
{
	mediaPresent = true;
	mediaChanged = false;
	trayOpen = false;

	#ifdef DEBUG
	LOG_MSG("CDROM: GetMediaTrayStatus present=%d, changed=%d, open=%d", mediaPresent, mediaChanged, trayOpen);
	#endif

	return true;
}

bool CDROM_Interface_Image::PlayAudioSector(unsigned long start, unsigned long len)
{
	bool is_playable(false);
	const int track = GetTrack(start) - 1;

	// The CDROM Red Book standard allows up to 99 tracks, which includes the data track
	if ( track < 0 || track > 98 )
		LOG(LOG_MISC, LOG_WARN)("Game tried to load track #%d, which is invalid", track);

	// Attempting to play zero sectors is a no-op
	else if (len == 0)
		LOG(LOG_MISC, LOG_WARN)("Game tried to play zero sectors, skipping");

	// The maximum storage achieved on a CDROM was ~900MB or just under 100 minutes
	// with overburning, so use this threshold to sanity-check the start sector.
	else if (start > 450000)
		LOG(LOG_MISC, LOG_WARN)("Game tried to read sector %lu, which is beyond the 100-minute maximum of a CDROM", start);

	// We can't play audio from a data track (as it would result in garbage/static)
	else if(track >= 0 && tracks[track].attr == 0x40)
		LOG(LOG_MISC,LOG_WARN)("Game tries to play the data track. Not doing this");

	// Checks passed, setup the audio stream
	else {
		TrackFile* trackFile = tracks[track].file;

		// Convert the playback start sector to a time offset (milliseconds) relative to the track
		const Bit32u offset = tracks[track].skip + (start - tracks[track].start) * tracks[track].sectorSize;
		is_playable = trackFile->seek(offset);

		// only initialize the player elements if our track is playable
		if (is_playable) {
			const Bit8u channels = trackFile->getChannels();
			const Bit32u rate = trackFile->getRate();

			player.cd = this;
			player.trackFile = trackFile;
			player.startFrame = start;
			player.currFrame = start;
			player.numFrames = len;
			player.bufferPos = 0;
			player.bufferConsumed = 0;
			player.isPlaying = true;
			player.isPaused = false;

			// Assign either the stereo or mono version of the Mixer's AddChannel function
			if (channels == 2) {
				#if defined(WORDS_BIGENDIAN)
				player.addSamples = &MixerChannel::AddSamples_s16_nonnative;
				#else
				player.addSamples = &MixerChannel::AddSamples_s16;
				#endif
			} else {
				#if defined(WORDS_BIGENDIAN)
				player.addSamples = &MixerChannel::AddSamples_m16_nonnative;
				#else
				player.addSamples = &MixerChannel::AddSamples_m16;
				#endif
			}

			const float bytesPerMs = rate * channels * 2 / 1000.0;
			player.playbackTotal = lround(len * tracks[track].sectorSize * bytesPerMs / 176.4);
			player.playbackRemaining = player.playbackTotal;

			#ifdef DEBUG
			LOG_MSG(
			   "CDROM: Playing track %d at %.1f KHz %d-channel at start sector %lu (%.1f minute-mark), seek %u (skip=%d,dstart=%d,secsize=%d), for %lu sectors (%.1f seconds)",
			   track,
			   rate/1000.0,
			   channels,
			   start,
			   offset * (1/10584000.0),
			   offset,
			   tracks[track].skip,
			   tracks[track].start,
			   tracks[track].sectorSize,
			   len,
			   player.playbackRemaining / (1000 * bytesPerMs)
			);
			#endif

			// start the channel!
			player.channel->SetFreq(rate);
			player.channel->Enable(true);
		}
	}
	if (!is_playable) StopAudio();
	return is_playable;
}

bool CDROM_Interface_Image::PauseAudio(bool resume)
{
	// Only switch states if needed
	if (player.isPaused == resume) {
		player.channel->Enable(resume);
		player.isPaused = !resume;
	}

	#ifdef DEBUG
	LOG_MSG("CDROM: PauseAudio, state=%s", resume ? "resumed" : "paused");
	#endif

	return true;
}

bool CDROM_Interface_Image::StopAudio(void)
{
	// Only switch states if needed
	if (player.isPlaying) {
		player.channel->Enable(false);
		player.isPlaying = false;
		player.isPaused = false;
	}

	#ifdef DEBUG
	LOG_MSG("CDROM: StopAudio");
	#endif

	return true;
}

void CDROM_Interface_Image::ChannelControl(TCtrl ctrl)
{
	player.ctrlUsed = (ctrl.out[0]!=0 || ctrl.out[1]!=1 || ctrl.vol[0]<0xfe || ctrl.vol[1]<0xfe);
	player.ctrlData = ctrl;
}

bool CDROM_Interface_Image::ReadSectors(PhysPt buffer, bool raw, unsigned long sector, unsigned long num)
{
	int sectorSize = raw ? RAW_SECTOR_SIZE : COOKED_SECTOR_SIZE;
	Bitu buflen = num * sectorSize;
	Bit8u* buf = new Bit8u[buflen];
	
	bool success = true; //Gobliiins reads 0 sectors
	for(unsigned long i = 0; i < num; i++) {
		success = ReadSector(&buf[i * sectorSize], raw, sector + i);
		if (!success) break;
	}

	MEM_BlockWrite(buffer, buf, buflen);
	delete[] buf;
	return success;
}

bool CDROM_Interface_Image::LoadUnloadMedia(bool unload)
{
	return true;
}

int CDROM_Interface_Image::GetTrack(int sector)
{
	vector<Track>::iterator i = tracks.begin();
	vector<Track>::iterator end = tracks.end() - 1;
	
	while(i != end) {
		Track &curr = *i;
		Track &next = *(i + 1);
		if (curr.start <= sector && sector < next.start) return curr.number;
		i++;
	}
	return -1;
}

bool CDROM_Interface_Image::ReadSector(Bit8u *buffer, bool raw, unsigned long sector)
{
	int track = GetTrack(sector) - 1;
	if (track < 0) return false;
	
	int seek = tracks[track].skip + (sector - tracks[track].start) * tracks[track].sectorSize;
	int length = (raw ? RAW_SECTOR_SIZE : COOKED_SECTOR_SIZE);
	if (tracks[track].sectorSize != RAW_SECTOR_SIZE && raw) return false;
	if (tracks[track].sectorSize == RAW_SECTOR_SIZE && !tracks[track].mode2 && !raw) seek += 16;
	if (tracks[track].mode2 && !raw) seek += 24;

	// LOG_MSG("CDROM: ReadSector track=%d, desired raw=%s, sector=%ld, length=%d", track, raw ? "true":"false", sector, length);
	return tracks[track].file->read(buffer, seek, length);
}

void printProgress(double percentage, const char* msg)
{
	// 60 is the number of characters in the full progress bar
	int val  = (int)(percentage * 100);
	int lpad = (int)(percentage * 60);
	int rpad = 60 - lpad;
	LOG_MSG("\r%3d%% [%.*s%*s] - %s", val, lpad,
	   "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||", rpad, "", msg);
	fflush(stdout);
}

void CDROM_Interface_Image::CDAudioCallBack(Bitu len)
{
	// Our member object "playbackRemaining" holds the
	// exact number of stream-bytes we need to play before meeting the
	// DOS program's desired playback duration in sectors. We simply
	// decrement this counter each callback until we're done.
	if (len == 0 || !player.isPlaying || player.isPaused) return;


	// determine bytes per request (16-bit samples)
	const Bit8u channels = player.trackFile->getChannels();
	const Bit8u bytes_per_request = channels * 2;
	Bit16u total_requested = len * bytes_per_request;

	while (total_requested > 0) {
		Bit16u requested = total_requested;

		// Every now and then the callback wants a big number of bytes,
		// which can exceed our circular buffer. In these cases we need
		// read through as many iteration of our circular buffer as needed.
		if (total_requested > AUDIO_DECODE_BUFFER_SIZE) {
			requested = AUDIO_DECODE_BUFFER_SIZE;
			total_requested -= AUDIO_DECODE_BUFFER_SIZE;
		}
		else {
			total_requested = 0;
		}

		// Three scenarios in order of probabilty:
		//
		// 1. Consume: If our decoded circular buffer is sufficiently filled to
		//             satify the requested size, then feed the callback with
		//             the requested number of bytes.
		//
		// 2. Wrap:    If we've decoded and consumed to edge of our buffer, then
		//             we need to wrap any remaining decoded-but-not-consumed
		//             samples back around to the front of the buffer.
		//
		// 3. Fill:    When out circular buffer is too depleted to satisfy the
		//             requested size, then perform chunked-decode reads from
		//             the audio-codec to either fill our buffer or satify our
		//             remaining playback - whichever is smaller.
		//
		bool have_consumed = false;
		while (!have_consumed) {

			// 1. Consume
			// ==========
			if (player.bufferPos - player.bufferConsumed >= requested) {
				if (player.ctrlUsed) {
					for (Bit8u i=0; i < channels; i++) {
						Bit16s  sample;
						Bit16s* samples = (Bit16s*)&player.buffer[player.bufferConsumed];
						for (Bitu pos = 0; pos < requested / bytes_per_request; pos++) {
							#if defined(WORDS_BIGENDIAN)
							sample = (Bit16s)host_readw((HostPt) & samples[pos * 2 + player.ctrlData.out[i]]);
							#else
							sample = samples[pos * 2 + player.ctrlData.out[i]];
							#endif
							samples[pos * 2 + i] = (Bit16s)(sample * player.ctrlData.vol[i] / 255.0);
						}
					}
				}
				// uses either the stereo or mono and native or nonnative AddSamples call assigned during construction
				(player.channel->*player.addSamples)(requested / bytes_per_request, (Bit16s*)(player.buffer + player.bufferConsumed) );
				player.bufferConsumed += requested;
				player.playbackRemaining -= requested;

				// Games can query the current Red Book MSF frame-position, so we keep that up-to-date here.
				// We scale the final number of frames by the percent complete, which
				// avoids having to keep track of the euivlent number of Red Book frames
				// read (which would involve coverting the compressed streams data-rate into
				// CDROM Red Book rate, which is more work than simply scaling).
				//
				const float playbackPercentSoFar = static_cast<float>(player.playbackTotal - player.playbackRemaining) / player.playbackTotal;
				player.currFrame = player.startFrame + ceil(player.numFrames * playbackPercentSoFar);

				have_consumed = true;
				// printProgress( (player.bufferPos - player.bufferConsumed)/(float)AUDIO_DECODE_BUFFER_SIZE, "consume");
				break;

			// 2. Wrap
			// =======
			} else {
				memcpy(player.buffer,
					   player.buffer + player.bufferConsumed,
					   player.bufferPos - player.bufferConsumed);
				player.bufferPos -= player.bufferConsumed;
				player.bufferConsumed = 0;
				// printProgress( (player.bufferPos - player.bufferConsumed)/(float)AUDIO_DECODE_BUFFER_SIZE, "wrap");
			}

			// 3. Fill
			// =======
			const Bit16u chunkSize = player.trackFile->chunkSize;
			while(AUDIO_DECODE_BUFFER_SIZE - player.bufferPos >= chunkSize &&
			      (player.bufferPos - player.bufferConsumed < player.playbackRemaining ||
				   player.bufferPos - player.bufferConsumed < requested) ) {

				const Bit16u decoded = player.trackFile->decode(player.buffer + player.bufferPos);
				player.bufferPos += decoded;

				// if we decoded less than expected, which could be due to EOF or if the CUE file specified
				// an exact "INDEX 01 MIN:SEC:FRAMES" value but the compressed track is ever-so-slightly less than
				// that specified, then simply pad with zeros.
				const Bit16s underDecode = chunkSize - decoded;
				if (underDecode > 0) {
					LOG_MSG("CDROM: Underdecoded by %d. Feeding mixer with zeros.", underDecode);
					memset(player.buffer + player.bufferPos, 0, underDecode);
					player.bufferPos += underDecode;
				}
				// printProgress( (player.bufferPos - player.bufferConsumed)/(float)AUDIO_DECODE_BUFFER_SIZE, "fill");
			}
		} // end while ! have_consumed
	} // end while total_requested

	if (player.playbackRemaining <= 0) {
		player.cd->StopAudio();
		// printProgress( (player.bufferPos - player.bufferConsumed)/(float)AUDIO_DECODE_BUFFER_SIZE, "stop");
	}
}

bool CDROM_Interface_Image::LoadIsoFile(char* filename)
{
	tracks.clear();
	
	// data track
	Track track = {0, 0, 0, 0, 0, 0, false, NULL};
	bool error;
	track.file = new BinaryFile(filename, error);
	if (error) {
		delete track.file;
		return false;
	}
	track.number = 1;
	track.attr = 0x40;//data
	
	// try to detect iso type
	if (CanReadPVD(track.file, COOKED_SECTOR_SIZE, false)) {
		track.sectorSize = COOKED_SECTOR_SIZE;
		track.mode2 = false;
	} else if (CanReadPVD(track.file, RAW_SECTOR_SIZE, false)) {
		track.sectorSize = RAW_SECTOR_SIZE;
		track.mode2 = false;		
	} else if (CanReadPVD(track.file, 2336, true)) {
		track.sectorSize = 2336;
		track.mode2 = true;		
	} else if (CanReadPVD(track.file, RAW_SECTOR_SIZE, true)) {
		track.sectorSize = RAW_SECTOR_SIZE;
		track.mode2 = true;		
	} else return false;
	
	track.length = track.file->getLength() / track.sectorSize;
	// LOG_MSG("LoadIsoFile: %s, track 1, 0x40, sectorSize=%d, mode2=%s", filename, track.sectorSize, track.mode2 ? "true":"false");

	tracks.push_back(track);
	
	// leadout track
	track.number = 2;
	track.attr = 0;
	track.start = track.length;
	track.length = 0;
	track.file = NULL;
	tracks.push_back(track);
	return true;
}

bool CDROM_Interface_Image::CanReadPVD(TrackFile *file, int sectorSize, bool mode2)
{
	Bit8u pvd[COOKED_SECTOR_SIZE];
	int seek = 16 * sectorSize;	// first vd is located at sector 16
	if (sectorSize == RAW_SECTOR_SIZE && !mode2) seek += 16;
	if (mode2) seek += 24;
	file->read(pvd, seek, COOKED_SECTOR_SIZE);
	// pvd[0] = descriptor type, pvd[1..5] = standard identifier, pvd[6] = iso version (+8 for High Sierra)
	return ((pvd[0] == 1 && !strncmp((char*)(&pvd[1]), "CD001", 5) && pvd[6] == 1) ||
			(pvd[8] == 1 && !strncmp((char*)(&pvd[9]), "CDROM", 5) && pvd[14] == 1));
}

#if defined(WIN32)
static string dirname(char * file) {
	char * sep = strrchr(file, '\\');
	if (sep == NULL)
		sep = strrchr(file, '/');
	if (sep == NULL)
		return "";
	else {
		int len = (int)(sep - file);
		char tmp[MAX_FILENAME_LENGTH];
		safe_strncpy(tmp, file, len+1);
		return tmp;
	}
}
#endif

bool CDROM_Interface_Image::LoadCueSheet(char *cuefile)
{
	Track track = {0, 0, 0, 0, 0, 0, false, NULL};
	tracks.clear();
	int shift = 0;
	int currPregap = 0;
	int totalPregap = 0;
	int prestart = 0;
	bool success;
	bool parsedIndex = false;
	bool parsedTrack = false;
	int frameFromCue = 0;
	char tmp[MAX_FILENAME_LENGTH];	// dirname can change its argument
	safe_strncpy(tmp, cuefile, MAX_FILENAME_LENGTH);
	string pathname(dirname(tmp));
	ifstream in;
	in.open(cuefile, ios::in);
	if (in.fail()) return false;
	
	while(!in.eof()) {
		// get next line
		char buf[MAX_LINE_LENGTH];
		in.getline(buf, MAX_LINE_LENGTH);
		if (in.fail() && !in.eof()) return false;  // probably a binary file
		istringstream line(buf);
		
		string command;
		GetCueKeyword(command, line);
		
		if (command == "TRACK") {
			if (parsedTrack && parsedIndex) {
				success = AddTrack(track, shift, prestart, totalPregap, currPregap, frameFromCue);
				parsedTrack = false;
				parsedIndex = false;
				frameFromCue = 0;
			}

			else success = true;
			
			parsedTrack = false;
			parsedIndex = false;
			frameFromCue = 0;
			track.start = 0;
			track.skip = 0;
			currPregap = 0;
			prestart = 0;
	
			line >> track.number;
			string type;
			GetCueKeyword(type, line);
			
			if (type == "AUDIO") {
				track.sectorSize = RAW_SECTOR_SIZE;
				track.attr = 0;
				track.mode2 = false;
			} else if (type == "MODE1/2048") {
				track.sectorSize = COOKED_SECTOR_SIZE;
				track.attr = 0x40;
				track.mode2 = false;
			} else if (type == "MODE1/2352") {
				track.sectorSize = RAW_SECTOR_SIZE;
				track.attr = 0x40;
				track.mode2 = false;
			} else if (type == "MODE2/2336") {
				track.sectorSize = 2336;
				track.attr = 0x40;
				track.mode2 = true;
			} else if (type == "MODE2/2352") {
				track.sectorSize = RAW_SECTOR_SIZE;
				track.attr = 0x40;
				track.mode2 = true;
			} else success = false;
			
			parsedTrack = true;
		}
		else if (command == "INDEX") {
			int index;
			line >> index;
			success = GetCueFrame(frameFromCue, line);
			
			if (index == 0) prestart = frameFromCue;
			else if (index == 1) {
				track.start = frameFromCue;
				parsedIndex = true;
			}
		}
		else if (command == "FILE") {
			if (parsedTrack && parsedIndex) {
				success = AddTrack(track, shift, prestart, totalPregap, currPregap, frameFromCue);
				parsedTrack = false;
				parsedIndex = false;
				frameFromCue = 0;
			}
			else success = true;

			string filename;
			GetCueString(filename, line);
			GetRealFileName(filename, pathname);
			string type;
			GetCueKeyword(type, line);

			track.file = NULL;
			bool error = true;
			if (type == "BINARY") {
				track.file = new BinaryFile(filename.c_str(), error);
			}
			else
				track.file = new AudioFile(filename.c_str(), error);
				// SDL_Sound first tries using a decoder having a matching registered extension
				// as the filename, and then falls back to trying each decoder before finally
				// giving up.
			if (error) {
				delete track.file;
				success = false;
			}
		}
		else if (command == "PREGAP") success = GetCueFrame(currPregap, line);
		else if (command == "CATALOG") success = GetCueString(mcn, line);
		// ignored commands
		else if (command == "CDTEXTFILE" || command == "FLAGS" || command == "ISRC"
			|| command == "PERFORMER" || command == "POSTGAP" || command == "REM"
			|| command == "SONGWRITER" || command == "TITLE" || command == "") success = true;
		// failure
		else success = false;

		if (!success) return false;
	}
	// add last track
	if (!AddTrack(track, shift, prestart, totalPregap, currPregap, frameFromCue)) return false;
	
	// add leadout track
	track.number++;
	track.attr = 0;//sync with load iso
	track.start = 0;
	track.length = 0;
	track.file = NULL;
	if(!AddTrack(track, shift, 0, totalPregap, 0, 0)) return false;

	return true;
}

bool CDROM_Interface_Image::AddTrack(Track &curr, int &shift, int prestart, int &totalPregap, int currPregap, int frameFromCue)
{
	#ifdef DEBUG
	LOG_MSG("AddTrack c.start=%d c.len=%d c.start+len=%d, shift=%d,"
	        " prestart=%d, totalPregap=%d, currPregap=%d, frameFromCue=%d",
	        curr.start, curr.length, curr.start + curr.length,
	        shift, prestart, totalPregap, currPregap, frameFromCue);
	#endif
	
	// frames between index 0(prestart) and 1(curr.start) must be skipped
	int skip;
	if (prestart > 0) {
		if (prestart > curr.start) return false;
		skip = curr.start - prestart;
	} else skip = 0;
	
	// first track (track number must be 1)
	if (tracks.empty()) {
		if (curr.number != 1) return false;
		curr.skip = skip * curr.sectorSize;
		curr.start += currPregap;
		totalPregap = currPregap;
		tracks.push_back(curr);
		return true;
	}
	
	Track &prev = *(tracks.end() - 1);

	// MSF is defined so leave current start as-is and only calculate the previous length
	if (frameFromCue)
		prev.length = curr.start + totalPregap - prev.start - skip;

	// current track consumes data from the same file as the previous
	if (prev.file == curr.file) {
		// MSF is 00:00:00, so we need to calculate curr.start
		if (!frameFromCue) {
			curr.start += shift;
			// only modify previous length if was not set during a previous MSF pass
			if (!prev.length) prev.length = curr.start + totalPregap - prev.start - skip;
		}
		// common
		curr.skip += prev.skip + prev.length * prev.sectorSize + skip * curr.sectorSize;
		totalPregap += currPregap;

	// current track uses a different file as the previous track
	} else {
		// MSF is 00:00:00, so we need to calculate curr.start
		 if (!frameFromCue) {
			// only modify previous length if was not set during a previous MSF pass
			if (!prev.length) {
				int tmp = prev.file->getLength() - prev.skip;
				prev.length = tmp / prev.sectorSize;
				if (tmp % prev.sectorSize != 0) prev.length++; // padding
			}
			curr.start += prev.start + prev.length;
		}
		// common
		curr.skip = skip * curr.sectorSize;
		shift += prev.start + prev.length;
		totalPregap = currPregap;
	}
	
	// In all cases, bump our current start by the total pregap
	curr.start += totalPregap;

	#ifdef DEBUG
	LOG_MSG("AddTrack c.start=%d c.len=%d c.start+len=%d | p.start=%d p.len=%d p.start+len=%d",
	        curr.start, curr.length, curr.start + curr.length,
	        prev.start, prev.length, prev.start + prev.length);
	#endif

	// error checks
	if (curr.number <= 1) return false;
	if (prev.number + 1 != curr.number) return false;
	if (curr.start < prev.start + prev.length) return false;
	if (curr.length < 0) return false;
	
	tracks.push_back(curr);
	return true;
}

bool CDROM_Interface_Image::HasDataTrack(void)
{
	//Data track has attribute 0x40
	for(track_it it = tracks.begin(); it != tracks.end(); it++) {
		if ((*it).attr == 0x40) return true;
	}
	return false;
}


bool CDROM_Interface_Image::GetRealFileName(string &filename, string &pathname)
{
	// check if file exists
	struct stat test;
	if (stat(filename.c_str(), &test) == 0) return true;
	
	// check if file with path relative to cue file exists
	string tmpstr(pathname + "/" + filename);
	if (stat(tmpstr.c_str(), &test) == 0) {
		filename = tmpstr;
		return true;
	}
	// finally check if file is in a dosbox local drive
	char fullname[CROSS_LEN];
	char tmp[CROSS_LEN];
	safe_strncpy(tmp, filename.c_str(), CROSS_LEN);
	Bit8u drive;
	if (!DOS_MakeName(tmp, fullname, &drive)) return false;
	
	localDrive *ldp = dynamic_cast<localDrive*>(Drives[drive]);
	if (ldp) {
		ldp->GetSystemFilename(tmp, fullname);
		if (stat(tmp, &test) == 0) {
			filename = tmp;
			return true;
		}
	}
#if defined (WIN32) || defined(OS2)
	//Nothing
#else
	//Consider the possibility that the filename has a windows directory seperator (inside the CUE file) 
	//which is common for some commercial rereleases of DOS games using DOSBox

	string copy = filename;
	size_t l = copy.size();
	for (size_t i = 0; i < l;i++) {
		if(copy[i] == '\\') copy[i] = '/';
	}

	if (stat(copy.c_str(), &test) == 0) {
		filename = copy;
		return true;
	}

	tmpstr = pathname + "/" + copy;
	if (stat(tmpstr.c_str(), &test) == 0) {
		filename = tmpstr;
		return true;
	}

#endif
	return false;
}

bool CDROM_Interface_Image::GetCueKeyword(string &keyword, istream &in)
{
	in >> keyword;
	for(Bitu i = 0; i < keyword.size(); i++) keyword[i] = toupper(keyword[i]);
	
	return true;
}

bool CDROM_Interface_Image::GetCueFrame(int &frames, istream &in)
{
	string msf;
	in >> msf;
	int min, sec, fr;
	bool success = sscanf(msf.c_str(), "%d:%d:%d", &min, &sec, &fr) == 3;
	frames = MSF_TO_FRAMES(min, sec, fr);
	
	return success;
}

bool CDROM_Interface_Image::GetCueString(string &str, istream &in)
{
	int pos = (int)in.tellg();
	in >> str;
	if (str[0] == '\"') {
		if (str[str.size() - 1] == '\"') {
			str.assign(str, 1, str.size() - 2);
		} else {
			in.seekg(pos, ios::beg);
			char buffer[MAX_FILENAME_LENGTH];
			in.getline(buffer, MAX_FILENAME_LENGTH, '\"');	// skip
			in.getline(buffer, MAX_FILENAME_LENGTH, '\"');
			str = buffer;
		}
	}
	return true;
}

void CDROM_Interface_Image::ClearTracks()
{
	vector<Track>::iterator i = tracks.begin();
	vector<Track>::iterator end = tracks.end();

	TrackFile* last = NULL;	
	while(i != end) {
		Track &curr = *i;
		if (curr.file != last) {
			delete curr.file;
			last = curr.file;
		}
		i++;
	}
	tracks.clear();
}

void CDROM_Image_Destroy(Section*) {
	Sound_Quit();
}

void CDROM_Image_Init(Section* sec) {
	sec->AddDestroyFunction(CDROM_Image_Destroy, false);
	Sound_Init();
}
