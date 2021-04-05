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
    fragColor = texture(tex, outUV);
    
	// Ambient
	float ambientStrength = 2.0f;
	vec3 ambient = ambientStrength * lightColor;
	

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPosition - lightPos);
	vec3 diffuseColor = vec3(1.f,1.f,1.f);
	float diff = clamp(dot(lightDir, fragNormal), 0,1);
	vec3 diffuseFinal = diffuseColor * diff;

	
	vec4 finalColor= vec4(ambient + diffuseFinal, 1.0f) * fragColor;
	fragColor = finalColor;
}
