#pragma once

#include <iostream>
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

enum enMatrixMode {
	MODEL_MATRIX=0,
	VIEW_MATRIX,
	PROJECTION_MATRIX
};

class Matrices
{
public:
	Matrices();
	~Matrices();
	void loadIdentity();/// no transformations
	void loadIdentity2All();// clean all transformation in all matrices;
	void matrixMode(int i);
	void update(); // update matrix over changes 
	//transformations
	void translate(float _x, float _y, float _z);
	void scale(float x,float y, float z);
	void scale(float x);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	// projection
	void perspective(float angle, float aRatio, float near, float far);
	void ortho(float left, float right, float bottom, float top, float near, float far);
	
	//viewMatrix
	void SetViewMatrix(glm::mat4 _VM);

	void pushMatrix();
	void popMatrix();
	
	glm::mat4& getModelMatrix();
	glm::mat4& getModelViewMatrix();
	glm::mat4& getModelViewProjectionMatrix();
	glm::mat4& getProjectionMatrix();
	glm::mat3& getNormalMatrix();

	//GLSL
	void updateMatrices(unsigned int programId);
private:
	std::vector<glm::mat4> modelMatrix; // object position
	std::vector<glm::mat4> viewMatrix; // by the camera
	//modelviewMatrix = viewMatrix * modelvMatrix
	std::vector<glm::mat4> projectionMatrix; //
	//modelViewProjectionMatrix = ProjectionMatrix * viewMatrix*modelMatrix
	//std::vector<glm::mat4> modelMatrix;

	//glMatrixMode
	int currentMatrix;

	bool matricesReady;
	glm::mat4 modelViewMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat3 normalMatrix;

};

