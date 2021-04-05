// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// ---------------
// Function declarations
// ---------------

/// <summary>
/// Creates a shader program based on the provided file paths for the vertex and fragment shaders.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
/// <returns>OpenGL handle to the created shader program</returns>
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

/// <summary>
/// Creates a shader based on the provided shader type and the path to the file containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderFilePath">Path to the file containing the shader source</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath);

/// <summary>
/// Creates a shader based on the provided shader type and the string containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderSource">Shader source string</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource);

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);

/// <summary>
/// Struct containing data about a vertex
/// </summary>
struct Vertex
{
	GLfloat x, y, z;	// Position
	GLubyte r, g, b;	// Color
	GLfloat u, v;		// UV coordinates
};

// Psuedotime
GLfloat pseudoTime = 0.0f;

/// <summary>
/// Main function.
/// </summary>
/// <returns>An integer indicating whether the program ended successfully or not.
/// A value of 0 indicates the program ended succesfully, while a non-zero value indicates
/// something wrong happened during execution.</returns>
int main()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return 1;
	}

	// Tell GLFW that we prefer to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW that we prefer to use the modern OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tell GLFW to create a window
	float windowWidth = 800;
	float windowHeight = 800;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Coordinate Spaces", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(window);

	// Register the callback function that handles when the framebuffer size has changed
	glfwSetFramebufferSizeCallback(window, FramebufferSizeChangedCallback);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---

    Vertex vertices[36];

                    // Position                Color                UV
    //Front
    vertices[0] = { -0.5f, -0.5f, 0.5f,        255, 255, 255,        0.25f, 0.67f };    // Lower-left
    vertices[1] = { 0.5f, -0.5f, 0.5f,        255, 255, 255,        0.5f, 0.67f };    // Lower-right
    vertices[2] = { 0.5f, 0.5f, 0.5f,        255, 255, 255,        0.5f, 0.33f };    // Upper-right

    vertices[3] = { 0.5f, 0.5f, 0.5f,        255, 255, 255,        0.5f, 0.33f };    // Upper-right
    vertices[4] = { -0.5f, 0.5f, 0.5f,        255, 255, 255,        0.25f, 0.33f };    // Upper-left
    vertices[5] = { -0.5f, -0.5f, 0.5f,        255, 255, 255,        0.25f, 0.67f };    // Lower-left
    
    //Back
    vertices[6] = { -0.5f, -0.5f, -0.5f,        255, 255, 255,        1.0f, 0.67f };    // Lower-left
    vertices[7] = { 0.5f, -0.5f, -0.5f,        255, 255, 255,        0.75f, 0.67f };    // Lower-right
    vertices[8] = { 0.5f, 0.5f, -0.5f,        255, 255, 255,        0.75f, 0.33f };    // Upper-right

    vertices[9] = { 0.5f, 0.5f, -0.5f,        255, 255, 255,        0.75f, 0.33f };    // Upper-right
    vertices[10] = { -0.5f, 0.5f, -0.5f,        255, 255, 255,        1.0f, 0.33f };    // Upper-left
    vertices[11] = { -0.5f, -0.5f, -0.5f,        255, 255, 255,        1.0f, 0.67f };    // Lower-left
    
    //Left Face
    vertices[12] = { -0.5f, -0.5f, -0.5f,        255, 255, 255,      0.0f, 0.67f };// Lower-left back
    vertices[13] = { -0.5f, -0.5f, 0.5f,        255, 255, 255,       0.25f, 0.67f }; // Lower-left front
    vertices[14] = { -0.5f, 0.5f, 0.5f,        255, 255, 255,       0.25f, 0.33f };  // Upper-left front
    
    vertices[15] = { -0.5f, 0.5f, 0.5f,        255, 255, 255,       0.25f, 0.33f };  // Upper-left front
    vertices[16] = { -0.5f, 0.5f, -0.5f,        255, 255, 255,       0.0f, 0.33f };  // Upper-left back
    vertices[17] = { -0.5f, -0.5f, -0.5f,        255, 255, 255,      0.0f, 0.67f };  // Lower-left back
    
    //Right Face
    vertices[18] = { 0.5f, -0.5f, 0.5f,        255, 255, 255,      0.5f, 0.67f };  // Lower-right Front
    vertices[19] = { 0.5f, -0.5f, -0.5f,        255, 255, 255,     0.75f, 0.67f };  // Lower-right Back
    vertices[20] = { 0.5f, 0.5f, -0.5f,        255, 255, 255,     0.75f, 0.33f };   // Upper-right Back
    
    vertices[21] = { 0.5f, 0.5f, -0.5f,        255, 255, 255,      0.75f, 0.33f };   // Upper-right Back
    vertices[22] = { 0.5f, 0.5f, 0.5f,        255, 255, 255,      0.5f, 0.33f };   // Upper-right Front
    vertices[23] = { 0.5f, -0.5f, 0.5f,        255, 255, 255,     0.5f, 0.67f };  // Lower-right Front
    
    //Top
    vertices[24] = { -0.5f, 0.5f, 0.5f,        255, 255, 255,      0.25f, 0.33f };  // Upper-left Front
    vertices[25] = { 0.5f, 0.5f, 0.5f,        255, 255, 255,       0.5f, 0.33f };  // Upper-right Front
    vertices[26] = { 0.5f, 0.5f, -0.5f,        255, 255, 255,        0.5f, 0.0f };  // Upper-right Back
    
    vertices[27] = { 0.5f, 0.5f, -0.5f,        255, 255, 255,        0.5f, 0.0f };  // Upper-right Back
    vertices[28] = { -0.5f, 0.5f, -0.5f,        255, 255, 255,        0.25f, 0.0f };  // Upper-left Back
    vertices[29] = { -0.5f, 0.5f, 0.5f,        255, 255, 255,      0.25f, 0.33f };  // Upper-left Front
     
    //Bottom
    vertices[30] = { -0.5f, -0.5f, -0.5f,        255, 255, 255,     0.25f, 1.0f };  // Lower-left Back
    vertices[31] = { 0.5f, -0.5f, -0.5f,        255, 255, 255,      0.5f, 1.0f };  // Lower-right Back
    vertices[32] = { 0.5f, -0.5f, 0.5f,        255, 255, 255,       0.5f, 0.67f };  // Lower-right Front
    
    vertices[33] = { 0.5f, -0.5f, 0.5f,        255, 255, 255,      0.5f, 0.67f };  // Lower-right Front
    vertices[34] = { -0.5f, -0.5f, 0.5f,        255, 255, 255,      0.25f, 0.67f };  // Lower-left Front
    vertices[35] = { -0.5f, -0.5f, -0.5f,        255, 255, 255,      0.25f, 1.0f };  // Lower-left Back

	// Create a vertex buffer object (VBO), and upload our vertices data to the VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, r)));

	// Vertex attribute 2 - UV coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));

	glBindVertexArray(0);

	// Create a shader program
	GLuint program = CreateShaderProgram("/Users/Anton/Documents/OpenGL/projects/helloTriangle/helloTriangle/main.vsh", "/Users/Anton/Documents/OpenGL/projects/helloTriangle/helloTriangle/main.fsh");

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
//	glViewport(0, 0, windowWidth, windowHeight);

	// Create a variable that will contain the ID for our texture,
	// and use glGenTextures() to generate the texture itself
	GLuint tex;
	glGenTextures(1, &tex);

	// --- Load our image using stb_image ---

	// Im image-space (pixels), (0, 0) is the upper-left corner of the image
	// However, in u-v coordinates, (0, 0) is the lower-left corner of the image
	// This means that the image will appear upside-down when we use the image data as is
	// This function tells stbi to flip the image vertically so that it is not upside-down when we use it
	stbi_set_flip_vertically_on_load(true);

	// 'imageWidth' and imageHeight will contain the width and height of the loaded image respectively
	int imageWidth, imageHeight, numChannels;

	// Read the image data and store it in an unsigned char array
	unsigned char* imageData = stbi_load("/Users/Anton/Documents/OpenGL/projects/helloTriangle/helloTriangle/dice.jpg", &imageWidth, &imageHeight, &numChannels, 0);

	// Make sure that we actually loaded the image before uploading the data to the GPU
	if (imageData != nullptr)
	{
		// Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
		glBindTexture(GL_TEXTURE_2D, tex);

		// Set the filtering methods for magnification and minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Set the wrapping method for the s-axis (x-axis) and t-axis (y-axis)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload the image data to GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// If we set minification to use mipmaps, we can tell OpenGL to generate the mipmaps for us
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Once we have copied the data over to the GPU, we can delete
		// the data on the CPU side, since we won't be using it anymore
		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

    //Depth tst
    glEnable(GL_DEPTH_TEST);
    
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
        
        pseudoTime += 1.3f;
		// Clear the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		// Bind our texture to texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		// Make our sampler in the fragment shader use texture unit 0
		GLint texUniformLocation = glGetUniformLocation(program, "tex");
		glUniform1i(texUniformLocation, 0);
        
        
        // Cube 1
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform,  glm::radians(pseudoTime), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(0.3f, 0.3f, 0.3f));
        
        glm::mat4 viewmat = glm::lookAt(glm::vec3(0.5, 0.0, 1.25), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projmat = glm::perspective(90.0f, 1.0f, 0.001f, 100.f);
        
        glm::mat4 product = projmat * viewmat * transform;

        GLint transformationMatrixUniformLocation = glGetUniformLocation(program, "transformationMatrix");
        glUniformMatrix4fv(transformationMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(product));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glDrawArrays(GL_TRIANGLES, 30, 6);

        // Cube 2
        glm::mat4 transformTwo = glm::mat4(1.0f);
        transformTwo = glm::translate(transformTwo, glm::vec3(-0.5f, 1.8f, -0.3f));
        transformTwo = glm::rotate(transformTwo,  glm::radians(pseudoTime), glm::vec3(-1.0f, 1.0f, 1.0f));
        transformTwo = glm::scale(transformTwo, glm::vec3(0.3f, 0.3f, 0.3f));
        
        product = projmat * viewmat * transformTwo;
        glUniformMatrix4fv(transformationMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(product));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glDrawArrays(GL_TRIANGLES, 30, 6);
        
        // Cube 3
        glm::mat4 transformThree = glm::mat4(1.0f);
        transformThree = glm::translate(transformThree, glm::vec3(-1.0f, -1.0f, 0.3f));
        transformThree = glm::rotate(transformThree,  glm::radians(pseudoTime), glm::vec3(1.0f, 0.0f, 0.0f));
        transformThree = glm::scale(transformThree, glm::vec3(0.4f, 0.4f, 0.4f));
        product = projmat * viewmat * transformThree;
        glUniformMatrix4fv(transformationMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(product));

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glDrawArrays(GL_TRIANGLES, 30, 6);
        
        // Cube 4 rotating on origin
        glm::mat4 transformFour = glm::mat4(1.0f);
        transformFour = glm::translate(transformFour, glm::vec3(sinf(pseudoTime/70)/2, cosf(pseudoTime/70)/2, 0.0f));
        transformFour = glm::rotate(transformFour,  glm::radians(pseudoTime), glm::vec3(2.0f, 0.0f, 0.0f));
        transformFour = glm::scale(transformFour, glm::vec3(0.3f, 0.3f, 0.3f));
        
        product = projmat * viewmat * transformFour;
        glUniformMatrix4fv(transformationMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(product));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glDrawArrays(GL_TRIANGLES, 30, 6);

    
        // Cube 5 rotating on cube 1
        glm::mat4 transformFive = glm::mat4(1.0f);
        //translate is at cube 1's local origin
        transformFive = glm::translate(transformFive, glm::vec3(1.0f, 0.0f, 0.0f));
        // Sin and Cos divided by 2 to achieve a 0.5 radius
        transformFive = glm::translate(transformFive, glm::vec3(-cosf(pseudoTime/70)/2.5, cosf(pseudoTime/70)/2.5, sinf(pseudoTime/70)/2.5));
        transformFive = glm::rotate(transformFive,  glm::radians(pseudoTime), glm::vec3(2.0f, 1.0f, 1.0f));
        transformFive = glm::scale(transformFive, glm::vec3(0.15f, 0.15f, 0.15f));
        
        product = projmat * viewmat * transformFive;
        glUniformMatrix4fv(transformationMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(product));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glDrawArrays(GL_TRIANGLES, 30, 6);
        
        
        //cube 6 at origin
        glm::mat4 transformSix = glm::mat4(1.0f);
        transformSix = glm::scale(transformSix, glm::vec3(0.2f, 0.2f, 0.2f));
        product = projmat * viewmat * transformSix;
        glUniformMatrix4fv(transformationMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(product));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glDrawArrays(GL_TRIANGLES, 30, 6);

        
		// "Unuse" the vertex array object
		glBindVertexArray(0);

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(window);

		// Tell GLFW to process window events (e.g., input events, window closed events, etc.)
		glfwPollEvents();
	}

	// --- Cleanup ---

	// Make sure to delete the shader program
	glDeleteProgram(program);

	// Delete the VBO that contains our vertices
	glDeleteBuffers(1, &vbo);

	// Delete the vertex array object
	glDeleteVertexArrays(1, &vao);

	// Remember to tell GLFW to clean itself up before exiting the application
	glfwTerminate();

	return 0;
}

/// <summary>
/// Creates a shader program based on the provided file paths for the vertex and fragment shaders.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
/// <returns>OpenGL handle to the created shader program</returns>
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	GLuint vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilePath);
	GLuint fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	// Check shader program link status
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetProgramInfoLog(program, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "program link error: " << infoLog << std::endl;
	}

	return program;
}

/// <summary>
/// Creates a shader based on the provided shader type and the path to the file containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderFilePath">Path to the file containing the shader source</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath)
{
	std::ifstream shaderFile(shaderFilePath);
	if (shaderFile.fail())
	{
		std::cerr << "Unable to open shader file: " << shaderFilePath << std::endl;
		return 0;
	}

	std::string shaderSource;
	std::string temp;
	while (std::getline(shaderFile, temp))
	{
		shaderSource += temp + "\n";
	}
	shaderFile.close();

	return CreateShaderFromSource(shaderType, shaderSource);
}

/// <summary>
/// Creates a shader based on the provided shader type and the string containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderSource">Shader source string</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource)
{
	GLuint shader = glCreateShader(shaderType);

	const char* shaderSourceCStr = shaderSource.c_str();
	GLint shaderSourceLen = static_cast<GLint>(shaderSource.length());
	glShaderSource(shader, 1, &shaderSourceCStr, &shaderSourceLen);
	glCompileShader(shader);

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}
