attribute vec4 VPosition;
attribute vec2 VTexCoord;
varying vec2 io_TexCoord;

uniform mat4 WVP;

void main()
{
	io_TexCoord = VTexCoord;
	vec4 opos = WVP * VPosition;
	opos.z=2.0*opos.z-opos.w;
	gl_Position = opos;
}