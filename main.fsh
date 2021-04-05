#version 330

// UV-coordinate of the fragment (interpolated by the rasterization stage)
in vec2 outUV;

// Color of the fragment received from the vertex shader (interpolated by the rasterization stage)
in vec3 outColor;

in vec3 fragNormal;
in vec3 fragPosition;


// Final color of the fragment that will be rendered on the screen
out vec4 fragColor;

uniform vec3 lightPos, lightColor ;

// Texture unit of the texture
uniform sampler2D tex;

void main()
{
	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos – fragPosition);
	float diff = max(dot(norm, lightDir), 0.f);
	vec3 diffuse = diff * lightColor;

	fragColor = texture(tex, outUV);
	vec4 finalColor= vec4(ambient + diffuse,1.0f) * fragColor;
	fragColor = finalColor;
}
