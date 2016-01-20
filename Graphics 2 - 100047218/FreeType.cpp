/*
	A quick and simple opengl font library that uses GNU freetype2, written
	and distributed as part of a tutorial for nehe.gamedev.net.
	Sven Olsen, 2003
*/
#include "gl/glew.h"
#include "gl/wglew.h"

#include <iostream>

//Include our header file.
#include "freetype.h"

#include "MatrixRoutines.h"



namespace freetype {
	Shader* Font::myShader = NULL;

///This function gets the first power of 2 >= the
///int that we pass it.
inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

///Create a display list coresponding to the given character.
void Font::make_dlist( FT_Face face, char ch, float color[3]) {

	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	//Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[ 4 * width * height]; //was 2

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			expanded_data[4*(i+j*width)]= expanded_data[4*(i+j*width)+1]=expanded_data[4*(i+j*width)+2]=expanded_data[4*(i+j*width)+3] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}


	//Now we just setup some texture paramaters.
    glBindTexture( GL_TEXTURE_2D, textures[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		  0, GL_RGBA, GL_UNSIGNED_BYTE, expanded_data );

	
	//With the texture created, we don't need to expanded data anymore
    delete [] expanded_data;

	float	x=(float)bitmap.width / (float)width,
			y=(float)bitmap.rows / (float)height;

	
	/********************** SOME OPENGL3.2 STUFF ****************/

	float vert[18];	// create a vertex array
	
	vert[0] =0; vert[1] =bitmap.rows; vert[2] =0.0; //x,y,z values for each vertex
	vert[3] =0; vert[4] =0; vert[5] =0.0;
	vert[6] = bitmap.width; vert[7] =bitmap.rows; vert[8]= 0.0;
	
	vert[ 9] = bitmap.width; vert[10] =bitmap.rows; vert[11]= 0.0;
	vert[12] = bitmap.width; vert[13] =0.0; vert[14]= 0.0;
	vert[15] = 0.0; vert[16] =0.0; vert[17]= 0.0;

	vert[0] += bitmap_glyph->left;
	vert[3] += bitmap_glyph->left;
	vert[6] += bitmap_glyph->left;
	vert[9] += bitmap_glyph->left;
	vert[12] += bitmap_glyph->left;
	vert[15] += bitmap_glyph->left;

	vert[1] += (bitmap_glyph->top-bitmap.rows);
	vert[4] += (bitmap_glyph->top-bitmap.rows);
	vert[7] += (bitmap_glyph->top-bitmap.rows);
	vert[10]+= (bitmap_glyph->top-bitmap.rows);
	vert[13]+= (bitmap_glyph->top-bitmap.rows);
	vert[16]+= (bitmap_glyph->top-bitmap.rows);

	float col[18];	// color array
	col[0] = color[0]; col[1] = color[1]; col[2] = color[2]; //r,g,b values for each vertex
	col[3] = color[0]; col[4] = color[1]; col[5] = color[2];
	col[6] = color[0]; col[7] = color[1]; col[8] = color[2];
	col[9] = color[0]; col[10] = color[1]; col[11] = color[2]; 
	col[12] = color[0]; col[13] = color[1]; col[14] = color[2];
	col[15] = color[0]; col[16] = color[1]; col[17] = color[2];

	float tex[12]; // tex coords

	tex[0] = 0.0;  tex[1] = 0.0; 
	tex[2] = 0.0;  tex[3] = y; 
	tex[4] = x;  tex[5] = 0.0; 

	tex[6] = x;  tex[7] = 0.0; 
	tex[8] = x;  tex[9] = y; 
	tex[10]= 0.0;  tex[11]= y; 

	glBindVertexArray(m_vaoID[ch]);

	glGenBuffers(3, &m_vboID[ch][0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[ch][0]);
    glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vert, GL_STATIC_DRAW);

	//get the location of the attribute in the shader - we are interested in colour and position
	glUseProgram(myShader->handle());
	GLint vertexLocation= glGetAttribLocation(myShader->handle(), "in_Position");
	GLint colorLocation= glGetAttribLocation(myShader->handle(), "in_Color");
	GLint texLocation= glGetAttribLocation(myShader->handle(), "in_TexCoord");
	glUniform1i(glGetUniformLocation(myShader->handle(), "DiffuseMap"), 0);

	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[ch][1]);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), col, GL_STATIC_DRAW);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[ch][2]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), tex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(texLocation);
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	/********************************************************/

	widthChar[ch] = face->glyph->advance.x >> 6;
}

void Font::loadShader(string filename)
{
	myShader = new Shader;

	string filenameVert = filename + ".vert";
	string filenameFrag = filename + ".frag";

	if(!myShader->load(filename, filenameVert.c_str(), filenameFrag.c_str()))
	{
		std::cout << "failed to load shader" << std::endl;
	}		
}

void Font::init(const char * fname, unsigned int h, float color[3]) {
	//Allocate some memory to store the texture ids.
	textures = new GLuint[128];

	glGenVertexArrays(128, &m_vaoID[0]);

	this->h=h;

	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face( library, fname, 0, &face )) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
	FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);

	//Here we ask opengl to allocate resources for
	//all the textures and displays lists which we
	//are about to create.  
	glGenTextures( 128, textures );

	//This is where we actually create each of the fonts display lists.
	for(unsigned char i=0;i<128;i++)
	{
		make_dlist(face,i, color);
	}

	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	FT_Done_Face(face);

	//Ditto for the library.
	FT_Done_FreeType(library);
}



///Much like Nehe's glPrint function, but modified to work
///with freetype fonts.
void print(const Font &ft_font, float x, float y, const char *fmt, ...)  {
	glDepthFunc(GL_ALWAYS); //ensure text is always displayed

	//GLuint font=ft_font.list_base;
	float h=ft_font.h/.63f;						//We make the height about 1.5* that of
	
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		*text=0;											// Do Nothing

	else {
	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text
	}


	//Here is some code to split the text that we have been
	//given into a set of lines.  
	//This could be made much neater by using
	//a regular expression library such as the one avliable from
	//boost.org (I've only done it out by hand to avoid complicating
	//this tutorial with unnecessary library dependencies).
	const char *start_line=text;
	vector<string> lines;
	const char*c = text;
	for(;*c;c++) {
		if(*c=='\n') {
			string line;
			for(const char *n=start_line;n<c;n++) line.append(1,*n);
			lines.push_back(line);
			start_line=c+1;
		}
	}
	if(start_line) {
		string line;
		for(const char *n=start_line;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	



	glUniform1i(glGetUniformLocation(ft_font.myShader->handle(), "DiffuseMap"), 0);

	for(int i=0;i<lines.size();i++) {
		
		float t[16];
		MatrixRoutines<float>::setTranslation(x,y-h*i, 0.0, t);

		const char* c = lines[i].c_str();
		
		for(int k=0; k<lines[i].length(); k++)
		{

			glUniformMatrix4fv(glGetUniformLocation(Font::myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, t);


			int ck = c[k];
			//std::cout << ck << " ";
			glBindTexture(GL_TEXTURE_2D,ft_font.textures[ck]);
			glBindVertexArray(ft_font.m_vaoID[ck]);
			glDrawArrays(GL_TRIANGLES, 0, 6);	// draw second object

			t[12] += ft_font.widthChar[c[k]];
		}
		

	}
	glDepthFunc(GL_LEQUAL);
}

}
