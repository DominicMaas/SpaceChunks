#version 400 core

varying vec4 texcoord;

uniform vec4 colorIn;

const vec4 fogcolor = vec4(0.12f, 0.56f, 1.0f, 1.0f);
const float fogdensity = 0.00002;

vec3 UnpackBlock(uint data)
{
    uint r = (data >> 16) & 0xff; 
	uint g = (data >> 8) & 0xff; 
	uint b = (data) & 0xff;
	
	return vec3(r, g, b);
}

vec3 GetBaseRGBFromBlock(uint block)
{
	vec3 i = UnpackBlock(block);
	
	i = (1.0f / 255)*i;
	
	return i;
}

void main()
{
	float intensity;

	if(texcoord.w < 0.0)
		intensity = 1.0;
	else
		intensity = 0.85;
	
	vec4 color = vec4(GetBaseRGBFromBlock(4155430), 1.0);
	
	 if(color.a < 0.4)
        discard;
	
	intensity -= 0.5;
	
	color.xyz *= intensity;

    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fog = clamp(exp(-fogdensity * z * z), 0.4, 1.0);
 
    gl_FragColor = color; //mix(fogcolor, color, fog);
}