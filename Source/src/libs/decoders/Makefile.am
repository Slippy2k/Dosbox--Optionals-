noinst_LIBRARIES = libdecoders.a

libdecoders_a_SOURCES =  \
	SDL_sound.c          \
	SDL_sound.h          \
	SDL_sound_internal.h \
	audio_convert.c      \
	wav.c                \
	flac.c               \
	dr_flac.h            \
	opus.c               \
	vorbis.cpp           \
	stb_vorbis.h         \
	mp3.cpp              \
	mp3_seek_table.cpp   \
	mp3_seek_table.h     \
	dr_mp3.h             \
	archive.h            \
	xxhash.c             \
	xxhash.h

libdecoders_a_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	$(CXXFLAGS) \
	-std=c++11 \
	-Wpedantic \
	-Wall

libdecoders_a_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	$(CPPFLAGS) \
	-Iinternal/ogg/include \
	-Iinternal/opus/include

libdecoders_a_CFLAGS = \
	$(AM_CFLAGS) \
	$(CFLAGS) \
	-Wpedantic \
	-Wall

opus.c: \
	internal/include/opus/opusfile.h \
	internal/include/speex/speex_resampler.h

internal/include/opus/opusfile.h:
	cd internal \
	&& $(MAKE) -j4 opusfile/Makefile

internal/include/speex/speex_resampler.h:
	cd internal \
	&& $(MAKE) -j4 speexdsp/Makefile

clean-local:
	cd internal \
	&& $(MAKE) dist-clean

