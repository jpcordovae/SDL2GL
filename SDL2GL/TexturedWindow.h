#ifndef TEXTURED_WINDOW_H_
#define TEXTURED_WINDOW_H_

#include "Window.h"
#include "Utils.h"
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include <memory>

class TexturedWindow : protected Window
{
public:
	typedef std::shared_ptr<TexturedWindow> texturedWindowPtr;
	TexturedWindow();
	virtual ~TexturedWindow();
	void Draw(float _dTime);
	void Init();
private:
	scenePtr scene;
};

typedef TexturedWindow::texturedWindowPtr texturedWindowPtr;

#endif // TEXTURED_WINDOW_H_
