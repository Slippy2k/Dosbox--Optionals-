/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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


#include <stdlib.h>
#include <math.h>
#include <map>

#include "dosbox.h"
#include "control.h"
#include "dos_inc.h"

#if C_OPENGL

#include "voodoo_vogl.h"

/* NTS: This causes errors in Linux because MesaGL already defines these */
#ifdef WIN32
PFNGLMULTITEXCOORD4FVARBPROC vglMultiTexCoord4fvARB = NULL;
PFNGLMULTITEXCOORD4FARBPROC vglMultiTexCoord4fARB = NULL;
PFNGLACTIVETEXTUREARBPROC vglActiveTextureARB = NULL;
#endif

PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = NULL;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = NULL;
PFNGLUNIFORM1IARBPROC glUniform1iARB = NULL;
PFNGLUNIFORM1FARBPROC glUniform1fARB = NULL;
PFNGLUNIFORM2FARBPROC glUniform2fARB = NULL;
PFNGLUNIFORM3FARBPROC glUniform3fARB = NULL;
PFNGLUNIFORM4FARBPROC glUniform4fARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB = NULL;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB = NULL;
PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB = NULL;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB = NULL;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = NULL;


static Bit32s opengl_version = -1;

static bool has_shaders = false;
static bool has_stencil = false;
static bool has_alpha = false;


static INT32 current_begin_mode = -1;

static Bit32s current_depth_mode = -1;
static Bit32s current_depth_func = -1;

static Bit32s current_alpha_enabled = -1;
static Bit32s current_src_rgb_fac = -1;
static Bit32s current_dst_rgb_fac = -1;
static Bit32s current_src_alpha_fac = -1;
static Bit32s current_dst_alpha_fac = -1;

static bool depth_masked = false;
static bool color_masked = false;
static bool alpha_masked = false;

// buffer read/write defaults are back-buffer for double buffered contexts
static bool draw_to_front_buffer = false;
static bool read_from_front_buffer = false;


void VOGL_Reset(void) {
	opengl_version = -1;
	has_shaders = false;
	has_stencil = false;
	has_alpha = false;

	current_depth_mode=-1;
	current_depth_func=-1;

	current_alpha_enabled=-1;
	current_src_rgb_fac=-1;
	current_dst_rgb_fac=-1;
	current_src_alpha_fac=-1;
	current_dst_alpha_fac=-1;

	depth_masked = false;
	color_masked = false;
	alpha_masked = false;

	draw_to_front_buffer = false;
	read_from_front_buffer = false;
}


void VOGL_InitVersion(void) {
	opengl_version = -1;
		
	char gl_verstr[16];
	const GLubyte* gl_verstr_ub = glGetString(GL_VERSION);
	strncpy(gl_verstr, (const char*)gl_verstr_ub, 16);
	
	LOG_MSG("VOODOO: GL Version: %s",glGetString(GL_VERSION));
	
	gl_verstr[15] = 0;
	char* gl_ver_minor = strchr(gl_verstr, '.');
	if (gl_ver_minor != NULL) {
		gl_ver_minor++;
		char* skip = strchr(gl_ver_minor, '.');
		if (skip != NULL) *skip = 0;
	}

	int ogl_ver = 100;
	if (gl_verstr[0] != 0) {
		int major = 1;
		int minor = 0;
		if (strchr(gl_verstr, '.') != NULL) {
			if (sscanf(gl_verstr,"%d.%d", &major, &minor) != 2) major = 0;
		} else {
			if (sscanf(gl_verstr, "%d", &major) != 1) major = 0;
		}
		if (major > 0) {
			ogl_ver = major*100;
			if (minor >= 0) {
				if (minor < 10) ogl_ver += minor*10;
				else ogl_ver += minor;
			}
		}
	}

	if (ogl_ver > 0) opengl_version = ogl_ver;
	
}

void VOGL_InitVersion_Fake(void) {
	//LOG_MSG("VOODOO: Opengl = Init & Get Version");

	int opengl_version;
	char glVersion[16];
	
	// Get GL Version From String
	const char *glVersion_ub = (const char*)glGetString(GL_VERSION);


	
	if (glVersion_ub)
	{ 
		LOG_MSG("VOODOO: GL Version: %s (Fake)",glGetString(GL_VERSION));	
		strncpy(glVersion, glVersion_ub,  16);
	}
	else
	{
		// Fake a GL Version
		const char glVersion_st[16] = {"2.0.0 Voodoo PC"};	 		
		strncpy(glVersion, (const char*)glVersion_st, 16);
		LOG_MSG("VOODOO: GL Version: %s (Fake)",glVersion);		
	}
	
	glVersion[16] = '\0';
	
	int major;
	int minor;
	char *gl_ver_minor = strchr(glVersion,'.');
	
	if (gl_ver_minor != NULL) {		
		sscanf(glVersion,"%d.%d",&major,&minor);
	}else{
		major = 1;
		major = 1;		
	}
	LOG_MSG("VOODOO: GL Version: %s (Fake)",glGetString(GL_VERSION));		
		
	int ogl_ver = 100;
	if (major > 0) {
		ogl_ver = major*100;		
		if (minor >= 0) {			
			if (minor < 10) {
				ogl_ver += minor*10;
			} else{
				ogl_ver += minor;
			}
		}
	}	
	if (ogl_ver > 0) opengl_version = ogl_ver;
}
void VOGL_ClearShaderFunctions(void) {
	glShaderSourceARB = NULL;
	glCompileShaderARB = NULL;
	glCreateProgramObjectARB = NULL;
	glAttachObjectARB = NULL;
	glLinkProgramARB = NULL;
	glUseProgramObjectARB = NULL;
	glUniform1iARB = NULL;
	glUniform1fARB = NULL;
	glUniform2fARB = NULL;
	glUniform3fARB = NULL;
	glUniform4fARB = NULL;
	glGetUniformLocationARB = NULL;
	glDetachObjectARB = NULL;
	glDeleteObjectARB  = NULL;
	glGetObjectParameterivARB = NULL;
	glGetInfoLogARB = NULL;
}

bool VOGL_Initialize(void) {
	VOGL_ClearShaderFunctions();
	
	Section_prop *section = static_cast<Section_prop *>(control->GetSection("pci"));
	bool compatibleFlag = section->Get_bool("compatible_flag");		
	
	Section_prop *sectsdl = static_cast<Section_prop *>(control->GetSection("sdl"));	
	const char *output  = sectsdl->Get_string("output");	
	const char *texture = sectsdl->Get_string("texture.renderer");	
	if (compatibleFlag || (strcasecmp(output,"surface") == 0) || (strcasecmp(texture,"direct3d") == 0) || (strcasecmp(texture,"software") == 0)|| (strcasecmp(texture,"auto") == 0)) {
		VOGL_InitVersion_Fake();
	}else{
		VOGL_InitVersion();
	}

#ifdef WIN32
	vglActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)((void*)SDL_GL_GetProcAddress("glActiveTextureARB"));
	if (!vglActiveTextureARB) {
		LOG_MSG("opengl: glActiveTextureARB extension not supported");
		return false;
	}

	vglMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC)((void*)SDL_GL_GetProcAddress("glMultiTexCoord4fARB"));
	if (!vglMultiTexCoord4fARB) {
		LOG_MSG("opengl: glMultiTexCoord4fARB extension not supported");
		return false;
	}

	vglMultiTexCoord4fvARB = (PFNGLMULTITEXCOORD4FVARBPROC)((void*)SDL_GL_GetProcAddress("glMultiTexCoord4fvARB"));
	if (!vglMultiTexCoord4fvARB) {
		LOG_MSG("opengl: glMultiTexCoord4fvARB extension not supported");
		return false;
	}
#endif

	glBlendFuncSeparateEXT = (PFNGLBLENDFUNCSEPARATEEXTPROC)((void*)SDL_GL_GetProcAddress("glBlendFuncSeparateEXT"));
	if (!glBlendFuncSeparateEXT) {
		LOG_MSG("opengl: glBlendFuncSeparateEXT extension not supported");
		return false;
	}

	glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)((void*)SDL_GL_GetProcAddress("glGenerateMipmapEXT"));
	if (!glGenerateMipmapEXT) {
		LOG_MSG("opengl: glGenerateMipmapEXT extension not supported");
		return false;
	}
	
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)((void*)SDL_GL_GetProcAddress("glGenerateMipmap"));
	if (!glGenerateMipmap) {
		LOG_MSG("opengl: glGenerateMipmap extension not supported");
		return false;
	}	
		
	//if (VOGL_CheckFeature(VOGL_ATLEAST_V20)) {
		const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
		if (strstr(extensions, "GL_ARB_shader_objects") && strstr(extensions, "GL_ARB_vertex_shader") &&
			strstr(extensions, "GL_ARB_fragment_shader")) {

			glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)((void*)SDL_GL_GetProcAddress("glCreateShaderObjectARB"));
			if (!glCreateShaderObjectARB) {
				LOG_MSG("opengl: shader extensions not supported. Using fixed pipeline");
			} else {
				glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)((void*)SDL_GL_GetProcAddress("glShaderSourceARB"));
				if (!glShaderSourceARB) LOG_MSG("opengl: glShaderSourceARB extension not supported");

				glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)((void*)SDL_GL_GetProcAddress("glCompileShaderARB"));
				if (!glCompileShaderARB) LOG_MSG("opengl: glCompileShaderARB extension not supported");

				glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)((void*)SDL_GL_GetProcAddress("glCreateProgramObjectARB"));
				if (!glCreateProgramObjectARB) LOG_MSG("opengl: glCreateProgramObjectARB extension not supported");

				glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)((void*)SDL_GL_GetProcAddress("glAttachObjectARB"));
				if (!glAttachObjectARB) LOG_MSG("opengl: glAttachObjectARB extension not supported");

				glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)((void*)SDL_GL_GetProcAddress("glLinkProgramARB"));
				if (!glLinkProgramARB) LOG_MSG("opengl: glLinkProgramARB extension not supported");

				glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)((void*)SDL_GL_GetProcAddress("glUseProgramObjectARB"));
				if (!glUseProgramObjectARB) LOG_MSG("opengl: glUseProgramObjectARB extension not supported");

				glUniform1iARB = (PFNGLUNIFORM1IARBPROC)SDL_GL_GetProcAddress("glUniform1iARB");
				if (!glUniform1iARB) LOG_MSG("opengl: glUniform1iARB extension not supported");

				glUniform1fARB = (PFNGLUNIFORM1FARBPROC)SDL_GL_GetProcAddress("glUniform1fARB");
				if (!glUniform1fARB) LOG_MSG("opengl: glUniform1fARB extension not supported");

				glUniform2fARB = (PFNGLUNIFORM2FARBPROC)SDL_GL_GetProcAddress("glUniform2fARB");
				if (!glUniform2fARB) LOG_MSG("opengl: glUniform2fARB extension not supported");

				glUniform3fARB = (PFNGLUNIFORM3FARBPROC)SDL_GL_GetProcAddress("glUniform3fARB");
				if (!glUniform3fARB) LOG_MSG("opengl: glUniform3fARB extension not supported");

				glUniform4fARB = (PFNGLUNIFORM4FARBPROC)SDL_GL_GetProcAddress("glUniform4fARB");
				if (!glUniform4fARB) LOG_MSG("opengl: glUniform4fARB extension not supported");

				glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)SDL_GL_GetProcAddress("glGetUniformLocationARB");
				if (!glGetUniformLocationARB) LOG_MSG("opengl: glGetUniformLocationARB extension not supported");

				glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)SDL_GL_GetProcAddress("glDetachObjectARB");
				if (!glDetachObjectARB) LOG_MSG("opengl: glDetachObjectARB extension not supported");

				glDeleteObjectARB  = (PFNGLDELETEOBJECTARBPROC)SDL_GL_GetProcAddress("glDeleteObjectARB");
				if (!glDeleteObjectARB) LOG_MSG("opengl: glDeleteObjectARB extension not supported");

				glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)SDL_GL_GetProcAddress("glGetObjectParameterivARB");
				if (!glGetObjectParameterivARB) LOG_MSG("opengl: glGetObjectParameterivARB extension not supported");

				glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)SDL_GL_GetProcAddress("glGetInfoLogARB");
				if (!glGetInfoLogARB) LOG_MSG("opengl: glGetInfoLogARB extension not supported");

				glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)SDL_GL_GetProcAddress("glGetAttribLocationARB");
				if (!glGetAttribLocationARB) LOG_MSG("opengl: glGetAttribLocationARB extension not supported");
			
				glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC)SDL_GL_GetProcAddress("glVertexAttrib1fARB");
				if (!glVertexAttrib1fARB) LOG_MSG("opengl: glVertexAttrib1fARB extension not supported");

				glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)SDL_GL_GetProcAddress("glPointParameterfv");
				if (!glPointParameterfv) LOG_MSG("opengl: glPointParameterfv extension not supported");
						
				
				if (glShaderSourceARB && glCompileShaderARB && glCreateProgramObjectARB &&
					glAttachObjectARB && glLinkProgramARB && glUseProgramObjectARB &&
					glUniform1iARB && glUniform1fARB && glUniform2fARB && glUniform3fARB &&
					glUniform4fARB && glGetUniformLocationARB && glDetachObjectARB &&
					glDeleteObjectARB && glGetObjectParameterivARB && glGetInfoLogARB) {
						VOGL_FlagFeature(VOGL_HAS_SHADERS);
						LOG_MSG("VOODOO: Shader Functionality Enabled");
				} else {
					VOGL_ClearShaderFunctions();
				}
			}
		//}
	}

	//LOG_MSG("opengl: I am able to use OpenGL to emulate Voodoo graphics");
	return true;
}


bool VOGL_CheckFeature(Bit32u feat) {
	switch (feat) {
		case VOGL_ATLEAST_V20:
			if (opengl_version >= 200) return true;
			break;
		case VOGL_ATLEAST_V21:
			if (opengl_version >= 210) return true;
			break;
		case VOGL_ATLEAST_V30:
			if (opengl_version >= 300) return true;
			break;
		case VOGL_HAS_SHADERS:
			if (has_shaders) return true;
			break;
		case VOGL_HAS_STENCIL_BUFFER:
			if (has_stencil) return true;
			break;
		case VOGL_HAS_ALPHA_PLANE:
			if (has_alpha) return true;
			break;
		default:
			LOG_MSG("opengl: unknown feature queried: %x",feat);
			break;
	}

	return false;
}

void VOGL_FlagFeature(Bit32u feat) {
	switch (feat) {
		case VOGL_HAS_SHADERS:
			has_shaders = true;
			break;
		case VOGL_HAS_STENCIL_BUFFER:
			has_stencil = true;
			break;
		case VOGL_HAS_ALPHA_PLANE:
			has_alpha = true;
			break;
		default:
			LOG_MSG("opengl: unknown feature: %x",feat);
			break;
	}
}


void VOGL_BeginMode(INT32 new_mode) {
	if (current_begin_mode > -1) {
		if (new_mode != current_begin_mode) {
			glEnd();
			if (new_mode > -1) glBegin(new_mode);
			current_begin_mode = new_mode;
		}
	} else {
		if (new_mode > -1) {
			glBegin(new_mode);
			current_begin_mode = new_mode;
		}
	}
}

void VOGL_ClearBeginMode(void) {
	if (current_begin_mode > -1) {
		glEnd();
		current_begin_mode = -1;
	}
}


void VOGL_SetDepthMode(Bit32s mode, Bit32s func) {
	if (current_depth_mode!=mode) {
		if (mode!=0) {
			VOGL_ClearBeginMode();
			glEnable(GL_DEPTH_TEST);
			current_depth_mode=1;
			if (current_depth_func!=func) {
				glDepthFunc(GL_NEVER+func);
				current_depth_func=func;
			}
		} else {
			VOGL_ClearBeginMode();
			glDisable(GL_DEPTH_TEST);
			current_depth_mode=0;
		}
	} else {
		if ((mode!=0) && (current_depth_func!=func)) {
			VOGL_ClearBeginMode();
			glDepthFunc(GL_NEVER+func);
			current_depth_func=func;
		}
	}
}


void VOGL_SetAlphaMode(Bit32s enabled_mode,GLuint src_rgb_fac,GLuint dst_rgb_fac,
											GLuint src_alpha_fac,GLuint dst_alpha_fac) {
	if (current_alpha_enabled!=enabled_mode) {
		VOGL_ClearBeginMode();
		if (enabled_mode!=0) {
			glEnable(GL_BLEND);
			current_alpha_enabled=1;
			if ((current_src_rgb_fac!=(Bit32s)src_rgb_fac) || (current_dst_rgb_fac!=(Bit32s)dst_rgb_fac) ||
				(current_src_alpha_fac!=(Bit32s)src_alpha_fac) || (current_dst_alpha_fac!=(Bit32s)dst_alpha_fac)) {
				glBlendFuncSeparateEXT(src_rgb_fac, dst_rgb_fac, src_alpha_fac, dst_alpha_fac);
				current_src_rgb_fac=(Bit32s)src_rgb_fac;
				current_dst_rgb_fac=(Bit32s)dst_rgb_fac;
				current_src_alpha_fac=(Bit32s)src_alpha_fac;
				current_dst_alpha_fac=(Bit32s)dst_alpha_fac;
			}
		} else {
			glDisable(GL_BLEND);
			current_alpha_enabled=0;
		}
	} else {
		if (current_alpha_enabled!=0) {
			if ((current_src_rgb_fac!=(Bit32s)src_rgb_fac) || (current_dst_rgb_fac!=(Bit32s)dst_rgb_fac) ||
				(current_src_alpha_fac!=(Bit32s)src_alpha_fac) || (current_dst_alpha_fac!=(Bit32s)dst_alpha_fac)) {
				VOGL_ClearBeginMode();
				glBlendFuncSeparateEXT(src_rgb_fac, dst_rgb_fac, src_alpha_fac, dst_alpha_fac);
				current_src_rgb_fac=(Bit32s)src_rgb_fac;
				current_dst_rgb_fac=(Bit32s)dst_rgb_fac;
				current_src_alpha_fac=(Bit32s)src_alpha_fac;
				current_dst_alpha_fac=(Bit32s)dst_alpha_fac;
			}
		}
	}
}


void VOGL_SetDepthMaskMode(bool masked) {
	if (depth_masked!=masked) {
		VOGL_ClearBeginMode();
		if (masked) {
			glDepthMask(GL_TRUE);
			depth_masked=true;
		} else {
			glDepthMask(GL_FALSE);
			depth_masked=false;
		}
	}
}


void VOGL_SetColorMaskMode(bool cmasked, bool amasked) {
	if ((color_masked!=cmasked) || (alpha_masked!=amasked)) {
		color_masked=cmasked;
		alpha_masked=amasked;
		GLboolean cm = (color_masked ? GL_TRUE : GL_FALSE);
		GLboolean am = (alpha_masked ? GL_TRUE : GL_FALSE);
		glColorMask(cm,cm,cm,am);
	}
}


void VOGL_SetDrawMode(bool front_draw) {
	if (draw_to_front_buffer!=front_draw) {
		VOGL_ClearBeginMode();
		if (front_draw) glDrawBuffer(GL_FRONT);
		else glDrawBuffer(GL_BACK);
		draw_to_front_buffer=front_draw;
	}
}


void VOGL_SetReadMode(bool front_read) {
	VOGL_ClearBeginMode();

	if (read_from_front_buffer!=front_read) {
		
		if (front_read){
			glReadBuffer(GL_FRONT);
			// glReadBuffer(GL_LEFT);
			// glReadBuffer(GL_FRONT_RIGHT);
			// glReadBuffer(GL_RIGHT);
		}
		else
		{
			glReadBuffer(GL_BACK);
		}
		read_from_front_buffer=front_read;
	}
}

#endif
