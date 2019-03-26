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
	glm::vec3 init_position;
	glm::vec3 init_direction;
	glm::vec3 init_up;
	float camPitch,camYaw,distance;
	float movevel;
	float mousevel;
	bool mi,ismoved;
	bool rotating;
	bool zooming;
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
	void setPitch(float _pitch) { camPitch = _pitch; };
	float getYaw();
	void setYaw(float _yaw) { camYaw = _yaw; };
	float getMovevel();
	float getMousevel();
	void Rotating(bool _rot) { rotating = _rot; }
	bool Rotating(void) { return rotating; }
	bool isMouseIn();
	void SetDistance(float dist);
	float GetDistance(void) { return distance; }
	void Location(glm::vec3 vec);
	glm::vec3 Location(void);
	void Zooming(bool _zooming) { zooming = _zooming; }
	bool Zooming(void) { return zooming;  }
	void LookAt(float pitch,float yaw);
	void mouseIn(bool b);
	void setSpeed(float mv,float mov);
		
	bool isMoved();
};

typedef Camera::cameraPtr cameraPtr;

#endif
