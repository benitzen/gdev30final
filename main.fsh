#version 330

// UV-coordinate of the fragment (interpolated by the rasterization stage)
in vec2 outUV;

// Color of the fragment received from the vertex shader (interpolated by the rasterization stage)
in vec3 outColor;

in vec3 fragNormal;
in vec3 fragPosition;


// Final color of the fragment that will be rendered on the screen
out vec4 fragColor;

uniform vec3 lightPos, lightColor;

uniform float ambientStrength;

// Texture unit of the texture
uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, outUV);
    
	// Ambient
	vec3 ambient = ambientStrength * lightColor;
	

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPosition);
	vec3 diffuseColor = vec3(1.0f,1.0f,1.0f);
	float diff = clamp(dot(lightDir, fragNormal), 0,1);
	vec3 diffuseFinal = diffuseColor * diff;


//    https://opengl-notes.readthedocs.io/en/latest/topics/texturing/aliasing.html
    float fogMax = 1.0;
    float fogMin = 0.1;
    vec4  fogColor = vec4(0.6, 0.6, 0.6, 1.0);

    // Calculate fog
    float fogFactor = (fogMax - 0.3f) / (fogMax - fogMin);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    
    
//	vec3 finalColor= (ambient + diffuseFinal) * outColor;
    vec4 finalColor= vec4(ambient + diffuseFinal, 1.0f) * fragColor;
//	fragColor = fragColor * vec4(finalColor, 1.0f);
    fragColor = finalColor * fogFactor * fogColor;
}
