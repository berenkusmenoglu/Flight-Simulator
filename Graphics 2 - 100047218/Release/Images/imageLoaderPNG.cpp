#include <iostream>
#include <windows.h>

#include "nvImage.h"

#include <fstream>
#include <string>
#include <exception>

#include <vector>
#include "imageLoader.h"

GLuint ImageLoader::loadPNG(const char* strFileName)
{
	GLuint myTexture;

	// Texture loading object
	nv::Image img;

	// Return true on success
	if(img.loadImageFromFile(strFileName))
	{
		glGenTextures(1, &myTexture);
		glBindTexture(GL_TEXTURE_2D, myTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	return myTexture;
}

