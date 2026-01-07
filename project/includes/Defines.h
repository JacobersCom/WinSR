#pragma once
#include "RasterSurface.h"
#include "XTime.h"
#include "greendragon.h"
#include <random>
#include "StoneHenge.h"
#include "StoneHenge_Texture.h"
#include <algorithm>
#include <stdio.h>

std::mt19937 rng(12345);
std::uniform_real_distribution<float> uni(-1.0f, 1.0f);

#define PI 3.1415926535897
#define SCREEN_H 512
#define SCREEN_W 1024
#define NUM_OF_PIXELS (SCREEN_H * SCREEN_W)
#define NUM_OF_STARS 3000

unsigned BACKGROUND_BUFFER[NUM_OF_PIXELS];
float DEPTH_BUFFER[NUM_OF_PIXELS];
unsigned SCREEN_ARRAY[NUM_OF_PIXELS];
unsigned COLOR_BUFFER[NUM_OF_PIXELS];

float nearZ = 0.1f;
float farZ = 10.0;
float FOV = 90.0f;
float aspect = (float)SCREEN_W / (float)SCREEN_H;

XTime times;
using Pixel = unsigned int;


enum KeyCodes
{
	W = 0x57,
	A = 0x41,
	S = 0x53,
	D = 0x44,

};



struct Matrix4 {
public:
	union {
		struct {
			float points[16];
		};
		struct {
			float xx;
			float xy; 
			float xz;
			float xw; 
			float yx;
			float yy;
			float yz;
			float yw;
			float zx;
			float zy;
			float zz;
			float zw;
			float wx;
			float wy;
			float wz;
			float ww;
		};
	};

	Matrix4& operator*(const Matrix4& b) const {

		Matrix4 c{};

		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				float MatA = points[i * 4 + k];
				
				for (int j = 0; j < 4; j++)
				{
					c.points[i * 4 + j] += MatA * b.points[k * 4 + j];
				}
			}
		}
		
		return c;
	}

	Matrix4& operator*=(const Matrix4& b) {

		Matrix4 c{};

		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				float MatA = points[i * 4 + k];

				for (int j = 0; j < 4; j++)
				{
					c.points[i * 4 + j] += MatA * b.points[k * 4 + j];
				}
			}
		}
		*this = c;
		return *this;
	}

};

struct Barycentric {
	union {
		struct {
			float points[3];
		};
		struct {
			float b, c, a;
		};
	};
};




struct Vertex {

	union {
		struct {
		
			float vertices[8];
			
		};
		struct {
			float x, y, z, w, u, v, invW, uW, vW;
		};
	};
	float nrm[3];

	float lightRatio;
	float pointLightRatio;
	Pixel color;
	

	Vertex& operator*=(const Matrix4& m) {

		float x = vertices[0];
		float y = vertices[1];
		float z = vertices[2];
		float w = vertices[3];

		const float* a = m.points;
		
		vertices[0] = (x * a[0]) + (y * a[4]) + (z * a[8]) + (w * a[12]);
		vertices[1] = (x * a[1]) + (y * a[5]) + (z * a[9]) + (w * a[13]);
		vertices[2] = (x * a[2]) + (y * a[6]) + (z * a[10]) + (w * a[14]);
		vertices[3] = (x * a[3]) + (y * a[7]) + (z * a[11]) + (w * a[15]);

		return *this;
	}

	Vertex& operator+(Vertex& v) {

		v.x + v.x;
		v.y + v.y;
		v.z + v.z;

		return *this;
	}
	Vertex& operator-(Vertex& v) {

		v.x - v.x;
		v.y - v.y;
		v.z - v.z;

		return *this;
	}

};

Vertex StarField[NUM_OF_STARS];
Vertex STONE_HENGEV[1457];


Vertex CubeVertices[8] = {

	//Top back left
	{ -0.25, -0.25, -0.25,1 },
	//Top back right
	{ +0.25, -0.25, -0.25,1 },

	//Bottom back left
	{ -0.25, +0.25, -0.25,1 },
	//Bottom back right
	{ +0.25, +0.25, -0.25,1 },

	//Top front left
	{ -0.25, -0.25, +0.25,1 },
	//Top front right right	
	{ +0.25, -0.25, +0.25,1 },

	//Bottom front left
	{ -0.25, +0.25, +0.25,1 },
	//Bottom front right
	{ +0.25, +0.25, +0.25,1 }
};

Vertex TriangleVertices[24] = {

	//Front face
	{ -0.25, +0.25, -0.25, 1, 1.0, 1.0},//top left
	{ -0.25, -0.25, -0.25, 1, 1.0, 0.0},//bottom left
	{ +0.25, +0.25, -0.25, 1 ,0.0, 1.0},//top right 
	{ +0.25, -0.25, -0.25, 1, 0.0, 0.0},//bottom right

	//Back face
	{ -0.25, +0.25, +0.25, 1, 0.0, 1.0},//bottom left
	{ -0.25, -0.25, +0.25, 1, 0.0, 0.0},//top left
	{ +0.25, +0.25, +0.25, 1, 1.0, 1.0},//bottom right
	{ +0.25, -0.25, +0.25, 1, 1.0, 0.0},//top right

	//Left face
	{ -0.25, +0.25, +0.25, 1, 1.0, 1.0},// top back
	{ -0.25, -0.25, +0.25, 1, 1.0, 0.0},// bottom back
	{ -0.25, +0.25, -0.25, 1, 0.0, 1.0},// top front
	{ -0.25, -0.25, -0.25, 1, 0.0, 0.0},// bottom front

	//Right face
	{ +0.25, +0.25, -0.25, 1 ,1.0, 1.0},//top back
	{ +0.25, -0.25, -0.25, 1, 1.0, 0.0},// bottom back
	{ +0.25, +0.25, +0.25, 1, 0.0, 1.0},// top front
	{ +0.25, -0.25, +0.25, 1, 0.0, 0.0},// bottom front
	
	//Top face
	{ -0.25, +0.25, -0.25, 1, 0.0, 1.0},//front left
	{ -0.25, +0.25, +0.25, 1, 0.0, 0.0},//back left
	{ +0.25, +0.25, -0.25, 1 ,1.0, 1.0},//front right 
	{ +0.25, +0.25, +0.25, 1, 1.0, 0.0},//back right

	//Bottom face
	{ -0.25, -0.25, -0.25, 1, 0.0, 1.0},//front left
	{ +0.25, -0.25, -0.25, 1, 0.0, 0.0},//front right
	{ -0.25, -0.25, +0.25, 1, 1.0, 1.0},//back left
	{ +0.25, -0.25, +0.25, 1, 1.0, 0.0},//back back


};


Vertex& operator+(float& s, Vertex& v) {
	Vertex& results = v;
	results.x + s;
	results.y + s;
	results.z + s;
	return results;
}

Vertex& operator*(float& s, Vertex& v) {
	
	Vertex results = v;
	results.x *= s;
	results.y *= s;
	results.z *= s;

	return results;
}


