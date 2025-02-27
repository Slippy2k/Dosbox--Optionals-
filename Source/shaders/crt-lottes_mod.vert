#version 330 core

//
// PUBLIC DOMAIN CRT STYLED SCAN-LINE SHADER
//
//   by Timothy Lottes
//
// This is more along the style of a really good CGA arcade monitor.
// With RGB inputs instead of NTSC.
// The shadow mask example has the mask rotated 90 degrees for less chromatic aberration.
//
// Left it unoptimized to show the theory behind the algorithm.
//
// It is an example what I personally would want as a display option for pixel art games.
// Please take and use, change, or whatever.
//

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

layout (std140) uniform program
{
	vec2 video_size;
	vec2 texture_size;
	vec2 output_size;
} IN;

out vec2 tex;
out vec2 OGL2Size;

void main()
{
	gl_Position = position;
	tex = texCoord;
	OGL2Size = IN.texture_size;	
}
