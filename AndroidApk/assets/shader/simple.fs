precision mediump float;
varying vec2 io_TexCoord;

//  Uniform
uniform sampler2D ModelTex;

void main() 
{
  gl_FragColor = texture(ModelTex,io_TexCoord);
  //gl_FragColor = vec4(0,1,0,1);
}