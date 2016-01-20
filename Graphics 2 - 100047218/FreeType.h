#ifndef FREE_NEHE_H
#define FREE_NEHE_H


//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL Headers 
#include <windows.h>		//(the GL headers need it)
#include "gl/glew.h"
#include "gl/wglew.h"

//Some STL headers
#include <vector>
#include <string>

#pragma comment(lib, "libfreetype.lib")

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

#include "shaders/shader.h"

//MSVC will spit out all sorts of useless warnings if
//you create vectors of strings, this pragma gets rid of them.
#pragma warning(disable: 4786) 

///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
namespace freetype {

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
using std::vector;

//Ditto for string.
using std::string;

//This holds all of the information related to any
//freetype font that we want to create.  
class Font{
public:
	float h;			///< Holds the height of the font.
	GLuint * textures;	///< Holds the texture id's 
	unsigned int m_vaoID[128];
	unsigned int m_vboID[128][3];
	float widthChar[128];
	
	static Shader* myShader;  ///shader object just need one

	//The init function will create a font of
	//of the height h from the file fname.
	void init(const char * fname, unsigned int h, float color[3]);

	void make_dlist( FT_Face face, char ch, float color[3]);

	static void loadShader(string filename);
};


//The flagship function of the library - this thing will print
//out text at window coordinates x,y, using the font ft_font.
//The current modelview matrix will also be applied to the text. 
void print(const Font &ft_font, float x, float y, const char *fmt, ...) ;

}

#endif