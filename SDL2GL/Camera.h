//http://www.youtube.com/user/thecplusplusguy
#ifndef CAMERA_H
#define CAMERA_H
#define NOMINMAX
#include <Windows.h>
#include <cmath>
#include <iostream>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Matrices.h"
#include <memory>
#include <SDL.h>

class Camera{
private:
	glm::vec3 loc;
	float camPitch,camYaw;
	float movevel;
	float mousevel;
	bool mi,ismoved;
	void lockCamera();
	void moveCamera(float dir);
	void moveCameraUp(float dir);
	// for lookAt
	glm::vec3 to, up;
	float mNear, mFar;
	float mZoom;
public:
	typedef std::shared_ptr<Camera> cameraPtr;
	Camera();
	Camera(glm::vec3 loc);
	Camera(glm::vec3 loc,float yaw,float pitch);
	Camera(glm::vec3 loc,float yaw,float pitch,float mv,float mov);
	//void Control(Matrices& pipeline, SDL_Window *_window);
	//void UpdateCamera(Matrices& pipeline);
	void SetIsMoved(bool _isMoved);
	glm::vec3 getVector();
	glm::vec3 getLocation();
	glm::mat4 LookAt(glm::vec3 eye, glm::vec3 to = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f));
	float GetZoom(void);
	void SetZoom(float _zoom);
	float GetNear(void);
	void SetNear(float _far);
	float GetFar();
	void SetFar(float _far);
	float getPitch();
	float getYaw();
	float getMovevel();
	float getMousevel();
	bool isMouseIn();
		
	void setLocation(glm::vec3 vec);
	void lookAt(float pitch,float yaw);
	void mouseIn(bool b);
	void setSpeed(float mv,float mov);
		
	bool isMoved();
};

typedef Camera::cameraPtr cameraPtr;

#endif
