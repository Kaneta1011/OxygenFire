attribute vec3 Pos;
attribute vec2 Tex;

varying vec2 TexCoord;
//uniform float isFrameBuffer;

void main()
{
	gl_Position = vec4( Pos, 1.0 );
	TexCoord = Tex;
	//TexCoord.y *= isFrameBuffer;
}