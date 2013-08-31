
attribute vec3 Pos;
attribute vec2 Tex;

attribute vec4 Color;

varying vec2 io_TexCoord;

varying vec4 io_Color;

uniform mat4 WVP;

//uniform mat4 W;
//uniform mat4 V;
//uniform mat4 P;

void main()
{
	io_TexCoord = Tex;

	io_Color = Color;

	//mat4 m;
	//m._11=W._11*V._11*P._11; m._12=W._12*V._12*P._12; m._13=W._13*V._13*P._13; m._14=W._14*V._14*P._14;
	//m._21=W._21*V._21*P._21; m._22=W._22*V._22*P._22; m._23=W._23*V._23*P._23; m._24=W._24*V._24*P._24;
	//m._31=W._31*V._31*P._31; m._32=W._32*V._32*P._32; m._33=W._33*V._33*P._33; m._34=W._34*V._34*P._34;
	//m._41=W._41*V._41*P._41; m._42=W._42*V._42*P._42; m._43=W._43*V._43*P._43; m._44=W._44*V._44*P._44;

	vec4 opos = WVP * vec4(Pos,1);
	opos.z=2.0*opos.z-opos.w;
	gl_Position = opos;

	//gl_Position = vec4(Pos,1);
}