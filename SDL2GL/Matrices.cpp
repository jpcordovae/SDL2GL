#include "stdafx.h"
#include "Matrices.h"


Matrices::Matrices()
{
	modelMatrix.push_back(glm::mat4(1.0));
	viewMatrix.push_back(glm::mat4(1.0));
	projectionMatrix.push_back(glm::mat4(1.0));
	currentMatrix = MODEL_MATRIX;
	modelViewMatrix = glm::mat4(1.0);
	modelViewProjectionMatrix = glm::mat4(1.0);
	normalMatrix = glm::mat3(1.0);
	matricesReady = false;
}

Matrices::~Matrices()
{
}

void Matrices::loadIdentity()/// no transformations
{
	if (currentMatrix == MODEL_MATRIX || currentMatrix == VIEW_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] = glm::mat4(1.0);
		viewMatrix[viewMatrix.size() - 1] = glm::mat4(1.0);
	}
	else
	{
		projectionMatrix[projectionMatrix.size() - 1] = glm::mat4(1.0);
	}
	matricesReady = false;
}

void Matrices::loadIdentity2All()
{
	modelMatrix[modelMatrix.size() - 1] = glm::mat4(1.0);
	viewMatrix[viewMatrix.size() - 1] = glm::mat4(1.0);
	projectionMatrix[projectionMatrix.size() - 1] = glm::mat4(1.0);
	matricesReady = false;
}

void Matrices::matrixMode(int i)
{
	if (i == MODEL_MATRIX)
	{
		currentMatrix = MODEL_MATRIX;
	}
	else if (i == VIEW_MATRIX)
	{
		currentMatrix = VIEW_MATRIX;
	}
	else if (i == PROJECTION_MATRIX) 
	{
		currentMatrix = PROJECTION_MATRIX;
	}
}

//transformations
void Matrices::translate(float x, float y, float z)
{
	if (currentMatrix == MODEL_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] *= glm::translate(glm::vec3(x,y,z));
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		viewMatrix[viewMatrix.size() - 1] *= glm::translate(glm::vec3(-x,-y,-z));
	}
	matricesReady = false;
}

void Matrices::scale(float x, float y, float z)
{
	if (currentMatrix == MODEL_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] *= glm::scale(glm::vec3(x, y, z));
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		viewMatrix[viewMatrix.size() - 1] *= glm::scale(glm::vec3(-x, -y, -z));
	}
	matricesReady = false;
}

void Matrices::scale(float x)
{
	if (currentMatrix == MODEL_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] *= glm::scale(glm::vec3(x,x,x));
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		viewMatrix[viewMatrix.size() - 1] *= glm::scale(glm::vec3(-x,-x,-x));
	}
	matricesReady = false;
}

void Matrices::rotateX(float angle)
{
	if (currentMatrix == MODEL_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] *= glm::rotate(angle,glm::vec3(1.0f,0.0f,0.0f));
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		viewMatrix[viewMatrix.size() - 1] *= glm::rotate(-angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	matricesReady = false;
}

void Matrices::rotateY(float angle)
{
	if (currentMatrix == MODEL_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] *= glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		viewMatrix[viewMatrix.size() - 1] *= glm::rotate(-angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	matricesReady = false;
}

void Matrices::rotateZ(float angle)
{
	if (currentMatrix == MODEL_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] *= glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		viewMatrix[viewMatrix.size() - 1] *= glm::rotate(-angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	matricesReady = false;
}

// projection
void Matrices::perspective(float angle, float aRatio, float near, float far)
{
	projectionMatrix[projectionMatrix.size() - 1] = glm::perspective(angle,aRatio,near,far);
	matricesReady = false;
}

void Matrices::ortho(float left, float right, float bottom, float top, float near, float far)
{
	projectionMatrix[projectionMatrix.size() - 1] = glm::ortho(left,right,bottom,top,near,far);
	matricesReady = false;
}

void Matrices::SetViewMatrix(glm::mat4 _VM)
{
	viewMatrix[viewMatrix.size() - 1] = _VM;
	matricesReady = false;
}

void Matrices::pushMatrix()
{
	glm::mat4 mTmp;
	if (currentMatrix == MODEL_MATRIX)
	{
		mTmp = modelMatrix[modelMatrix.size() - 1];
		modelMatrix.push_back(mTmp);
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		mTmp = viewMatrix[viewMatrix.size() - 1];
		viewMatrix.push_back(mTmp);
	}
	else 
	{
		mTmp = projectionMatrix[projectionMatrix.size() - 1];
		projectionMatrix.push_back(mTmp);
	}
}

void Matrices::popMatrix()
{
	if (currentMatrix == MODEL_MATRIX)
	{
		if(modelMatrix.size()>1) modelMatrix.pop_back();
	}
	else if (currentMatrix == VIEW_MATRIX)
	{
		if (viewMatrix.size() > 1)	viewMatrix.pop_back();
	}
	else
	{
		if (projectionMatrix.size() > 1) projectionMatrix.pop_back();
	}
}

glm::mat4& Matrices::getModelMatrix()
{
	return modelMatrix[modelMatrix.size()-1];
}

glm::mat4& Matrices::getModelViewMatrix()
{
	if (!matricesReady) 
	{
		modelViewMatrix = viewMatrix[viewMatrix.size()-1] * modelMatrix[modelMatrix.size()-1];
	}
	return modelViewMatrix;
}

glm::mat4& Matrices::getModelViewProjectionMatrix()
{
	if (!matricesReady)
	{
		modelViewProjectionMatrix = projectionMatrix[projectionMatrix.size()-1] * viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
	}
	return modelViewProjectionMatrix;
}

glm::mat4& Matrices::getProjectionMatrix()
{
	return projectionMatrix[projectionMatrix.size() - 1];
}

glm::mat3& Matrices::getNormalMatrix()
{
	if (!matricesReady)
	{
		normalMatrix = glm::mat3(getModelViewMatrix());
	}
	return normalMatrix;
}

// GLSL 
void Matrices::updateMatrices(unsigned int programId)
{
	/*if (!matricesReady)
	{*/
		modelViewMatrix = viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
		modelViewProjectionMatrix = projectionMatrix[projectionMatrix.size() - 1] * viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
		normalMatrix = glm::mat3(getModelViewMatrix());
		matricesReady = true;
	//}

	glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"),     1, GL_FALSE, &modelMatrix[modelMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"),      1, GL_FALSE, &viewMatrix[viewMatrix.size() - 1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"),1, GL_FALSE, &projectionMatrix[projectionMatrix.size() - 1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewProjectionMatrix"),1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "normalMatrix"),	   1, GL_FALSE, &normalMatrix[0][0]);

}

