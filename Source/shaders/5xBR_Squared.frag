#version 330 core
/*
    Hyllian's 5xBR v3.7c (squared) Shader
   
   Copyright (C) 2011, 2012 Hyllian/Jararaca - sergiogdb@gmail.com
   Copyright (C) 2012 crazy46guy (GLSL conversion)

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   (The original Cg version of the xBR family of shaders is available on
   Github: https://github.com/twinaphex/common-shaders/tree/master/xBR )
*/

uniform sampler2D rubyTexture;
in vec2 rubyTextureSize;
in vec2 texCoord0;
in vec4 texCoord1;
in vec4 texCoord2;
in vec4 texCoord3;
in vec4 texCoord4;
in vec4 texCoord5;
in vec4 texCoord6;
in vec4 texCoord7;


const float coef = 2.0;

const float threshold = 15.0;

const float y_weight = 48.0;
const float u_weight = 7.0;
const float v_weight = 6.0;

const mat3 yuv = mat3(0.299, 0.587, 0.114, -0.169, -0.331, 0.499, 0.499, -0.418, -0.0813);
const mat3 yuv_weighted = mat3(y_weight * yuv[0], u_weight * yuv[1], v_weight * yuv[2]);

vec4 RGBtoYUV(vec3 v0, vec3 v1, vec3 v2, vec3 v3) {
  float a = yuv_weighted[0].x * v0.x + yuv_weighted[0].y * v0.y + yuv_weighted[0].z * v0.z;
  float b = yuv_weighted[0].x * v1.x + yuv_weighted[0].y * v1.y + yuv_weighted[0].z * v1.z;
  float c = yuv_weighted[0].x * v2.x + yuv_weighted[0].y * v2.y + yuv_weighted[0].z * v2.z;
  float d = yuv_weighted[0].x * v3.x + yuv_weighted[0].y * v3.y + yuv_weighted[0].z * v3.z;

  return vec4(a, b, c, d);
}

bvec4 _and_(bvec4 A, bvec4 B) {
  return bvec4(A.x && B.x, A.y && B.y, A.z && B.z, A.w && B.w);
}

bvec4 _or_(bvec4 A, bvec4 B) {
  return bvec4(A.x || B.x, A.y || B.y, A.z || B.z, A.w || B.w);
} 

vec4 df(vec4 A, vec4 B) {
  return vec4(abs(A - B));
}

bvec4 close(vec4 A, vec4 B) {
  return (lessThan(df(A, B), vec4(threshold)));
}

vec4 weighted_distance(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4 g, vec4 h) {
  return (df(a, b) + df(a, c) + df(d, e) + df(d, f) + 4.0 * df(g, h));
}

out vec4 fragColor;

void main() {
  vec2 fp = fract(texCoord0.xy * rubyTextureSize);

  vec3 A1 = texture(rubyTexture, texCoord1.xw).rgb;
  vec3 B1 = texture(rubyTexture, texCoord1.yw).rgb;
  vec3 C1 = texture(rubyTexture, texCoord1.zw).rgb;

  vec3 A  = texture(rubyTexture, texCoord2.xw).rgb;
  vec3 B  = texture(rubyTexture, texCoord2.yw).rgb;
  vec3 C  = texture(rubyTexture, texCoord2.zw).rgb;

  vec3 D  = texture(rubyTexture, texCoord3.xw).rgb;
  vec3 E  = texture(rubyTexture, texCoord3.yw).rgb;
  vec3 F  = texture(rubyTexture, texCoord3.zw).rgb;

  vec3 G  = texture(rubyTexture, texCoord4.xw).rgb;
  vec3 H  = texture(rubyTexture, texCoord4.yw).rgb;
  vec3 I  = texture(rubyTexture, texCoord4.zw).rgb;

  vec3 G5 = texture(rubyTexture, texCoord5.xw).rgb;
  vec3 H5 = texture(rubyTexture, texCoord5.yw).rgb;
  vec3 I5 = texture(rubyTexture, texCoord5.zw).rgb;

  vec3 A0 = texture(rubyTexture, texCoord6.xy).rgb;
  vec3 D0 = texture(rubyTexture, texCoord6.xz).rgb;
  vec3 G0 = texture(rubyTexture, texCoord6.xw).rgb;

  vec3 C4 = texture(rubyTexture, texCoord7.xy).rgb;
  vec3 F4 = texture(rubyTexture, texCoord7.xz).rgb;
  vec3 I4 = texture(rubyTexture, texCoord7.xw).rgb;

  vec4 b = RGBtoYUV(B, D, H, F);
  vec4 c = RGBtoYUV(C, A, G, I);
  vec4 e = RGBtoYUV(E, E, E, E);
  vec4 d = b.yzwx;
  vec4 f = b.wxyz;
  vec4 g = c.zwxy;
  vec4 h = b.zwxy;
  vec4 i = c.wxyz;

  vec4 i4 = RGBtoYUV(I4, C1, A0, G5);
  vec4 i5 = RGBtoYUV(I5, C4, A1, G0);
  vec4 h5 = RGBtoYUV(H5, F4, B1, D0);
  vec4 f4 = h5.yzwx;

  vec4 Ao = vec4( 1.0, -1.0, -1.0,  1.0 );
  vec4 Bo = vec4( 1.0,  1.0, -1.0, -1.0 );
  vec4 Co = vec4( 1.5,  0.5, -0.5,  0.5 );
  vec4 Ax = vec4( 1.0, -1.0, -1.0,  1.0 );
  vec4 Bx = vec4( 0.5,  2.0, -0.5, -2.0 );
  vec4 Cx = vec4( 1.0,  1.0, -0.5,  0.0 );
  vec4 Ay = vec4( 1.0, -1.0, -1.0,  1.0 );
  vec4 By = vec4( 2.0,  0.5, -2.0, -0.5 );
  vec4 Cy = vec4( 2.0,  0.0, -1.0,  0.5 );

  // These inequations define the line below which interpolation occurs
  bvec4 fx      = greaterThan(Ao * fp.y + Bo * fp.x, Co); 
  bvec4 fx_left = greaterThan(Ax * fp.y + Bx * fp.x, Cx);
  bvec4 fx_up   = greaterThan(Ay * fp.y + By * fp.x, Cy);

  bvec4 t1 = _and_( notEqual(e, f), notEqual(e, h) );
  bvec4 t2 = _and_( not(close(f, b)), not(close(f, c)) );
  bvec4 t3 = _and_( not(close(h, d)), not(close(h, g)) );
  bvec4 t4 = _and_( not(close(f, f4)), not(close(f, i4)) );
  bvec4 t5 = _and_( not(close(h, h5)), not(close(h, i5)) );
  bvec4 t6 = _and_( close(e, i), _or_(t4, t5) );
  bvec4 t7 = _or_( close(e, g), close(e, c) );
  bvec4 interp_restriction_lv1 = _and_( t1, _or_( _or_( _or_(t2, t3), t6 ), t7 ) );

  bvec4 interp_restriction_lv2_left = _and_( notEqual(e, g), notEqual(d, g) );
  bvec4 interp_restriction_lv2_up   = _and_( notEqual(e, c), notEqual(b, c) );

  bvec4 edr      = _and_( lessThan(weighted_distance(e, c, g, i, h5, f4, h, f), 
                                   weighted_distance(h, d, i5, f, i4, b, e, i)), interp_restriction_lv1 );
  bvec4 edr_left = _and_( lessThanEqual(coef * df(f, g), df(h, c)), interp_restriction_lv2_left );
  bvec4 edr_up   = _and_( greaterThanEqual(df(f, g), coef * df(h, c)), interp_restriction_lv2_up );

  bvec4 nc = _and_( edr, _or_( _or_( fx, _and_(edr_left, fx_left) ), _and_(edr_up, fx_up) ) );

  bvec4 px = lessThanEqual(df(e, f), df(e, h));

  vec3 res = nc.x ? px.x ? F : H : nc.y ? px.y ? B : F : nc.z ? px.z ? D : B : nc.w ? px.w ? H : D : E;

  fragColor.rgb = res;
  fragColor.a = 1.0;
}
