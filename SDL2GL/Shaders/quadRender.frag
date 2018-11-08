uniform sampler2D texture;
varying vec2 outUV;

void main()
{
	gl_FragColor=texture2D(texture,outUV);
}
