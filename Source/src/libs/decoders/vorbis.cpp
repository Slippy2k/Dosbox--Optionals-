/*
 * SDL1 Sound 1.0.x Vorbis decoder backend maintained by Kevin R. Croft (krcroft@gmail.com)
 * The upstream SDL2 Sound 1.9.x Vorbis decoder is written and copyright by Ryan C. Gordon. (icculus@icculus.org)
 * This decoder makes use of the excellent STB Vorbis decoder (v1.14) by Sean Barrett
 *
 * Source links
 *   - STB: https://github.com/nothings/stb (source)
 *   - STB: https://twitter.com/nothings (website/author info)
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file is part of the SDL Sound Library.
 *
 *  This SDL_sound Ogg Opus decoder backend is free software: you can redistribute
 *  it and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This  SDL_sound Ogg Opus decoder backend is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the SDL Sound Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef memcpy
#  undef memcpy
#endif

#include <string.h> // memcpy
#include <math.h>

#include "../../../include/logging.h" // provides: LOG_MSG

#include "SDL_sound.h"
#define __SDL_SOUND_INTERNAL__
#include "SDL_sound_internal.h"

#ifdef asset
#  undef assert
#  define assert SDL_assert
#endif

#ifdef memset
#  undef memset
#  define memset SDL_memset
#endif

#define memcmp SDL_memcmp
#define qsort SDL_qsort
#define malloc SDL_malloc
#define realloc SDL_realloc
#define free SDL_free
#define dealloca(x) SDL_stack_free((x))

/* Configure and include stb_vorbis for compiling... */
#define STB_VORBIS_NO_STDIO 1
#define STB_VORBIS_NO_CRT 1
#define STB_VORBIS_NO_PUSHDATA_API 1
#define STB_VORBIS_MAX_CHANNELS 2
#define STBV_CDECL
// #define STB_FORCEINLINE SDL_FORCE_INLINE
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define STB_VORBIS_BIG_ENDIAN 1
#endif

#include "stb_vorbis.h"

static const char *vorbis_error_string(const int err)
{
    switch (err)
    {
        case VORBIS__no_error: return NULL;
        case VORBIS_need_more_data: return "VORBIS: need more data";
        case VORBIS_invalid_api_mixing: return "VORBIS: can't mix API modes";
        case VORBIS_outofmem: return "VORBIS: out of memory";
        case VORBIS_feature_not_supported: return "VORBIS: feature not supported";
        case VORBIS_too_many_channels: return "VORBIS: too many channels";
        case VORBIS_file_open_failure: return "VORBIS: failed opening the file";
        case VORBIS_seek_without_length: return "VORBIS: can't seek in unknown length stream";
        case VORBIS_unexpected_eof: return "VORBIS: unexpected eof";
        case VORBIS_seek_invalid: return "VORBIS: invalid seek";
        case VORBIS_invalid_setup: return "VORBIS: invalid setup";
        case VORBIS_invalid_stream: return "VORBIS: invalid stream";
        case VORBIS_missing_capture_pattern: return "VORBIS: missing capture pattern";
        case VORBIS_invalid_stream_structure_version: return "VORBIS: invalid stream structure version";
        case VORBIS_continued_packet_flag_invalid: return "VORBIS: continued packet flag invalid";
        case VORBIS_incorrect_stream_serial_number: return "VORBIS: incorrect stream serial number";
        case VORBIS_invalid_first_page: return "VORBIS: invalid first page";
        case VORBIS_bad_packet_type: return "VORBIS: bad packet type";
        case VORBIS_cant_find_last_page: return "VORBIS: can't find last page";
        case VORBIS_seek_failed: return "VORBIS: seek failed";
        case VORBIS_ogg_skeleton_not_supported: return "VORBIS: multi-track streams are not supported; "
                                                       "consider re-encoding without the Ogg Skeleton bitstream";
        default: break;
    } /* switch */

    return "VORBIS: unknown error";
} /* vorbis_error_string */



static int VORBIS_init(void)
{
    return 1;  /* always succeeds. */
} /* VORBIS_init */

static void VORBIS_quit(void)
{
    /* it's a no-op. */
} /* VORBIS_quit */

static int VORBIS_open(Sound_Sample *sample, const char *ext)
{
    Sound_SampleInternal *internal = (Sound_SampleInternal *) sample->opaque;
    SDL_RWops *rw = internal->rw;
    int err = 0;
    stb_vorbis *stb = stb_vorbis_open_rwops(rw, 0, &err, NULL);

	if(stb == NULL) {
		LOG_MSG("%s (error code: %d)", vorbis_error_string(err), err);
        return 0;
	}
    internal->decoder_private = stb;
    sample->flags = SOUND_SAMPLEFLAG_CANSEEK;
    sample->actual.format = AUDIO_S16SYS;
    sample->actual.channels = stb->channels;
    sample->actual.rate = stb->sample_rate;
    const unsigned int num_frames = stb_vorbis_stream_length_in_samples(stb);
    if (!num_frames)
        internal->total_time = -1;
    else
    {
        const unsigned int rate = stb->sample_rate;
        internal->total_time = (num_frames / rate) * 1000;
        internal->total_time += (num_frames % rate) * 1000 / rate;
    } /* else */

    return 1; /* we'll handle this data. */
} /* VORBIS_open */


static void VORBIS_close(Sound_Sample *sample)
{
    Sound_SampleInternal *internal = (Sound_SampleInternal *) sample->opaque;
    stb_vorbis *stb = (stb_vorbis *) internal->decoder_private;
    stb_vorbis_close(stb);
} /* VORBIS_close */


static Uint32 VORBIS_read(Sound_Sample *sample)
{
    Uint32 retval;
    int rc;
    int err;
    Sound_SampleInternal *internal = (Sound_SampleInternal *) sample->opaque;
    stb_vorbis *stb = (stb_vorbis *) internal->decoder_private;
    const int channels = (int) sample->actual.channels;
    const int want_samples = (int) (internal->buffer_size / sizeof (int16_t));

    stb_vorbis_get_error(stb);  /* clear any error state */
    rc = stb_vorbis_get_samples_short_interleaved(stb, channels, (int16_t *) internal->buffer, want_samples);
    retval = (Uint32) (rc * channels * sizeof (int16_t));  /* rc == number of sample frames read */
    err = stb_vorbis_get_error(stb);

    if (retval == 0)
    {
        if (!err)
            sample->flags |= SOUND_SAMPLEFLAG_EOF;
        else
        {
            sample->flags |= SOUND_SAMPLEFLAG_ERROR;
        } /* else */
    } /* if */

    else if (retval < internal->buffer_size)
        sample->flags |= SOUND_SAMPLEFLAG_EAGAIN;

    return retval;
} /* VORBIS_read */


static int VORBIS_rewind(Sound_Sample *sample)
{
    Sound_SampleInternal *internal = (Sound_SampleInternal *) sample->opaque;
    stb_vorbis *stb = (stb_vorbis *) internal->decoder_private;

    if (!stb_vorbis_seek_start(stb)) {
        LOG_MSG("%s", vorbis_error_string(stb_vorbis_get_error(stb)));
        return 0;
    }

    return 1;
} /* VORBIS_rewind */


static int VORBIS_seek(Sound_Sample *sample, Uint32 ms)
{
    Sound_SampleInternal *internal = (Sound_SampleInternal *) sample->opaque;
    stb_vorbis *stb = (stb_vorbis *) internal->decoder_private;
    const float frames_per_ms = ((float) sample->actual.rate) / 1000.0f;
    const Uint32 frame_offset = (Uint32) (frames_per_ms * ((float) ms));
    const unsigned int sampnum = (unsigned int) frame_offset;

    if(!stb_vorbis_seek(stb, sampnum)) {
        LOG_MSG("%s", vorbis_error_string(stb_vorbis_get_error(stb)));
        return 0;
    }
    return 1;
} /* VORBIS_seek */


static const char *extensions_vorbis[] = { "OGG", "OGA", "VORBIS", NULL };
extern const Sound_DecoderFunctions __Sound_DecoderFunctions_VORBIS =
{
    {
        extensions_vorbis,
        "Ogg Vorbis audio",
        "Ryan C. Gordon <icculus@icculus.org>",
        "https://icculus.org/SDL_sound/"
    },

    VORBIS_init,       /*   init() method */
    VORBIS_quit,       /*   quit() method */
    VORBIS_open,       /*   open() method */
    VORBIS_close,      /*  close() method */
    VORBIS_read,       /*   read() method */
    VORBIS_rewind,     /* rewind() method */
    VORBIS_seek        /*   seek() method */
}; }

/* end of SDL_sound_vorbis.c ... */
