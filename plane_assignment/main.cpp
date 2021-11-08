//!Includes
#include <GL/glew.h> //OpenGL Related Functions
#include <GL/glut.h> //OpenGL Related Functions
#include <Shader.h>	 //Functions for compiling shaders
#include <Vector.h>	 //Functions for Vectors
#include <iostream>	 //Writing to the Console using std::cout
#include <math.h>	 // Includes Math functions
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h> // Texture Class
#include <string>
#include <Map.h>
#include <Object.h>

//!Function Prototypes
bool initGL(int argc, char **argv);				 //Function to init OpenGL
void initShader();								 //Function to init Shader
void initGeometry();							 //Function to init Geometry
void drawGeometry();							 //Function to draw Geometry
void display(void);								 //OpenGL Display Function
void keyboard(unsigned char key, int x, int y);	 //Keyboard Function
void mouse(int button, int state, int x, int y); //Mouse Function
void motion(int x, int y);						 //Mouse Motion Function
void initTexture(std::string filename, GLuint &textureID);
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void motion(int x, int y);
void Restart();
void Timer(int value);
void NextLevel(std::string levelFile);

//Shaders
GLuint shaderProgramID;			// Shader Program ID
GLuint skyboxShaderID;

// Skybox Vertex Attributes
GLuint SkyboxVertexPositionAttribute; // Vertex Position Attribute Location
GLuint SkyboxVertexNormalAttribute;
GLuint SkyboxVertexTexcoordAttribute; // Vertex Texcoord Attribute Location

// Vertex Attributes
GLuint vertexPositionAttribute; // Vertex Position Attribute Location
GLuint vertexNormalAttribute;
GLuint vertexTexcoordAttribute; // Vertex Texcoord Attribute Location

// Light Locations
GLuint LightPositionUniformLocation; // Light Position Uniform
GLuint AmbientUniformLocation;
GLuint SpecularUniformLocation;
GLuint SpecularPowerUniformLocation;

// Skybox Light Locations
GLuint SkyboxLightPositionUniformLocation; // Light Position Uniform
GLuint SkyboxAmbientUniformLocation;
GLuint SkyboxSpecularUniformLocation;
GLuint SkyboxSpecularPowerUniformLocation;

// Matrix Uniforms
GLuint MVMatrixUniformLocation;	  // ModelView Matrix Uniform
GLuint ViewMatrixUniformLoctaion;
GLuint ProjectionUniformLocation; // Projection Matrix Uniform Location

// Skybox Matrix Uniforms
GLuint SkyboxMVMatrixUniformLocation;	  // ModelView Matrix Uniform
GLuint SkyboxViewMatrixUniformLoctaion;
GLuint SkyboxProjectionUniformLocation;

// Texture Map Uniforms
GLuint TextureMapUniformLocation; // Texture Map Location
GLuint SkyboxTextureMapUniformLocation; // Skybox Texture Map Location

//Textures
GLuint planeTexture;			  // OpenGL Texture
GLuint terrainTexture;
GLuint coinTexture;
GLuint loopTexture;
GLuint cubemapTexture;
GLuint skyboxTexture[6];

//Model, View, Projection Matrix
Matrix4x4 ModelViewMatrix;			// Model Matrix
Matrix4x4 ViewMatrix;			  	// View Matrix
Matrix4x4 ProjectionMatrix;		  	// Projection Matrix

//Transformation and Rotation Matrix
Matrix4x4 planeDirection;			// Transformation Matrix
Matrix4x4 planeRotation;			// Rotation Matrix

//Mesh
Mesh plane;
Mesh terrain;
Mesh loop;
Mesh coin;
Mesh skybox[6];

Vector3f camera;
Vector3f Up(0, 1, 0);
Vector3f Rotation;
Vector3f planeDirectionVector3f;
Vector3f lightPosition = Vector3f(20.0, 20.0, 20.0); // Light Position
Vector3f ambient = Vector3f(0.1, 0.1, 0.1);
Vector3f SkyboxAmbient = Vector3f(0.2, 0.2, 0.2);
Vector3f specular = Vector3f(1.0, 1.0, 1.0);

int screenWidth = 720; 				// Screen size
int screenHeight = 720;
int screenPosX = 200;	 			// Screen Position
int screenPosY = 200;
int score = 0;
// int levels = 2;

bool keyStates[256]; // Array of key states
bool start = false;
bool set = false;
bool thirdPerson = true;
bool rotate = false;
bool yaw = false;
bool pitch = false;
bool roll = false;

float speed = 0;
float angle = 0;
float coinRotation;
float old_time = 0;
float new_time = 0;
float time_change = 0;
float specularPower = 10.0;

std::vector<bool> scored;
std::vector<int> collected;
std::vector<std::string> levels;

Map map;
Object planeObj;

//! Main Program Entry
int main(int argc, char *argv[])
{

	//init OpenGL
	if (!initGL(0, nullptr))
	{
		return -1;
	}

	//Init Key States to false;
	for (int i = 0; i < 256; i++)
	{
		keyStates[i] = false;
	}

	initShader();

	planeObj.Initiate("../models/plane1.obj","../models/plane1.bmp");

	terrain.initQuad();
	initTexture("../models/grass.bmp", terrainTexture);

	loop.loadOBJ("../models/torus.obj");

	coin.loadOBJ("../models/coin.obj");
	initTexture("../models/coin.bmp", coinTexture);

	for(int i = 0; i < 6; i++) {
		skybox[i].initQuad();
	}

	initTexture("../models/right.bmp", skyboxTexture[0]);
	initTexture("../models/left.bmp", skyboxTexture[1]);
	initTexture("../models/top.bmp", skyboxTexture[2]);
	initTexture("../models/bottom.bmp", skyboxTexture[3]);
	initTexture("../models/front.bmp", skyboxTexture[4]);
	initTexture("../models/back.bmp", skyboxTexture[5]);

	map.generateMap(argv[1]);

	for(int i = 2; i < argc; i++) {
		levels.push_back(argv[i]);
	}

	// std::cout << argv[1] << std::endl;

	//Enter main loop
	glutMainLoop();

	//Clean-Up
	glDeleteProgram(shaderProgramID);

	// MatrixGen(5,5);
	return 0;
}

//! Function to Initialise OpenGL
bool initGL(int argc, char **argv)
{

	//Init GLUT
	glutInit(&argc, argv);

	//Set Display Mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	//Set Window Size
	glutInitWindowSize(screenWidth, screenHeight);

	// Window Position
	glutInitWindowPosition(screenPosX, screenPosY);

	//Create Window
	glutCreateWindow("Plane Assignment");

	// Init GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	//Set Display function
	glutDisplayFunc(display);

	//Set Keyboard Interaction Functions
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp);

	//Start start timer function after 100 milliseconds
	glutTimerFunc(100, Timer, 0);

	//Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Enable Textures
	glEnable(GL_TEXTURE_2D);

	return true;
}

//Init Shader
void initShader()
{
	//Create shader
	shaderProgramID = Shader::LoadFromFile("shader.vert", "shader.frag");
	skyboxShaderID = Shader::LoadFromFile("skyboxShader.vert", "skyboxShader.frag");

	// Get a handle for our vertex position buffer
	vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");
	vertexNormalAttribute = glGetAttribLocation(shaderProgramID, "aVertexNormal");
	vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID, "aVertexTexcoord");

	SkyboxVertexPositionAttribute = glGetAttribLocation(skyboxShaderID, "aVertexPosition");
	SkyboxVertexNormalAttribute = glGetAttribLocation(skyboxShaderID, "aVertexNormal");
	SkyboxVertexTexcoordAttribute = glGetAttribLocation(skyboxShaderID, "aVertexTexcoord");

	SkyboxMVMatrixUniformLocation = glGetUniformLocation(skyboxShaderID, "MVMatrix_uniform");
	SkyboxViewMatrixUniformLoctaion = glGetUniformLocation(skyboxShaderID, "ViewMatrix_uniform");
	SkyboxProjectionUniformLocation = glGetUniformLocation(skyboxShaderID, "ProjMatrix_uniform");
	SkyboxLightPositionUniformLocation = glGetUniformLocation(skyboxShaderID, "LightPosition_uniform");
	SkyboxAmbientUniformLocation = glGetUniformLocation(skyboxShaderID, "Ambient_uniform");
	SkyboxSpecularUniformLocation = glGetUniformLocation(skyboxShaderID, "Specular_uniform");
	SkyboxSpecularPowerUniformLocation = glGetUniformLocation(skyboxShaderID, "SpecularPower_uniform");
	SkyboxTextureMapUniformLocation = glGetUniformLocation(skyboxShaderID, "TextureMap_uniform");

	//!
	MVMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "MVMatrix_uniform");
	ViewMatrixUniformLoctaion = glGetUniformLocation(shaderProgramID, "ViewMatrix_uniform");
	ProjectionUniformLocation = glGetUniformLocation(shaderProgramID, "ProjMatrix_uniform");
	LightPositionUniformLocation = glGetUniformLocation(shaderProgramID, "LightPosition_uniform");
	AmbientUniformLocation = glGetUniformLocation(shaderProgramID, "Ambient_uniform");
	SpecularUniformLocation = glGetUniformLocation(shaderProgramID, "Specular_uniform");
	SpecularPowerUniformLocation = glGetUniformLocation(shaderProgramID, "SpecularPower_uniform");
	TextureMapUniformLocation = glGetUniformLocation(shaderProgramID, "TextureMap_uniform");
}

void initTexture(std::string filename, GLuint &textureID)
{
	//Generate texture and bind
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Get texture Data
	int width, height;
	char *data;
	Texture::LoadBMP(filename, width, height, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Cleanup data - copied to GPU
	delete[] data;
}

void display(void) {
	Matrix4x4 m;

	old_time = new_time;
	new_time = glutGet(GLUT_ELAPSED_TIME);
	time_change =new_time - old_time;

	if (!yaw && !pitch && !roll)
	{
		planeRotation.toIdentity();
	}

	planeDirection = planeDirection * planeRotation;

	//Handle keys
	handleKeys();

	//Set Viewport
	glViewport(0, 0, screenWidth, screenHeight);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use shader
	glUseProgram(shaderProgramID);

	glUniform3f(LightPositionUniformLocation, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
	glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
	glUniform1f(SpecularPowerUniformLocation, specularPower);

	//Projection Matrix - Perspective Projection
	ProjectionMatrix.perspective(90, 1.0, 0.0001, 5000.0);

	//Set Projection Matrix
	glUniformMatrix4fv(
		ProjectionUniformLocation,	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose Matrix
		ProjectionMatrix.getPtr()); //Pointer to ModelViewMatrixValues

	if(start) {
		glDepthMask(GL_FALSE);

		glUniformMatrix4fv(
			ViewMatrixUniformLoctaion,	//Uniform location
			1,							//Number of Uniforms
			false,						//Transpose Matrix
			ViewMatrix.getPtr()); //Pointer to ModelViewMatrixValues

		Matrix4x4 skyboxMatrix[6];

		int const skyboxSize = 1023;

		//right
		skyboxMatrix[0].translate(planeObj.GetMatrix().getVector3f().x - skyboxSize/2, planeObj.GetMatrix().getVector3f().y - skyboxSize/2, planeObj.GetMatrix().getVector3f().z + skyboxSize/2);
		skyboxMatrix[0].scale(skyboxSize,skyboxSize,skyboxSize);
		//left
		skyboxMatrix[1].translate(planeObj.GetMatrix().getVector3f().x - skyboxSize/2, planeObj.GetMatrix().getVector3f().y - skyboxSize/2, planeObj.GetMatrix().getVector3f().z + skyboxSize/2);
		skyboxMatrix[1].scale(skyboxSize,skyboxSize,skyboxSize);
		skyboxMatrix[1].translate(0,0,-0.998);
		//top
		skyboxMatrix[2].translate(planeObj.GetMatrix().getVector3f().x - skyboxSize/2, planeObj.GetMatrix().getVector3f().y - skyboxSize/2, planeObj.GetMatrix().getVector3f().z + skyboxSize/2);
		skyboxMatrix[2].scale(skyboxSize,skyboxSize,skyboxSize);
		skyboxMatrix[2].rotate(-90,1,0,0);
		skyboxMatrix[2].translate(0,0,0.998);
		//bottom
		skyboxMatrix[3].translate(planeObj.GetMatrix().getVector3f().x - skyboxSize/2, planeObj.GetMatrix().getVector3f().y - skyboxSize/2, planeObj.GetMatrix().getVector3f().z + skyboxSize/2);
		skyboxMatrix[3].scale(skyboxSize,skyboxSize,skyboxSize);
		skyboxMatrix[3].rotate(-90,0.998,0,0);
		//front
		skyboxMatrix[4].translate(planeObj.GetMatrix().getVector3f().x - skyboxSize/2, planeObj.GetMatrix().getVector3f().y - skyboxSize/2, planeObj.GetMatrix().getVector3f().z + skyboxSize/2);
		skyboxMatrix[4].scale(skyboxSize,skyboxSize,skyboxSize);
		skyboxMatrix[4].rotate(90,0,1,0);
		skyboxMatrix[4].translate(0,0,0.998);
		//back
		skyboxMatrix[5].translate(planeObj.GetMatrix().getVector3f().x - skyboxSize/2, planeObj.GetMatrix().getVector3f().y - skyboxSize/2, planeObj.GetMatrix().getVector3f().z + skyboxSize/2);
		skyboxMatrix[5].scale(skyboxSize,skyboxSize,skyboxSize);
		skyboxMatrix[5].rotate(90,0,0.998,0);

		glUseProgram(skyboxShaderID);

		glUniform3f(SkyboxLightPositionUniformLocation, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform4f(SkyboxAmbientUniformLocation, SkyboxAmbient.x, SkyboxAmbient.y, SkyboxAmbient.z, 1.0);
		glUniform1f(SkyboxSpecularPowerUniformLocation, specularPower);

		glUniformMatrix4fv(
			SkyboxProjectionUniformLocation,	//Uniform location
			1,							//Number of Uniforms
			false,						//Transpose Matrix
			ProjectionMatrix.getPtr()); //Pointer to ModelViewMatrixValues

		glUniformMatrix4fv(
			SkyboxViewMatrixUniformLoctaion,	//Uniform location
			1,							//Number of Uniforms
			false,						//Transpose Matrix
			ViewMatrix.getPtr()); //Pointer to ModelViewMatrixValues


		for(int i = 0; i < 6; i++) {
			glUniformMatrix4fv(
				SkyboxMVMatrixUniformLocation,	//Uniform location
				1,							//Number of Uniforms
				false,						//Transpose Matrix
				skyboxMatrix[i].getPtr()); //Pointer to ModelViewMatrixValues

			glBindTexture(GL_TEXTURE_2D, skyboxTexture[i]);
			skybox[i].Draw(SkyboxVertexPositionAttribute, SkyboxVertexNormalAttribute, SkyboxVertexTexcoordAttribute);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glUseProgram(shaderProgramID);

		glUniformMatrix4fv(
			ProjectionUniformLocation,	//Uniform location
			1,							//Number of Uniforms
			false,						//Transpose Matrix
			ProjectionMatrix.getPtr()); //Pointer to ModelViewMatrixValues

		glUniformMatrix4fv(
			ViewMatrixUniformLoctaion,	//Uniform location
			1,							//Number of Uniforms
			false,						//Transpose Matrix
			ViewMatrix.getPtr()); //Pointer to ModelViewMatrixValues

		glDepthMask(GL_TRUE);

		//Apply Camera Manipluator to Set Model View Matrix on GPU
		ModelViewMatrix.toIdentity();

		planeObj.toIdentitiy();
		planeObj.rotate(90, 0, 1, 0);
		planeObj.SetMatrix(planeObj.GetMatrix() * planeDirection);
		if(thirdPerson) {
			camera = planeObj.GetMatrix().getVector3f() - planeObj.GetMatrix().getForward() * 5 + planeObj.GetMatrix().getUp() * 0.25;
			ViewMatrix.lookAt(camera , planeObj.GetMatrix().getVector3f(), Up);
		} else {
			ViewMatrix.lookAt(camera , planeObj.GetMatrix().getVector3f() + planeObj.GetMatrix().getForward() * 5, Up);
			camera = planeObj.GetMatrix().getVector3f() + planeObj.GetMatrix().getForward() * 2 + planeObj.GetMatrix().getUp() * 0.25;
		}
		planeObj.Draw(MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

		// Terrain Code
		Matrix4x4 terrainMatrix;
		terrainMatrix.scale(500, 500, 500);
		terrainMatrix.rotate(270, 1, 0, 0);
		terrainMatrix.translate(-0.1, -0.5, -0.05);

		glUniformMatrix4fv(
			MVMatrixUniformLocation, //Uniform location
			1,						 //Number of Uniforms
			false,					 //Transpose Matrix
			terrainMatrix.getPtr());			 //Pointer to Matrix Values

		glBindTexture(GL_TEXTURE_2D, terrainTexture);

		//Draw terrain
		terrain.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
		glBindTexture(GL_TEXTURE_2D, 0);

		int ringNum = map.getRingNum();
		std::vector<int> hight = map.getHightList();
		std::vector<int> row = map.getRowList();
		std::vector<int> col = map.getColList();
		std::vector<Matrix4x4> l(ringNum);
		std::vector<Matrix4x4> c(ringNum);

		for(int i(0); i < ringNum; i++) {
			bool conti = false;
			int startPos;
			if(set == false) {
				for(int j = 0; j < ringNum; j++) {
					scored.push_back(false);
				}
				set = true;
			}

			//Set rings to map 0,0
			l[i].scale(5,5,5);
			l[i].rotate(90, 1, 0, 0);
			l[i].rotate(270, 0, 0, 1);
			l[i].translate(-(int)(map.getCol()/2)*10, 10, 3.7);

			l[i].translate(col[i]*10, row[i]*10, -hight[i]/10 - 3.7);

			float distance = 1.5;

			if(planeObj.GetMatrix().getVector3f().x > l[i].getVector3f().x - 1 && planeObj.GetMatrix().getVector3f().x < l[i].getVector3f().x + 1) {
				if(planeObj.GetMatrix().getVector3f().distance(l[i].getVector3f()) > 4 && planeObj.GetMatrix().getVector3f().distance(l[i].getVector3f()) < 7) {
					Restart();
				}
			}

			if(planeObj.GetMatrix().getVector3f().distance(l[i].getVector3f()) < 3 && (scored[i] == false)) {
				score++;
				scored[i] = true;
				collected.push_back(i);
			}

			glUniformMatrix4fv(
				MVMatrixUniformLocation, //Uniform location
				1,						 //Number of Uniforms
				false,					 //Transpose Matrix
				l[i].getPtr());			 //Pointer to Matrix Values

			//Draw loop
			loop.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
			glBindTexture(GL_TEXTURE_2D, 0);

			c[i] = l[i];
			c[i].scale(0.2,0.2,0.2);
			c[i].rotate(90,1,0,0);
			coinRotation += 0.01 * time_change;
			c[i].rotate(coinRotation,0,1,0);

			glUniformMatrix4fv(
				MVMatrixUniformLocation, //Uniform location
				1,						 //Number of Uniforms
				false,					 //Transpose Matrix
				c[i].getPtr());			 //Pointer to Matrix Values

			for(int j = 0; j < collected.size(); j++) {
				if(i == collected[j]){
					conti = true;
				}
			}

			int count = 0;
			for(int j = 0; j < scored.size(); j++) {
				if(scored[j] == true) {
					count++;
				}
			}

			if(count == ringNum) {
				for(int j = 0; j < levels.size(); j++) {
					NextLevel(levels[j]);
				}
			}

			if(conti) {
				continue;
			}


			glBindTexture(GL_TEXTURE_2D, coinTexture);
			coin.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	if(planeObj.GetMatrix().getVector3f().y < -24.5) {
		Restart();
	}

	//Unuse Shader
	glUseProgram(0);

	if(!start) {
		glColor3f(256,256,256);
		glRasterPos2f(-0.28,0);
		std::string startText = "Press Space to begin";
		for(int i = 0; i < startText.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, startText[i]);
		}
	}

	if(start) {
		//HUD
		glRasterPos2f(-0.9,0.9);
		std::string hudText = "Score: ";
		hudText += std::to_string(score);
		hudText += "    Time: ";
		hudText += std::to_string(new_time/1000);
		hudText = hudText.substr(0, hudText.find(".")+4);
		for(int i = 0; i < hudText.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hudText[i]);
		}

		glRasterPos2f(-0.9,0.8);
		hudText = "Altitude: ";
		hudText += std::to_string(planeObj.GetMatrix().getVector3f().y + 24.5);
		hudText = hudText.substr(0, hudText.find(".")+4);
		for(int i = 0; i < hudText.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hudText[i]);
		}

		glRasterPos2f(-0.9,0.7);
		hudText = "Yaw: ";
		hudText += std::to_string(planeObj.GetMatrix().getRotation().x);
		hudText = hudText.substr(0, hudText.find(".")+4);
		for(int i = 0; i < hudText.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hudText[i]);
		}

		glRasterPos2f(-0.9,0.6);
		hudText = "Pitch: ";
		hudText += std::to_string(planeObj.GetMatrix().getRotation().y);
		hudText = hudText.substr(0, hudText.find(".")+4);
		for(int i = 0; i < hudText.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hudText[i]);
		}

		glRasterPos2f(-0.9,0.5);
		hudText = "Roll: ";
		hudText += std::to_string(planeObj.GetMatrix().getRotation().z);
		hudText = hudText.substr(0, hudText.find(".")+4);
		for(int i = 0; i < hudText.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hudText[i]);
		}
	}

	//Swap Buffers and post redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

void NextLevel(std::string levelFile) {
	map.generateMap(levelFile);
	start = false;
	planeDirection.toIdentity();
	set = false;
	score = 0;
	scored.clear();
	collected.clear();
}

void Restart() {
	start = false;
	planeDirection.toIdentity();
	set = false;
	score = 0;
	scored.clear();
	collected.clear();
}

void MoveForward(float speed)
{
	planeDirection.translate(0, 0, speed);
}

void Yaw(float angle)
{
	planeRotation.rotate(angle, 0, 1, 0);
}

void Pitch(float angle)
{
	planeRotation.rotate(angle, 1, 0, 0);
}

void Roll(float angle) {
	planeRotation.rotate(angle, 0, 0, 1);
}

//! Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	//Quits program when esc is pressed
	if (key == 27) //esc key code
	{
		exit(0);
	}

	//Set key status
	keyStates[key] = true;
}

//! Handle key up situation
void keyUp(unsigned char key, int x, int y)
{
	if (keyStates['x']) {
		thirdPerson = !thirdPerson;
	}
	keyStates[key] = false;
}

//! Handle Keys
void handleKeys()
{
	const float rotateSpeed = 0.0001 * time_change, moveSpeed = 0.009 * time_change;

	//Yaw
	if (keyStates['a'])
	{
		yaw = true;
		Yaw(rotateSpeed);
	}
	else if (keyStates['d'])
	{
		yaw = true;
		Yaw(-rotateSpeed);
	}
	else
	{
		yaw = false;
	}

	//Pitch
	if (keyStates['w'])
	{
		pitch = true;
		Pitch(rotateSpeed);
	}
	else if (keyStates['s'])
	{
		pitch = true;
		Pitch(-rotateSpeed);
	}
	else
	{
		pitch = false;
	}

	//Roll
	if (keyStates['e']) {
		roll = true;
		Roll(rotateSpeed);
	} else if (keyStates['q']) {
		roll = true;
		Roll(-rotateSpeed);
	} else {
		roll = false;
	}

	//Move Forward
	if (keyStates[' '])
	{
		start = true;
		MoveForward(moveSpeed);
	}
}

//! Timer Function
void Timer(int value)
{
	// Do something

	//Call function again after 10 milli seconds
	glutTimerFunc(10, Timer, 0);
}