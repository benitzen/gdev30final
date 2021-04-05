#version 330

// Vertex position
layout(location = 0) in vec3 vertexPosition;

// Vertex color
layout(location = 1) in vec3 vertexColor;

// Vertex UV coordinate
layout(location = 2) in vec2 vertexUV;

// Vertex Normal
layout(location = 3) in vec3 vertexNormal;

out vec3 fragPosition;
out vec3 fragNormal;

// UV coordinate (will be passed to the fragment shader)
out vec2 outUV;

// Color (will be passed to the fragment shader)
out vec3 outColor;

// Transformation matrix
uniform mat4 transformationMatrix;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	// Convert our vertex position to homogeneous coordinates by introducing the w-component.
	// Vertex positions are ... positions, so we specify the w-coordinate as 1.0.
	vec4 semiFinalPosition = vec4(vertexPosition, 1.0);

	// We multiply the vertex position with our transformation matrix, which will
	// ultimately transform our vertex based on the combination of transformations that
	// is contained inside the transformationMatrix uniform.
    semiFinalPosition = transformationMatrix * semiFinalPosition;

	// Give OpenGL the final position of our vertex
    // mat3 modelNormalMat = mat3(transpose(inverse(model)));
    
    fragPosition = vec3(semiFinalPosition);
    fragNormal = vec3(model) * vertexNormal;
    
//    gl_Position = projection * view * model * semiFinalPosition;
    gl_Position = semiFinalPosition;
	
	outUV = vertexUV;
	outColor = vertexColor;
}
