//uniform vec2 frag_LightOrigin;
//uniform float mattenuation;
//uniform vec3 frag_LightColor;
//uniform vec2 frag_ScreenResolution;*/
uniform sampler2D texture;
uniform vec2 center;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	//vec2 baseDistance =  gl_FragCoord.xy;
	//baseDistance.y = frag_ScreenResolution.y - baseDistance.y;
	/*float attenuation = 1.0f / (mattenuation);
	vec4 lightColor = vec4(frag_LightColor, 1.0f);
	vec4 color = vec4(attenuation, attenuation, attenuation, 1.0f) * lightColor;*/
	vec2 distance = gl_FragCoord.xy - center;
	float dx = gl_FragCoord.x - center.x;
	float dy = gl_FragCoord.y - center.y;
	float dist2 = dx * dx + dy * dy;
	int lenght = (dist2);
	if((lenght % 500) > 300)
		gl_FragColor = vec4(gl_Color.r, gl_Color.g, gl_Color.b, gl_Color.a * 0.8) * pixel;
	else
		gl_FragColor = gl_Color;// * pixel;
};