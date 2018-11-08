#include "temporal.h"
#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint vs, fs, program;

int LoadShaders()
{
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	int length = strlen(vertex_shader);
	glShaderSource(vs, 1, (const GLchar **)&vertex_shader, &length);
	glCompileShader(vs);

	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		fprintf(stderr, "vertex shader compilation failed\n");
		return 1;
	}

	length = strlen(fragment_shader);
	glShaderSource(fs, 1, (const GLchar **)&fragment_shader, &length);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		fprintf(stderr, "fragment shader compilation failed\n");
		return 1;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glBindAttribLocation(program, attrib_position, "i_position");
	glBindAttribLocation(program, attrib_color, "i_color");
	glLinkProgram(program);

}

int temporalDraw(int width, int height)
{
	glUseProgram(program);

	//glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(attrib_position);
	glEnableVertexAttribArray(attrib_color);

	glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(attrib_position, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(4 * sizeof(float)));

	const GLfloat g_vertex_buffer_data[] = {
		/*  R, G, B, A, X, Y  */
			1, 0, 0, 1, 0, 0,
			0, 1, 0, 1, width, 0,
			0, 0, 1, 1, width, height,

			1, 0, 0, 1, 0, 0,
			0, 0, 1, 1, width, height,
			1, 1, 1, 1, 0, height
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glm::mat4 projection_matrix = glm::identity<glm::mat4>();
	//projection_matrix =  glm::translate(glm::identity<glm::mat4>(),glm::vec3(-1.0f,-1.0f,0.0f));
	//projection_matrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glUniformMatrix4fv(glGetUniformLocation(program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glBindVertexArray(vao);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	return 1;
}