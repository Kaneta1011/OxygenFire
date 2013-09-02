precision mediump float;

varying vec2 io_TexCoord;

varying vec4 io_Color;

//uniform float test;

uniform sampler2D uTex;

void main()
{
  vec4 texColor = texture2D(uTex,io_TexCoord);

  //vec4 c = vec4( io_Color, 1, 1 );

  gl_FragColor =  texColor * io_Color ;

  //unsigned int c = unsigned int(test);

  //unsigned int t = floatBitsToUint(texColor.w);

  //gl_FragColor = io_Color;

  //gl_FragColor = vec4(0,1,0,1);
}