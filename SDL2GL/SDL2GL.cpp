// SDL2GL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include  "Application.h"

int main(int argc, char *argv[])
{
	Application::Instance(argc,argv);
	while (Application::Instance()->Run());
    return 0;
}

