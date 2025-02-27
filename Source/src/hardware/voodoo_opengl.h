 /*
 *  Copyright (C) 2002-2011  The DOSBox Team
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


#ifndef DOSBOX_VOODOO_OPENGL_H
#define DOSBOX_VOODOO_OPENGL_H

#include <stdlib.h>
#include <math.h>
#include <map>

#include "dosbox.h"
#include "cross.h"


#if C_OPENGL

#include "voodoo_vogl.h"
#include "voodoo_types.h"

void _check_gl_error(const char *file, int line);
 
///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

void glCheckError(const GLchar* file, GLuint line, const GLchar* expression);
#define glCheck(expr) do { expr; glCheckError(__FILE__, __LINE__, #expr); } while (false)

/* texture data */
struct ogl_texture_data {
	GLuint texID;
	bool enable;
};

/* triangle vertex map data */
struct ogl_vertex_map_data {
	float sw,tw,z,w;
	float s,t;
	float lodblend;
};

/* triangle vertex data */
struct ogl_vertex_data {
	float x,y,d,w,z;
	float r,g,b,a;
	float fogblend;
	ogl_vertex_map_data m[2];
};

struct ogl_texmap {
	bool valid_data;
	bool valid_pal;
	UINT32 format;
	UINT32 current_id;

	std::map<const UINT32, GLuint>* ids;
};

#endif


bool voodoo_ogl_init(voodoo_state *v);
void voodoo_ogl_leave(bool leavemode);
void voodoo_ogl_shutdown(voodoo_state *v);

void voodoo_ogl_set_window(voodoo_state *v);
void voodoo_ogl_swap_buffer(void);
void voodoo_ogl_vblank_flush(void);

void voodoo_ogl_update_dimensions(void);
void voodoo_ogl_clear(void);
void voodoo_ogl_fastfill(void);

void voodoo_ogl_clip_window(voodoo_state *v);
void voodoo_ogl_texture_clear(UINT32 texbase, int TMU);
void voodoo_ogl_invalidate_paltex(void);

void voodoo_ogl_draw_pixel(int x, int y, bool has_rgb, bool has_alpha, int r, int g, int b, int a);
void voodoo_ogl_draw_z(int x, int y, int z);
void voodoo_ogl_draw_pixel_pipeline(int x, int y, int r, int g, int b);
UINT32 voodoo_ogl_read_pixel(int x, int y);

void voodoo_ogl_draw_triangle(poly_extra_data *extra);

using namespace std;
class OpenGLReadWriteSpeed
{
public:
    // OpenGLReadWriteSpeed(const GLdouble maximumSamplePeriod = 0.5);
   // ~OpenGLReadWriteSpeed();

       typedef struct
    {
        GLenum format;
        GLenum type;
        string name;
    } modeType;

   const static modeType  _modes[11];
};

const OpenGLReadWriteSpeed::modeType _modes[11] =
{
       { GL_DEPTH_COMPONENT,  GL_INT,            "GLint    Depth"   },
       { GL_DEPTH_COMPONENT,  GL_UNSIGNED_INT,   "GLuint   Depth"   },
       { GL_DEPTH_COMPONENT,  GL_SHORT,          "GLshort  Depth"   },
       { GL_DEPTH_COMPONENT,  GL_UNSIGNED_SHORT, "GLushort Depth"   },
       { GL_DEPTH_COMPONENT,  GL_FLOAT,          "GLfloat  Depth"   },
       { GL_STENCIL_INDEX,    GL_BYTE,           "GLbyte   Stencil" },
       { GL_STENCIL_INDEX,    GL_UNSIGNED_BYTE,  "GLubyte  Stencil" },
       { GL_RGBA,             GL_BYTE,           "GLbyte   RGBA"    },
       { GL_RGBA,             GL_UNSIGNED_BYTE,  "GLubyte  RGBA"    },
       { GL_BGRA,             GL_BYTE,           "GLbyte   BGRA"    },
       { GL_BGRA,             GL_UNSIGNED_BYTE,  "GLubyte  BGRA"    }
};


    //const GLdouble  _maximumSamplePeriod;

    //const static GltString _banner;
    //const static GLdouble  _mill;



#endif
