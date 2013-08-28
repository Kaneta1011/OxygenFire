
attribute vec3 Pos;

uniform float size;
uniform float offsetX;
uniform float offsetY;

void main()
{
	gl_Position = vec4( Pos, 1.0 );
	gl_Position.x += offsetX * size * 0.5;
	gl_Position.y += offsetY * size * 0.5;
	gl_Position.xy = gl_Position.xy*2.0 - 1.0;
	gl_Position.y *= -1.0;
	gl_PointSize = size;
}