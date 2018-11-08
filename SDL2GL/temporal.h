#pragma once

static const char * vertex_shader =
"#version 130\n"
"in vec2 i_position;\n"
"in vec4 i_color;\n"
"out vec4 v_color;\n"
"uniform mat4 u_projection_matrix;\n"
"void main() {\n"
"    v_color = i_color;\n"
"    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );\n"
"}\n";

static const char * fragment_shader =
"#version 130\n"
"in vec4 v_color;\n"
"out vec4 o_color;\n"
"void main() {\n"
"    o_color = v_color;\n"
"}\n";

typedef enum t_attrib_id
{
	attrib_position,
	attrib_color
} t_attrib_id;

int LoadShaders();
int temporalDraw(int width, int height);
