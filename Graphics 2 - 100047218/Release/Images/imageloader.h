#ifndef LOADER_H
#define LOADER_H


#include <string>
//#include <GL/glext.h>
#include <gl/gl.h>
#include <gl/glu.h>

class ImageLoader
{
private:
	static void GLErrorReport();

public:
	static void loadExtensions();
	
	static GLuint loadPNG(const char* filename);

};

#endif LOADER_H