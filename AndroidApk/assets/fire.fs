precision mediump float;

uniform sampler2D colorTex;

uniform sampler2D maskTex;

uniform float alpha;

varying vec2 TexCoord;

float lerp(float x,float y,float s)
{
	return x+s*(y-x);
}

void main()
{
vec4 mask=texture2D(maskTex,TexCoord);
if (mask.r < alpha) discard;
float val=max(pow((1.0-(mask.r-alpha))*1.0,8.0),0.0);
vec4 diffuse = texture2D(colorTex, TexCoord);
//gl_FragColor = vec4(val,val,val,1.0);
vec3 point=vec3(1.0,0.8,0.6);

float pval=max((val*4.0)-3.0,0.0);
vec3 lim=vec3((pval+0.2)*0.8,pval*0.2,pval*0.1);
  gl_FragColor = (mix(diffuse,vec4(lim,1.0),val)+vec4(mix(point,vec3(0.0,0.0,0.0),1.0-pval),0.0))*diffuse.a;
}
