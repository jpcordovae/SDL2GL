// SDL2GL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include  "Application.h"

int main(int argc, char *argv[])
{
	try {
		Application::Instance(argc, argv);
		while (Application::Instance()->Run()) {};
	}
	catch (std::exception &e)
	{
		std::cout << "EXCEPTION: " << e.what() << std::endl;
	}

    return 0;
}

