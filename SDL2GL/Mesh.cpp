#include "stdafx.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <GL/GLU.h>
#include "Utils.h"

Mesh::Mesh(std::vector<stVertexData> *vd, std::vector<unsigned int>*id, materialPtr material)
{
	SetMeshData(vd,id,material);
}

void Mesh::SetMeshData(std::vector<stVertexData> *vd, std::vector<unsigned int>*id)
{
	data = *vd;
	indices = *id;
	
	GLCall(glCreateBuffers(1, &VBO));
	GLCall(glNamedBufferStorage(VBO, data.size() * sizeof(stVertexData), data.data(), GL_DYNAMIC_STORAGE_BIT));

	GLCall(glCreateBuffers(1, &IND));
	GLCall(glNamedBufferStorage(IND, indices.size() * sizeof(unsigned int), indices.data(), 0));

	GLCall(glCreateVertexArrays(1, &VAO));
	GLCall(glVertexArrayElementBuffer(VAO, IND));

	// set up attribute 0 / vertex
	GLCall(glVertexArrayAttribFormat(VAO, 0u, 3, GL_FLOAT, GL_FALSE, 0u));
	GLCall(glVertexArrayVertexBuffer(VAO, 0u, VBO, 0, sizeof(stVertexData)));
	GLCall(glVertexArrayAttribBinding(VAO, 0u, 0u));
	GLCall(glEnableVertexArrayAttrib(VAO, 0u));

	// set up attribute 1 / normal
	GLCall(glVertexArrayAttribFormat(VAO, 1u, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)));
	GLCall(glVertexArrayVertexBuffer(VAO, 1u, VBO, 0, sizeof(stVertexData)));
	GLCall(glVertexArrayAttribBinding(VAO, 1u, 1u));
	GLCall(glEnableVertexArrayAttrib(VAO, 1u));

	// set up attribute 2 / tangent
	GLCall(glVertexArrayAttribFormat(VAO, 2u, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3)));
	GLCall(glVertexArrayVertexBuffer(VAO, 2u, VBO, 0, sizeof(stVertexData)));
	GLCall(glVertexArrayAttribBinding(VAO, 2u, 2u));
	GLCall(glEnableVertexArrayAttrib(VAO, 2u));

	// set up attribute 3 / color
	GLCall(glVertexArrayAttribFormat(VAO, 3u, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3)));
	GLCall(glVertexArrayVertexBuffer(VAO, 3u, VBO, 0, sizeof(stVertexData)));
	GLCall(glVertexArrayAttribBinding(VAO, 3u, 3u));
	GLCall(glEnableVertexArrayAttrib(VAO, 3u));

	// set up attribute 4 / uv
	GLCall(glVertexArrayAttribFormat(VAO, 4u, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec3)));
	GLCall(glVertexArrayVertexBuffer(VAO, 4u, VBO, 0, sizeof(stVertexData)));
	GLCall(glVertexArrayAttribBinding(VAO, 4u, 4u));
	GLCall(glEnableVertexArrayAttrib(VAO, 4u));
}

void Mesh::SetMeshData(std::vector<stVertexData> *vd, std::vector<unsigned int>*id, materialPtr material)
{
	m_material = material;
	SetMeshData(vd, id);
}

void Mesh::SetDisplayType(mesh_display md)
{
	display_type = md;
}

void Mesh::draw(unsigned int programId)
{
	GLCall(glUseProgram(programId));
	GLint text_diffuse_loc;
	GLuint id;
	switch (display_type)
	{
	case DIFFUSE_COLOR:

		break;
	case TEXTURED:
		text_diffuse_loc = glGetUniformLocation(programId, "texture_diffuse");
		id = (GLuint)m_material->m_texture_diffuse->GetID();
		GLCall(glUniform1i(text_diffuse_loc, 0));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, id));
		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));
		break;
	case RAW_TRIANGLES:
		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));
		break;
	case RAW_MESH:
	default:
		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));
		break;
	};
	
	//GLCall(glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0));

	GLCall(glBindVertexArray(0u));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IND);
}
