#include "camera.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include "Utils.h"

void Camera::lockCamera()
{
	if(camPitch>90)
		camPitch=90;
	if(camPitch<-90)
		camPitch=-90;
	if(camYaw<0.0)
		camYaw+=360.0;
	if(camYaw>360.0)
		camYaw-=360;
}

void Camera::moveCamera(float dir)
{
	float rad=(camYaw+dir)*glm::pi<float>()/180.0f;
	loc.x-=sin(rad)*movevel;
	loc.z-=cos(rad)*movevel;
}

void Camera::moveCameraUp(float dir)
{
	float rad=(camPitch+dir)*glm::pi<float>()/180.0f;
	loc.y+=sin(rad)*movevel;	
}

Camera::Camera()
{
	camPitch=0.0f;
	camYaw=0.0f;
	movevel=0.2f;
	mousevel=0.2f;
	//mi=ismoved=false;
	ismoved = false;
	mZoom = 45.0f;
	mNear = 0.1f;
	mFar = 1000.0f;
	rotating = false;
	zooming = false;
}

Camera::Camera(glm::vec3 l)
{
	loc = l;
	camPitch=0;
	camYaw=0;
	movevel=0.2f;
	mousevel=0.2f;
	//mi=ismoved=false;
	ismoved = false;
	mZoom = 45.0f;
	mNear = 0.1f;
	mFar = 1000.0f;
	rotating = false;
	zooming = false;
}

Camera::Camera(glm::vec3 l,float yaw,float pitch)
{
	loc = l;
	camPitch=pitch;
	camYaw=yaw;
	movevel=0.2f;
	mousevel=0.2f;
	//mi=ismoved=false;
	ismoved = false;
	mZoom = 45.0f;
	mNear = 0.1f;
	mFar = 1000.0f;
	rotating = false;
	zooming = false;
}

Camera::Camera(glm::vec3 l,float yaw,float pitch,float mv,float mov)
{
	loc = l;
	camPitch=pitch;
	camYaw=yaw;
	movevel=mv;
	mousevel=mov;
	//mi=false;
	ismoved = false;
	mZoom = 45.0f;
	mNear = 0.1f;
	mFar = 1000.0f;
	rotating = false;
	zooming = false;
}

void Camera::SetIsMoved(bool _isMoved)
{
	ismoved = _isMoved;
}

/*void Camera::Control(Matrices& pipeline, SDL_Window *_window)
{
	pipeline.matrixMode(VIEW_MATRIX);
	pipeline.rotateX(camPitch);
	pipeline.rotateY(camYaw);
}*/

/*void Camera::UpdateCamera(Matrices& pipeline)
{
	pipeline.matrixMode(VIEW_MATRIX);
	pipeline.translate(loc.x,loc.y,loc.z);
}*/

//change the spherical coordinate system to cartesian
glm::vec3 Camera::getVector()
{
	//Google->spherical to cartesian
 	return glm::vec3(	-cos(camPitch*M_PI/180.0)*sin(camYaw*M_PI/180.0),
						sin(camPitch*M_PI/180.0),
						-cos(camPitch*M_PI/180.0)*cos(camYaw*M_PI/180.0))*distance;
}

glm::vec3 Camera::getLocation()
{
	return getVector();
	//return loc;
}

glm::mat4 Camera::LookAt(glm::vec3 _eye, glm::vec3 _to, glm::vec3 _up)
{
	loc = _eye;
	to = _to;
	up = _up;
	return glm::lookAt(loc, to, up);
}

float Camera::GetZoom(void)
{
	return mZoom;
}

void Camera::SetZoom(float _zoom)
{
	mZoom = _zoom;
}

float Camera::GetNear()
{
	return mNear;
}

void Camera::SetNear(float _near)
{
	mNear = _near;
}

float Camera::GetFar()
{
	return mFar;
}

void Camera::SetFar(float _far)
{
	mFar = _far;
}

float Camera::getPitch()
{
	return camPitch;
}

float Camera::getYaw()
{
	return camYaw;
}
float Camera::getMovevel()
{
	return movevel;
}
float Camera::getMousevel()
{
	return mousevel;
}

bool Camera::isMouseIn()
{
	return mi;
}
		
void Camera::Location(glm::vec3 vec)
{
	this->distance = glm::length(vec);
	loc = vec;
}

void Camera::LookAt(float pitch,float yaw)
{
	camPitch=pitch;
	camYaw=yaw;
}

void Camera::mouseIn(bool b)
{
	mi=b;
}

void Camera::setSpeed(float mv,float mov)
{
	movevel=mv;
	mousevel=mov;
}

bool Camera::isMoved()
{
	return ismoved;
}

void Camera::SetDistance(float dist)
{
	this->distance = dist;
	loc = glm::normalize(loc)*dist;
}
