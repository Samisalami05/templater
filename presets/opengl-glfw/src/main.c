#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define LOG_ERROR(type, msg) fprintf(stderr, "%s:%d - \e[91m%s Error\e[0m: %s\n", __FILE__, __LINE__, type, msg);

static const char* vertex_shader_text =
	"#version 330\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec3 pos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"	pos = aPos;\n"
	"}\n";
 
static const char* fragment_shader_text =
	"#version 330\n"
	"out vec4 frag_color;\n"
	"in vec3 pos;\n"
	"uniform float time;\n"
	"\n"
	"// Written by Sam Hocevar\n"
	"vec3 hsv_to_rgb(vec3 c) {"
	"	vec4 k = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
	"	vec3 p = abs(fract(c.xxx + k.xyz) * 6.0 - k.www);\n"
    "	return c.z * mix(k.xxx, clamp(p - k.xxx, 0.0, 1.0), c.y);\n"
	"}\n"
	"void main()\n"
	"{\n"
	"	vec3 color = hsv_to_rgb(vec3(fract(time / 10 + cos(pos.x) + sin(pos.y / 2)), 1.0, 1.0));\n"
	"	float v = pow((sin(time * 2 + cos(pos.x * 2) + sin(pos.y * 2 / 2)) + 1) / 2, 1.5);\n"
	"	frag_color = vec4(color * (v / 2 + 0.5), 1.0);\n"
	"}\n";

static float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

void error_callback(int error, const char* description)
{
	LOG_ERROR("GLFW", description);
}

void resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(void) {
	if (!glfwInit()) {
		LOG_ERROR("GLFW", "Failed to initialize GLFW\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(1280, 720, "window", NULL, NULL);
	if (window == NULL) {
		LOG_ERROR("GLFW", "Failed to create window\n");
		glfwTerminate();
		return 1;
	}

	glfwSetErrorCallback(error_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("GLAD", "Failed to load OpenGL\n");
		return 1;
	}

	int  success;
	char infoLog[512];

	const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		LOG_ERROR("Shader", infoLog);
	}

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		LOG_ERROR("Shader", infoLog);
	}
 
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glUseProgram(program);
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glUseProgram(program);
		int loc = glGetUniformLocation(program, "time");
		glUniform1f(loc, glfwGetTime());

        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
