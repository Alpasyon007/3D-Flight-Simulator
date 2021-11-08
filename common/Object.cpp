#include <Object.h>

Object::Object() {
	std::cout << "Decleration" << std::endl;
};

Object::Object(std::string objFile, std::string textureFile, bool set) {
	if(set) {
		mesh.loadOBJ(objFile);

			//Generate texture and bind
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Get texture Data
		int width, height;
		char *data;
		Texture::LoadBMP(textureFile, width, height, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Cleanup data - copied to GPU
		delete[] data;
	}
}

Matrix4x4 Object::GetMatrix() {
    return matrix;
}

void Object::Initiate(std::string objFile, std::string textureFile) {
		mesh.loadOBJ(objFile);

			//Generate texture and bind
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//Get texture Data
		int width, height;
		char *data;
		Texture::LoadBMP(textureFile, width, height, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Cleanup data - copied to GPU
		delete[] data;
}

void Object::InitiateQuad(std::string textureFile) {
	mesh.initQuad();

	//Generate texture and bind
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Get texture Data
	int width, height;
	char *data;
	Texture::LoadBMP(textureFile, width, height, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Cleanup data - copied to GPU
	delete[] data;
}

void Object::Draw(GLuint UniformLocation, GLuint VertexPositionAttribute, GLuint VertexNormalAttribute, GLuint VertexTexcoordAttribute) {
    glUniformMatrix4fv(
		UniformLocation,    //Uniform location
		1,					//Number of Uniforms
		false,				//Transpose Matrix
        matrix.getPtr());   //Pointer to ModelViewMatrixValues
    
    glBindTexture(GL_TEXTURE_2D, texture);
    mesh.Draw(VertexPositionAttribute, VertexNormalAttribute, VertexTexcoordAttribute);
    glBindTexture(GL_TEXTURE_2D, 0);
};

void Object::translate(float x, float y, float z) {
	matrix.translate(x, y, z);
};

void Object::rotate(float angle, float x, float y, float z) {
	matrix.rotate(angle, x, y, z);
}

void Object::scale(float x, float y, float z) {
	matrix.scale(x, y, z);
}

void Object::SetMatrix(Matrix4x4 m) {
    matrix = m;
}

void Object::SetPos(Vector3f v) {
	matrix.setVector3f(v);
}

void Object::toIdentitiy() {
    matrix.toIdentity();
}