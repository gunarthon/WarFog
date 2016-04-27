//uniform vec2 frag_LightOrigin;
//uniform float mattenuation;
//uniform vec3 frag_LightColor;
//uniform vec2 frag_ScreenResolution;*/
uniform sampler2D texture;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	//vec2 baseDistance =  gl_FragCoord.xy;
	//baseDistance.y = frag_ScreenResolution.y - baseDistance.y;
	/*float attenuation = 1.0f / (mattenuation);
	vec4 lightColor = vec4(frag_LightColor, 1.0f);
	vec4 color = vec4(attenuation, attenuation, attenuation, 1.0f) * lightColor;*/
	if(((int) gl_FragCoord.x % 10) > 5 || ((int) gl_FragCoord.y % 10) > 5)
		gl_FragColor = vec4(gl_Color.r, gl_Color.g, gl_Color.b, gl_Color.a * 0.8) * pixel;
	else
		gl_FragColor = gl_Color;// * pixel;
};
