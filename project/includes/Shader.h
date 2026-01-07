#pragma once
#include "Graphics_Math.h"

//Active vertex shader. Modifies an incoming vertex, pre-rasterization. Function pointer.
void (*VertexShader)(Vertex&);
void (*PixelShader)(Pixel&);

float lightDir[3] = { 0.577, 0.577, -0.577 };
float negLightDir[3] = { -lightDir[0], -lightDir[1], -lightDir[2] };
float pointLight[3] = { -1.0f, 0.5f, 1.0f };
unsigned int lightColor = 0xFFC0C0F0;
unsigned int pointLightColor = 0xFFFFFF00;

Matrix4 VS_World;
Matrix4 VS_View;
Matrix4 VS_Projection;

//World View Projection 
void VS_WVP(Vertex& v) {

	
	v *= VS_World;
	
	normalize(negLightDir);
	normalize(v.nrm);
	
	v.lightRatio = Clamp(DotProduct(negLightDir, v.nrm));
	
	float pointLightDir[3] = {pointLight[0] - v.x , pointLight[1] - v.y  , pointLight[2] -  v.z };
	float pointLightDis = sqrt(pointLightDir[0] * pointLightDir[0] + pointLightDir[1] * pointLightDir[1] + pointLightDir[2] * pointLightDir[2]);
	float pointLightDirNL[3] = {pointLightDir[0] / pointLightDis, pointLightDir[1] / pointLightDis, pointLightDir[2] / pointLightDis};
	

	
	//normalize(pointLight);
	v *= VS_View;
	v *= VS_Projection;

	v.invW = 1.0f / v.w;
	v.uW = v.u * v.invW;
	v.vW = v.v * v.invW;
	
	
	
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;

	//const float pointLightRange = 20.0f;
	//float att = Clamp(1.0 - (pointLightDis / pointLightRange));
	//att *= att; // Creates nonlinear falloff
	//v.pointLightRatio = Clamp(DotProduct(pointLightDirNL, v.nrm));

	//printf("%f\n", v.lightRatio);

	//printf("%f\n", v.pointLightRatio);
	


// ====================
// DIRECTIONAL LIGHT
// ====================
	float dirR = float(RedValue(lightColor));
	float dirG = float(GreenValue(lightColor));
	float dirB = float(BlueValue(lightColor));
	/*printf("%f\n", dirR);
	printf("%f\n", dirG);
	printf("%f\n", dirB);*/

	
	unsigned directionalR = (1.0f - v.lightRatio) * 0x00 + v.lightRatio * dirR;
	unsigned directionalG = (1.0f - v.lightRatio) * 0x00 + v.lightRatio * dirG;
	unsigned directionalB = (1.0f - v.lightRatio) * 0x00 + v.lightRatio * dirB;
	/*printf("%u\n", R);
	printf("%u\n", G);
	printf("%u\n", B);*/
	unsigned int directionalLight = Packer(directionalR, directionalG, directionalB);
	

// ====================
// POINT LIGHT
// ====================

	/*float ptR = float(RedValue(pointLightColor));
	float ptG = float(GreenValue(pointLightColor));
	float ptB = float(BlueValue(pointLightColor));

	
	float tPoint = v.pointLightRatio * att;
	unsigned int pointR = (1.0f - tPoint) * 0x00 + tPoint * ptR;
	unsigned int pointG = (1.0f - tPoint) * 0x00 + tPoint * ptG;
	unsigned int pointB = (1.0f - tPoint) * 0x00 + tPoint * ptB;*/
	/*printf("%u\n", pointR);
	printf("%u\n", pointG);
	printf("%u\n", pointB);*/

	/*unsigned int pointLightColor = Packer(pointR, pointG, pointB);*/

	unsigned newColor = directionalLight;
	v.color = newColor;
	
}

//Accesses the pixel struct to change the color of a pixel;
void PS_Color(Pixel& color) {
	
	 color = 0xFFFFFFFF;
}

unsigned PS_Text(float u, float v, unsigned src_w, unsigned src_h, const unsigned* src) {

	if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0) {
		
		int tu = int((u * (src_w - 1)));
		int tv = int(((1.0f - v) * (src_h - 1)));

		unsigned texel = idx(tu, tv, src_w);

		unsigned texture = BGRAtoARGB(src[texel]);

		return texture;
	}
	
}  