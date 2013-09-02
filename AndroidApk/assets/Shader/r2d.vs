attribute vec3 Pos;
attribute vec2 Tex;

varying vec2 TexCoord;

void main()
{
    gl_Position = vec4( Pos, 1.0 );
    TexCoord = Tex;
}
