/**
 * DOSBox MP3 Seek Table handler, Copyright 2018 Kevin R. Croft (krcroft@gmail.com)
 * The seek table handler makes use of the following single-header public libraries:
 *   - dr_mp3: http://mackron.github.io/dr_mp3.html, by David Reid
 *   - archive: https://github.com/voidah/archive, by Arthur Ouellet
 *   - xxHash: http://cyan4973.github.io/xxHash, by Yann Collet
 *
 *  This seek table handler is free software: you can redistribute
 *  it and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DOSBox.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <vector>    // provides: vector
#include <SDL.h>     // provides: SDL_RWops
#include "archive.h" // provides: archive

// Ensure we only get the API
#ifdef DR_MP3_IMPLEMENTATION
#  undef DR_MP3_IMPLEMENTATION
#endif
#include "dr_mp3.h" // provides: drmp3

// Note: this C++ struct must match (in binary-form) the "drmp3_seek_point" struct
//       defined in dr_mp3.h.  If that changes, then update this to match, along
//       with adjusting the Serialize() template function that union's the values.
//
struct drmp3_seek_point_serial {
    drmp3_uint64 seekPosInBytes;      // Points to the first byte of an MP3 frame.
    drmp3_uint64 pcmFrameIndex;       // The index of the PCM frame this seek point targets.
    drmp3_uint16 mp3FramesToDiscard;  // The number of whole MP3 frames to be discarded before pcmFramesToDiscard.
    drmp3_uint16 pcmFramesToDiscard;
    template <class T> void Serialize(T& archive) {
            archive & seekPosInBytes & pcmFrameIndex & mp3FramesToDiscard & pcmFramesToDiscard;
    }
};

// Our private-decoder structure where we hold:
//   - a pointer to the working dr_mp3 instance
//   - a template vector of seek_points (the serializeable form)
struct mp3_t {
    drmp3* p_dr;    // the actual drmp3 instance we open, read, and seek within
    std::vector<drmp3_seek_point_serial> seek_points_vector;
};

const Uint64 populate_seek_points(struct SDL_RWops* const context, mp3_t* p_mp3, const char* seektable_filename);
