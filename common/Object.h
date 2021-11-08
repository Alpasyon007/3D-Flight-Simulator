#pragma once
#include <Mesh.h>
#include <Matrix.h>
#include <Texture.h>

class Object : public Mesh {
    private:
        Mesh mesh;
        Matrix4x4 matrix;
        GLuint texture;
    public:
        Object();
        Object(std::string objFile, std::string textureFile, bool set);

        Matrix4x4 GetMatrix();

        void Initiate(std::string objFile, std::string textureFile);
        void InitiateQuad(std::string textureFile);
        void Draw(GLuint UniformLocation, GLuint VertexPositionAttribute, GLuint VertexNormalAttribute, GLuint VertexTexcoordAttribute);

        void translate(float x, float y, float z);
        void rotate(float angle, float x, float y, float z);
        void scale(float x, float y, float z);

        void SetMatrix(Matrix4x4);
        void SetPos(Vector3f);
        void toIdentitiy();
};