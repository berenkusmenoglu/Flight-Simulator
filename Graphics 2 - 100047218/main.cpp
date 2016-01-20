/*
	An OpenGL Project by Beren Kusmenoglu
	3rd Year Graphics 2 Coursework
*/

#include <windows.h>	
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include <GL/glut.h>
#include "console.h"
#include "Structures\Vector3d.h"
#include "TextureHandler\TextureHandler.h"
#include "shaders/Shader.h"   
#include "GameObject.h"
#include "Game.h"
#include "FreeType.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "MatrixRoutines.h"
#include "Camera.h"
#include "RandomTerrainGenerator.h"
#include "FlightControl.h"
#include "Cube.h"
#include "Images\nvImage.h"
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include "Sphere.h"
#include "Rect.h"
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"

#define PLANE_SPEED 0.005

using namespace freetype;

// ==== OBJECTS ====

FlightControl flightControl(FlightControl::NORMAL);
Camera camera(glm::vec3(0));
ThreeDModel ground, radar, truck, airBridge, ac130, tower;
GameObject airplane;
Game myGame;
ConsoleWindow console;
Shader* airplaneShader;  ///shader object 
Shader colliderShader;  ///shader object 
OBJLoader objLoader;
Font myfont, myfont2, mytitlefont, mytitlefont2;
Font guiFont;

// ==== END OBJECTS ====

// ==== COLLISION ====

//Sphere radarSphere = Sphere(radarPosition, 10);
//Sphere truckSphere = Sphere(truckPosition, 10);
//Sphere airBridgeSphere = Sphere(airBridgePosition, 16);
//Sphere ac130Sphere = Sphere(ac130Position, 16);
//Sphere towerSpheres = Sphere(towerPosition, 32);
Sphere airplaneSpheres[3];

glm::vec3 towerPosition(-100, 10, -200);
glm::vec3 groundPosition(0, -10, -0);
glm::vec3 radarPosition(-100, 10, -100);
glm::vec3 truckPosition(100, 0, -100);
glm::vec3 airplaneSpawn(0, 0, 0);
glm::vec3 airBridgePosition(-100, 0, -150);
glm::vec3 ac130Position(100, 12, -200);

Rect radarRect = Rect(radarPosition, 8, 15, 8);
Rect truckRect = Rect(truckPosition, 10, 5, 20);
Rect airBridgeRect = Rect(glm::vec3(airBridgePosition.x + 5, airBridgePosition.y, airBridgePosition.z + 5), 10, 10, 15);
Rect ac130Rect[5];
Rect towerRects = Rect(towerPosition, 30, 120, 30);
Rect airplaneRects[3];

// ==== END COLLISION ====

// ==== VARIABLES ====

float timeInterval;
float gravity = 0.1f;

int desiredFrameRate = 120;
int cameraIndex = 0;
int screenWidth = 1080, screenHeight = 720;

bool keys[256];
bool debug = false;

// ====  END VARIABLES ====

// ==== MATRICES ====

enum MatrixType
{
	PROJECTION,
	CAMERA,
	MODELVIEW,
	NORMAL
};

GLuint matLocation;
glm::mat3 normalMatrix;
glm::mat4 cameraMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelViewMatrix;
glm::mat4 ObjectModelMatrix;
glm::mat4 GroundModelMatrix;

// ==== END MATRICES ====

//Material properties
float Material_Ambient[4] = { 0.1, 0.1, 0.1, 1.0 };
float Material_Diffuse[4] = { 0.8, 0.8, 0.8, 1.0 };
float Material_Specular[4] = { 0.9,0.9,0.8,1.0 };
float Material_Shininess = 50;

//Light Properties
float Light_Ambient_And_Diffuse[4] = { 0.6, 0.6, 0.6, 1.0 };
float Light_Specular[4] = { 1.0,1.0,1.0,1.0 };
float LightPos[4] = { 0.0, 1.0, 0.0, 0.0 };


//OPENGL FUNCTION PROTOTYPES

void display();				//called in winmain to draw everything to the screen
void reshape();				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void update();				//called in winmain to update variables
void restartGame();
void SetUpLighting(Shader* aShader);
void SetUpMatrix(MatrixType matrixType);
bool AnimateNextFrame(int desiredFrameRate);
//void spawnCollisionSphere(glm::vec3 pos, float dim, bool isPlane);
void drawSphere(Sphere sphere, bool isPlane);
void drawRectangle(Rect rect);
bool checkSphereSphereCollision(Sphere sp1, Sphere sp2);
bool checkSphereRectangleCollision(Sphere sp1, Rect sp2);



/*************    START OF OPENGL FUNCTIONS   ****************/
/*
 A method for setting up the ligting easier.
*/
void SetUpLighting(Shader* aShader)
{

	glUniform4fv(glGetUniformLocation(aShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(aShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(aShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(aShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(aShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(aShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(aShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(aShader->handle(), "material_shininess"), Material_Shininess);

}
/*
 A method for setting up the matrices easier.
*/
void SetUpMatrix(MatrixType matrixType)
{
	switch (matrixType)
	{
	case PROJECTION:

		matLocation = glGetUniformLocation(airplaneShader->handle(), "ProjectionMatrix");
		glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		break;
	case CAMERA:

		cameraMatrix = camera.UseCamera(cameraIndex);
		glUniformMatrix4fv(glGetUniformLocation(airplaneShader->handle(), "ViewMatrix"), 1, GL_FALSE, &cameraMatrix[0][0]);

		break;
	case MODELVIEW:

		glUniformMatrix4fv(glGetUniformLocation(airplaneShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

		break;

	case NORMAL:

		normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(airplaneShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

		break;
	default:
		break;
	}



}
/*
 Initialisation.
*/
void init()
{
	for (size_t j = 0; j < 5; j++)
	{

		ac130Rect[j] = Rect(glm::vec3(ac130Position.x, ac130Position.y-11, ac130Position.z - (j * 60)), 40, 5, 25);
		
	}

	glClearColor(0.5, 0.7, 0.85, 0.0);

	if (!colliderShader.load("BasicView", "glslfiles/Color.vert", "glslfiles/Color.frag"))
	{
		cout << "failed to load shader" << endl;
	}

	glEnable(GL_DEPTH_TEST);
	airplaneShader = new Shader;

	if (!airplaneShader->load("BasicView", "glslfiles/Shader.vert", "glslfiles/Shader.frag"))
	{
		cout << "failed to load shader" << endl;
	}


	glUseProgram(airplaneShader->handle());

	glEnable(GL_TEXTURE_2D);

	cout << " loading airplane.model " << endl;

	if (objLoader.loadModel("TestModels/myPlane.obj", airplane.model))
	{
		cout << " airplane.model loaded " << endl;

		airplane.model.calcCentrePoint();
		airplane.model.centreOnZero();
		airplane.model.calcVertNormalsUsingOctree();
		airplane.model.initDrawElements();
		airplane.model.initVBO(airplaneShader);
		airplane.model.deleteVertexFaceData();

	}
	else
	{
		cout << " airplane.model failed to load " << endl;
	}

	if (objLoader.loadModel("TestModels/box.obj", ground))
	{
		cout << " model loaded " << endl;

		ground.calcCentrePoint();
		ground.centreOnZero();
		ground.calcVertNormalsUsingOctree();
		ground.initDrawElements();
		ground.initVBO(airplaneShader);
		ground.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	if (objLoader.loadModel("TestModels/Radar.obj", radar))
	{
		cout << " model loaded " << endl;

		radar.calcCentrePoint();
		radar.centreOnZero();
		radar.calcVertNormalsUsingOctree();
		radar.initDrawElements();
		radar.initVBO(airplaneShader);
		radar.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	if (objLoader.loadModel("TestModels/myTruck.obj", truck))
	{
		cout << " model loaded " << endl;

		truck.calcCentrePoint();
		truck.centreOnZero();
		truck.calcVertNormalsUsingOctree();
		truck.initDrawElements();
		truck.initVBO(airplaneShader);
		truck.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	if (objLoader.loadModel("TestModels/AirBridge.obj", airBridge))
	{
		cout << " model loaded " << endl;

		airBridge.calcCentrePoint();
		airBridge.centreOnZero();
		airBridge.calcVertNormalsUsingOctree();
		airBridge.initDrawElements();
		airBridge.initVBO(airplaneShader);
		airBridge.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
	if (objLoader.loadModel("TestModels/ac130.obj", ac130))
	{
		cout << " model loaded " << endl;

		ac130.calcCentrePoint();
		ac130.centreOnZero();
		ac130.calcVertNormalsUsingOctree();
		ac130.initDrawElements();
		ac130.initVBO(airplaneShader);
		ac130.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	if (objLoader.loadModel("TestModels/Tower.obj", tower))
	{
		cout << " model loaded " << endl;

		tower.calcCentrePoint();
		tower.centreOnZero();
		tower.calcVertNormalsUsingOctree();
		tower.initDrawElements();
		tower.initVBO(airplaneShader);
		tower.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}

	////	FONT
	Font::loadShader("glslfiles/basicTexture");
	float colorYellow[] = { 1.0, 1.0, 0.0 };
	float colorRed[] = { 0.9, 0.1, 0.1 };
	float colorDarkRed[] = { 1, 0.20, 0.20 };
	float colorGrey[] = { 0.3, 0.3, 0.3 };
	float colorBlack[] = { 0, 0, 0 };

	myfont.init("fonts/MEgalopolisExtra.OTF", 36, colorRed);
	myfont2.init("fonts/MEgalopolisExtra.OTF", 36, colorDarkRed);

	mytitlefont.init("fonts/MEgalopolisExtra.OTF", 36, colorGrey);
	mytitlefont2.init("fonts/MEgalopolisExtra.OTF", 36, colorBlack);
	guiFont.init("fonts/MEgalopolisExtra.OTF", 15, colorYellow);

}
/*
 Drawing every frame.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Collision spheres on the airplane
	airplaneSpheres[0] = Sphere(glm::vec3(airplane.objectPosition.x, airplane.objectPosition.y, airplane.objectPosition.z), 1);
	airplaneSpheres[1] = Sphere(glm::vec3(airplane.objectPosition.x + 3, airplane.objectPosition.y + .5f, airplane.objectPosition.z - 2), 1);
	airplaneSpheres[2] = Sphere(glm::vec3(airplane.objectPosition.x - 3, airplane.objectPosition.y + .5f, airplane.objectPosition.z - 2), 1);

	if (debug)
	{
		for (size_t i = 0; i < 3; i++)
		{
			drawSphere(airplaneSpheres[i], true);
		}

		//drawSphere(radarSphere, false);
		//drawSphere(truckSphere, false);
		//drawSphere(airBridgeSphere, false);
		//drawSphere(ac130Sphere, false);
		//drawSphere(towerSpheres, false);

		// Collision rects for models
		for (size_t j = 0; j < 5; j++)
		{

			ac130Rect[j] = Rect(glm::vec3(ac130Position.x, ac130Position.y, ac130Position.z - (j * 60)), 40, 5, 25);
			drawRectangle(ac130Rect[j]);
		}

		drawRectangle(radarRect);
		drawRectangle(truckRect);
		drawRectangle(airBridgeRect);

		drawRectangle(towerRects);

	}

	glMatrixMode(GL_MODELVIEW);

	glUseProgram(airplaneShader->handle());  // use the shader

	SetUpMatrix(PROJECTION);
	SetUpMatrix(CAMERA);

	SetUpLighting(airplaneShader);

	//START SCREEN
	if (myGame.state == myGame.PAUSED)
	{
		//FONT
		glUseProgram(Font::myShader->handle());

		glm::mat4 twoDProjection;
		twoDProjection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);


		glUniformMatrix4fv(glGetUniformLocation(Font::myShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &twoDProjection[0][0]);

		print(mytitlefont2, 54, screenHeight / 2 - 5, "WELCOME TO BERENS FLIGHT SIMULATOR ");
		print(mytitlefont, 50, screenHeight / 2, "WELCOME TO BERENS FLIGHT SIMULATOR ");

		print(myfont, 254, 40 - 5, "Press Space to Start ");
		print(myfont2, 250, 40, "Press Space to Start ");

	}
	//GAMEOVER SCREEN
	else if (myGame.state == myGame.FINISHED)
	{
		//FONT
		glUseProgram(Font::myShader->handle());

		glm::mat4 twoDProjection;
		twoDProjection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);


		glUniformMatrix4fv(glGetUniformLocation(Font::myShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &twoDProjection[0][0]);

		print(mytitlefont2, 400, screenHeight / 2 - 5, "YOU CRASHED!\n GAME OVER");

		print(myfont, 304, 40 - 5, "Press R to Restart ");
		print(myfont2, 300, 40, "Press R to Restart ");
	}
	//GAME LOOP
	if (myGame.state == myGame.PLAYING)
	{

		ObjectModelMatrix = glm::mat4();

		ObjectModelMatrix = cameraMatrix;

		ObjectModelMatrix *= airplane.objectTransformationMatrix;

		ModelViewMatrix = ObjectModelMatrix;

		//MODEL

		SetUpMatrix(MODELVIEW);
		SetUpMatrix(NORMAL);

		airplane.model.drawElementsUsingVBO(airplaneShader);

		//GROUND

		GroundModelMatrix = glm::mat4();

		GroundModelMatrix = glm::translate(cameraMatrix, groundPosition);

		GroundModelMatrix = glm::scale(GroundModelMatrix, glm::vec3(200, 0.1f, 200));

		ModelViewMatrix = GroundModelMatrix;

		normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));


		SetUpMatrix(NORMAL);
		SetUpMatrix(MODELVIEW);


		ground.drawElementsUsingVBO(airplaneShader);

		/////////////////// RADAR

		ModelViewMatrix = glm::translate(cameraMatrix, radarPosition);


		SetUpMatrix(NORMAL);
		SetUpMatrix(MODELVIEW);
		radar.drawElementsUsingVBO(airplaneShader);


		/////// TRUCK

		ModelViewMatrix = glm::translate(cameraMatrix, truckPosition);
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.1, 0.1, 0.1));

		SetUpMatrix(NORMAL);
		SetUpMatrix(MODELVIEW);
		truck.drawElementsUsingVBO(airplaneShader);

		//////////// AIRBRIDGE


		ModelViewMatrix = glm::translate(cameraMatrix, airBridgePosition);
		ModelViewMatrix = glm::rotate(ModelViewMatrix, 150.0f, glm::vec3(0, 1, 0));

		SetUpMatrix(NORMAL);
		SetUpMatrix(MODELVIEW);
		airBridge.drawElementsUsingVBO(airplaneShader);



		//////////// AC130

		for (size_t i = 0; i < 5; i++)
		{
			ModelViewMatrix = glm::translate(cameraMatrix, glm::vec3(ac130Position.x, ac130Position.y, ac130Position.z - (i * 60)));
			ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(3.5, 3.5, 3.5));
			ModelViewMatrix = glm::rotate(ModelViewMatrix, 300.0f, glm::vec3(0, 1, 0));

			SetUpMatrix(NORMAL);
			SetUpMatrix(MODELVIEW);
			ac130.drawElementsUsingVBO(airplaneShader);


		}

		/////////// TOWER

		ModelViewMatrix = glm::translate(cameraMatrix, towerPosition);

		SetUpMatrix(NORMAL);
		SetUpMatrix(MODELVIEW);
		tower.drawElementsUsingVBO(airplaneShader);

		////////// GUI

		//FONT
		glUseProgram(Font::myShader->handle());

		glm::mat4 twoDProjection;
		twoDProjection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);


		glUniformMatrix4fv(glGetUniformLocation(Font::myShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &twoDProjection[0][0]);
		print(guiFont, 20, 50, "Speed = %7.2f", airplane.objectSpeed);
		print(guiFont, 20, 110, "Altitude = %7.2f", airplane.objectPosition.y + 5);

		if (debug)
		{
			print(guiFont, 20, 150, "transform.x = %7.2f", airplane.transform.x);
			print(guiFont, 20, 180, "transform.y = %7.2f", airplane.transform.z);
		}

		glFlush();


	}
}
/*
 A method for restarting the game.(Easier for demonstration)
*/
void restartGame()
{
	SetUpMatrix(PROJECTION);
	SetUpMatrix(CAMERA);
	SetUpLighting(airplaneShader);

	cameraMatrix = glm::mat4();
	ObjectModelMatrix = cameraMatrix;
	airplane.objectPosition = glm::vec3();
	airplane.transform = glm::quat();
	airplane.objectSpeed = 0;

	myGame.StartGame();

}
/*
 Processes key inputs.
*/
void processKeys()
{
	flightControl.updatePlaneState(airplane.objectPosition, groundPosition, airplane.objectSpeed, glm::vec3(airplane.transform.x, 0, airplane.transform.z));

	if (keys['0'])
	{
		cameraIndex = 0;
	}
	if (keys['1'])
	{
		cameraIndex = 1;
	}
	if (keys['E'])
	{
		cameraIndex = 2;
	}
	if (keys['3'])
	{
		cameraIndex = 3;
	}
	if (keys['4'])
	{
		cameraIndex = 4;
	}


	if (keys[VK_SPACE] && myGame.state == myGame.PAUSED)
	{
		if (myGame.state == myGame.PAUSED)
			myGame.ResumeGame();
	}

	if (keys[VK_DOWN])
	{

		if (flightControl.state == FlightControl::FLYING)
		{
			airplane.applyQuaternion(airplane.DOWN);
		}

		else if (flightControl.allowTakeOff(myGame))
			airplane.applyQuaternion(airplane.DOWN);

	}
	if (keys[VK_UP])
	{

		if (flightControl.state == FlightControl::FLYING)
		{
			airplane.applyQuaternion(airplane.UP);
		}

		else if (flightControl.allowRotation(myGame))
			airplane.applyQuaternion(airplane.UP);
	}
	if (keys[VK_LEFT])
	{


		if (flightControl.state == FlightControl::FLYING) {
			airplane.applyQuaternion(airplane.LEFT);

		}

		else if (flightControl.allowRotation(myGame))
			airplane.applyQuaternion(airplane.LEFT);
	}
	if (keys[VK_RIGHT])
	{


		if (flightControl.state == FlightControl::FLYING)
		{
			airplane.applyQuaternion(airplane.RIGHT);

		}

		else if (flightControl.allowRotation(myGame))
			airplane.applyQuaternion(airplane.RIGHT);
	}

	if (keys['W'])
	{
		if (airplane.objectSpeed < flightControl.maxObjectSpeed)
			airplane.objectSpeed += PLANE_SPEED;
	}


	if (keys['S'])
	{
		if (flightControl.state == FlightControl::FLYING)
		{
			if (airplane.objectSpeed > flightControl.minimumFlyingSpeed)
			{
				airplane.objectSpeed -= PLANE_SPEED;
			}

		}
		else if (flightControl.state == FlightControl::TAXYING)
		{
			if (airplane.objectSpeed > 0)
				airplane.objectSpeed -= 0.01;

	

		}

	}

	if (keys['A'])
	{
		airplane.applyQuaternion(airplane.TURNLEFT);
	}
	if (keys['D'])
	{
		airplane.applyQuaternion(airplane.TURNRIGHT);
	}


	if (flightControl.state == flightControl.CRASHED)
	{
		myGame.state == myGame.FINISHED;
	}

	if (keys['R'])
	{
		restartGame();
	}
	//UP
	if (keys['H'])
	{
		camera.cameraReferenceHeight += .1;
	}
	//DOWN
	if (keys['B'])
	{
		camera.cameraReferenceHeight -= .1;
	}
	//FRW
	if (keys['J'])
	{
		camera.cameraforwardVectorFactor += .2;
	}
	//BACK
	if (keys['N'])
	{
		camera.cameraforwardVectorFactor -= .2;
	}
	if (keys['K'])
	{
		camera.cameraHeight += .1;
	}
	if (keys['M'])
	{
		camera.cameraHeight -= .1;
	}

	if (keys['C'])
	{
		debug = !debug;
	}

	airplane.MoveObjectByQuaternion();


}
/*
 A method to update variables every frame.
*/
void update()
{
	if (myGame.state == myGame.PLAYING)
	{

		if ((airplane.objectPosition.y <= groundPosition.y + 5) && flightControl.state == flightControl.FLYING)

		{
			if (!flightControl.validLanding())
			{
				PlaySound(TEXT("crash.wav"), NULL, SND_SYNC);
				myGame.StopGame();

				//CRASH

			}


		}

		///////////

		if (myGame.PLAYING)
		{
			airplane.MoveObject(0, -gravity, 0);
		}

		if (airplane.objectPosition.y <= groundPosition.y + 5)
		{
			airplane.objectPosition.y = groundPosition.y + 5;

		}

		for (size_t i = 0; i < 3; i++)
		{
			/*if (checkSphereSphereCollision(airplaneSpheres[i], radarSphere) || checkSphereSphereCollision(airplaneSpheres[i], towerSpheres) || checkSphereSphereCollision(airplaneSpheres[i], truckSphere) || checkSphereSphereCollision(airplaneSpheres[i], ac130Sphere) || checkSphereSphereCollision(airplaneSpheres[i], airBridgeSphere))
			{
				PlaySound(TEXT("crash.wav"), NULL, SND_SYNC);
				myGame.StopGame();

			}*/
			for (size_t j = 0; j < 5; j++)
			{
				if (checkSphereRectangleCollision(airplaneSpheres[i], radarRect) || checkSphereRectangleCollision(airplaneSpheres[i], towerRects) || checkSphereRectangleCollision(airplaneSpheres[i], truckRect) || checkSphereRectangleCollision(airplaneSpheres[i], ac130Rect[j]) || checkSphereRectangleCollision(airplaneSpheres[i], airBridgeRect))
				{
					PlaySound(TEXT("crash.wav"), NULL, SND_SYNC);
					myGame.StopGame();
					break;
				}
			}
		}


		///////////

		camera.UpdateCameras(airplane.objectPosition, airplane.transform);


	}
}
/*
 Window reshaping method.
*/
void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;


	glViewport(0, 0, width, height);

	ProjectionMatrix = glm::perspective(70.0f, (GLfloat)width / (GLfloat)height, 0.01f, 1000.0f);
}
/*
 Frame rate controller method.
*/
bool AnimateNextFrame(int desiredFrameRate)
{


	static double lastTime = 0.0f;
	double elapsedTime = 0.0;

	// Get current time in seconds  (milliseconds * .001 = seconds)
	double currentTime = GetTickCount() * 0.001;

	// Get the elapsed time by subtracting the current time from the last time
	elapsedTime = currentTime - lastTime;

	// Check if the time since we last checked is over (1 second / framesPerSecond)
	if (elapsedTime > (1.0 / desiredFrameRate))
	{
		// Reset the last time
		lastTime = currentTime;

		// Return TRUE, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
}
/*
 Sphere to sphere collison detection method.
*/
bool checkSphereSphereCollision(Sphere sp1, Sphere sp2)
{
	bool collision = false;
	float dist = 0;

	dist = sqrt(((sp1.cx - sp2.cx) * (sp1.cx - sp2.cx)) + ((sp1.cy - sp2.cy) * (sp1.cy - sp2.cy)) + ((sp1.cz - sp2.cz) *(sp1.cz - sp2.cz)));

	collision = (dist <= sp2.r + sp1.r);

	return collision;
}
/*
 Sphere to Rectangle collison detection method.
*/
bool checkSphereRectangleCollision(Sphere sp1, Rect rect)
{

	float sphereXDistance = abs(sp1.cx - rect.cx);
	float sphereYDistance = abs(sp1.cy - rect.cy);
	float sphereZDistance = abs(sp1.cz - rect.cz);

	if (sphereXDistance >= (rect.width + sp1.r)) { return false; }
	if (sphereYDistance >= (rect.height + sp1.r)) { return false; }
	if (sphereZDistance >= (rect.depth + sp1.r)) { return false; }

	if (sphereXDistance < (rect.width)) { return true; }
	if (sphereYDistance < (rect.height)) { return true; }
	if (sphereZDistance < (rect.depth)) { return true; }

	float cornerDistance_sq = ((sphereXDistance - rect.width) * (sphereXDistance - rect.width)) +
		((sphereYDistance - rect.height) * (sphereYDistance - rect.height) +
			((sphereYDistance - rect.depth) * (sphereYDistance - rect.depth)));

	return (cornerDistance_sq < (sp1.r * sp1.r));

}
/*
 Drawing the collison spheres.(Debug)
*/
void drawSphere(Sphere sphere, bool isPlane)
{

	if (isPlane)
	{

		sphere.constructGeometry(&colliderShader, 16);

		glUseProgram(colliderShader.handle());

		GLuint matLocation = glGetUniformLocation(colliderShader.handle(), "ProjectionMatrix");
		glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		ModelViewMatrix = glm::mat4();

		ModelViewMatrix = cameraMatrix;

		ModelViewMatrix *= airplane.objectTransformationMatrix;

		ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-airplane.objectPosition.x, -airplane.objectPosition.y, -airplane.objectPosition.z));

		glUniformMatrix4fv(glGetUniformLocation(colliderShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(colliderShader.handle(), "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		sphere.render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
	else
	{

		sphere.constructGeometry(&colliderShader, 16);

		glUseProgram(colliderShader.handle());

		GLuint matLocation = glGetUniformLocation(colliderShader.handle(), "ProjectionMatrix");
		glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		ModelViewMatrix = glm::mat4();

		ModelViewMatrix = cameraMatrix;

		glUniformMatrix4fv(glGetUniformLocation(colliderShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(colliderShader.handle(), "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		sphere.render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

	}
}
/*
 Drawing the collison cuboids.(Debug)
*/
void drawRectangle(Rect rect)
{

	rect.constructGeometry(&colliderShader);

	glUseProgram(colliderShader.handle());

	GLuint matLocation = glGetUniformLocation(colliderShader.handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	ModelViewMatrix = glm::mat4();

	ModelViewMatrix = cameraMatrix;


	glUniformMatrix4fv(glGetUniformLocation(colliderShader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(colliderShader.handle(), "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	rect.render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}






/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop

	console.Open();


	// Create Our OpenGL Window
	if (!CreateGLWindow("Beren's Flight Simulator", screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (keys[VK_ESCAPE])
				done = true;

			//process keyboard
			if (AnimateNextFrame(desiredFrameRate))
			{
				processKeys();
				display();					// Draw The Scene
				update();
				SwapBuffers(hDC);				// update variables
			}
			else
			{
				Sleep(1);								// Let other processes work
			}

			// Swap Buffers (Double Buffering)
		}
	}

	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	break;

	///*case WM_LBUTTONDOWN:
	//{
	//	mouse_x = LOWORD(lParam);
	//	mouse_y = screenHeight - HIWORD(lParam);
	//	LeftPressed = true;
	//}*/
	//break;

	//case WM_LBUTTONUP:
	//{
	//	LeftPressed = false;
	//}
	//break;

	//case WM_MOUSEMOVE:
	//{
	//	mouse_x = LOWORD(lParam);
	//	mouse_y = screenHeight - HIWORD(lParam);
	//}
	//break;
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{

		keys[wParam] = true;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = false;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}
	break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/

bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context " << endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << GLVersionString << endl;

	//OpenGL 3.2 way of checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}



