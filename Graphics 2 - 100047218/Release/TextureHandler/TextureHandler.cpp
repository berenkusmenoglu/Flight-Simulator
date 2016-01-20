#include "../gl/glew.h"
#include "TextureHandler.h"
#include <iostream>

std::map<std::string, int> TextureHandler::theTextureMap;	
int TextureHandler::boundTextures[4];

int TextureHandler::loadTexture(const char *fileName, bool buildMipMaps)
{
	int textureID;

	try
	{
		const char * resultPNG = strstr(fileName,".png");
		
		if(resultPNG != NULL)
		{
			textureID = ImageLoader::loadPNG(fileName);
		}
		else
		{
			std::cout << " unsupported texture format for " << fileName << std::endl;
			std::cout << " loader supports: png " << std::endl;
		}

		theTextureMap.insert(std::map<std::string,int>::value_type(std::string(fileName), textureID)); // put it into the map
	}
	catch (...)
	{
		std::cout << "Could not open " << fileName << std::endl;
		textureID = -1;
	} 

	return textureID;
}

bool TextureHandler::searchMap(const char *fileName, int &result)
{
	std::map<std::string,int>::iterator locTexture;
	
	locTexture = theTextureMap.find(fileName);

	if (locTexture != theTextureMap.end())
	{
		result = (*locTexture).second;
		return true;
	}
	else
	{
		return false;	
	}
}

int TextureHandler::lookUpTexture(string fileName, bool buildMipMaps)
{
	int textureID;

	int tempRes;
	if (searchMap(fileName.c_str(), tempRes)) 
	{
		textureID = tempRes;
	}
	else
	{
		textureID = loadTexture(fileName.c_str(), buildMipMaps);
	}
	
	return textureID;
}

void TextureHandler::destroyAllTextures()
{
	std::map<std::string,int>::iterator locTexture;

	for (locTexture = theTextureMap.begin(); locTexture != theTextureMap.end(); locTexture++)
	{
		int currentID = (*locTexture).second;
		glDeleteTextures(1, (GLuint * ) &currentID);
	}
}

string TextureHandler::reverseLookupTexture(int textID)
{
	std::map<std::string,int>::iterator locTexture;

	for (locTexture = theTextureMap.begin(); locTexture != theTextureMap.end(); locTexture++)
	{
		if (textID == (*locTexture).second)
		{
			return (*locTexture).first;			
		}
	}

	return "unkown";
}

void TextureHandler::clearAllBoundTextures()
{
	boundTextures[0] = -1;
	boundTextures[1] = -1;
	boundTextures[2] = -1;
	boundTextures[3] = -1;
}

void TextureHandler::clearBoundTextures(int textUnit)
{
	boundTextures[textUnit] = -1;
}

void TextureHandler::bindTexture(int textUnit, int texture)
{
	//if (boundTextures[textUnit] != texture)
	{	
		if (textUnit == 0)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			
		}
		else
		{
			glActiveTexture(GL_TEXTURE0+textUnit);
			glBindTexture(GL_TEXTURE_2D, texture);
			glActiveTexture(GL_TEXTURE0); 
		}
	
		boundTextures[textUnit] = texture;
	}

	
}

