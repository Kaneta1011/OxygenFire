precision mediump float;

uniform sampler2D colorTex;

varying vec2 TexCoord;

void main()
{
vec4 ret = vec4(TexCoord.x,TexCoord.y,1,1);
  vec4 texColor = texture2D(colorTex, TexCoord);
  gl_FragColor = texColor*ret;
}
