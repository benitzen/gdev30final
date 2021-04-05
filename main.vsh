#version 330

// Vertex position
layout(location = 0) in vec3 vertexPosition;

// Vertex color
layout(location = 1) in vec3 vertexColor;

// Vertex UV coordinate
layout(location = 2) in vec2 vertexUV;

// UV coordinate (will be passed to the fragment shader)
out vec2 outUV;

// Color (will be passed to the fragment shader)
out vec3 outColor;

// Transformation matrix
uniform mat4 transformationMatrix;

void main()
{
	// Convert our vertex position to homogeneous coordinates by introducing the w-component.
	// Vertex positions are ... positions, so we specify the w-coordinate as 1.0.
	vec4 finalPosition = vec4(vertexPosition, 1.0);

	// We multiply the vertex position with our transformation matrix, which will
	// ultimately transform our vertex based on the combination of transformations that
	// is contained inside the transformationMatrix uniform.
	finalPosition = transformationMatrix * finalPosition;

	// Give OpenGL the final position of our vertex
	gl_Position = finalPosition;

	outUV = vertexUV;
	outColor = vertexColor;
}
