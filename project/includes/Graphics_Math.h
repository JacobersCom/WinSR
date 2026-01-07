#pragma once

unsigned RedValue(unsigned c) {
	return((c >> 16) & 0xFF);
}
unsigned GreenValue(unsigned c) {
	return ((c >> 8) & 0xFF);
}
unsigned BlueValue(unsigned c) {
	return (c & 0xFF);
}
unsigned Packer(uint8_t r, uint8_t g, uint8_t b) {

	return (r << 16) | (g << 8) | (b);
}

unsigned BGRAtoARGB(unsigned C) {

	unsigned alpha = (C & 0x000000FFu) << 24;
	unsigned red = (C & 0x0000FF00u) << 8;
	unsigned green = (C & 0x00FF0000u) >> 8;
	unsigned blue = (C & 0xFF000000u) >> 24;

	return alpha | red | green | blue;
}

unsigned int ColorClamp(unsigned int a) {
	if (a < 0)a = 0;
	if (a > 255)a = 255;
	return a;
}

unsigned int ColorMultiply(unsigned int ColorA, unsigned int ColorB) {

	float texelR = (float)RedValue(ColorA) / 255;
	float texelG = (float)GreenValue(ColorA) / 255;
	float texelB = (float)BlueValue(ColorA) / 255;

	float lightR = (float)RedValue(ColorB) / 255;
	float lightG = (float)GreenValue(ColorB) / 255;
	float lightB = (float)BlueValue(ColorB) / 255;

	unsigned int R = (unsigned int)ColorClamp(texelR * (lightR + 0.08) * 255);
	unsigned int G = (unsigned int)ColorClamp(texelG * (lightG + 0.08) * 255);
	unsigned int B = (unsigned int)ColorClamp(texelB * (lightB + 0.08) * 255);

	return Packer(R, G, B);
}


unsigned int ColorAdd(unsigned int ColorA, unsigned int ColorB) {

	float _aColorR = (float)RedValue(ColorA);
	float _aColorG = (float)GreenValue(ColorA) ;
	float _aColorB = (float)BlueValue(ColorA);

	float _bColorR = (float)RedValue(ColorB);
	float _bColorG = (float)GreenValue(ColorB);
	float _bColorB = (float)BlueValue(ColorB);

	unsigned int R = (unsigned int)ColorClamp((_aColorR + _bColorR));
	unsigned int G = (unsigned int)ColorClamp((_aColorG + _bColorG));
	unsigned int B = (unsigned int)ColorClamp((_aColorB + _bColorB));
	/*printf("%u\n", R);
	printf("%u\n", G);
	printf("%u\n", B);*/


	return Packer(R, G, B);
}

unsigned int ColorLerp(unsigned int colorA,unsigned int colorB,unsigned int colorC, Barycentric b) {

	float aR = (float)RedValue(colorA);
	/*printf("%f", aR);*/
	float aG = (float)GreenValue(colorA);
	//printf("%f", aG);
	float aB = (float)BlueValue(colorA);
	//printf("%f", aB);

	float bR = (float)RedValue(colorB);
	float bG = (float)GreenValue(colorB);
	float bB = (float)BlueValue(colorB);

	float cR = (float)RedValue(colorC);
	float cG = (float)GreenValue(colorC);
	float cB = (float)BlueValue(colorC);

	unsigned int baryColorA = b.b * aR + b.a * aG + b.c * aB;
	unsigned int baryColorB = b.b * bR + b.a * bG + b.c * bB;
	unsigned int baryColorC = b.b * cR + b.a * cG + b.c * cB;
	/*printf("%u\n", baryColorA);
	printf("%u\n", baryColorC); 
	printf("%u\n", baryColorB);*/
	return Packer(baryColorA, baryColorB, baryColorC);
}

float normalize(float* v) {
	float len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (len > 0.0f) {
		float inv = 1.0f / len;
		v[0] *= inv; v[1] *= inv; v[2] *= inv;
	}
	return len;
}

float Clamp(float a) {

	if (a < 0.0f)a = 0.0f;
	if (a > 1.0f)a = 1.0f;
	return a;
}


float DotProduct(float* A, float* B) {

	
	float dot = (A[0] * B[0]) + (A[1] * B[1]) + (A[2] * B[2]);
	return dot;
}

float ImplicitLine(float x1, float y1, float x2, float y2, float testX, float testY) {

	return (y1 - y2) * testX + (x2 - x1) * testY + (x1 * y2) - (x2 * y1);
}

Barycentric FindBarycentricCoordinates(float Ax, float Ay, float Bx, float By,float Cx, float Cy, float Testx, float Testy) {

	float beta = ImplicitLine(Ax, Ay, Cx, Cy, Bx, By);
	float gamma = ImplicitLine(Bx, By, Ax, Ay, Cx, Cy);
	float alpha = ImplicitLine(Cx, Cy, Bx, By, Ax, Ay);

	float b = ImplicitLine(Ax, Ay, Cx, Cy, Testx, Testy);
	float y = ImplicitLine(Bx, By, Ax, Ay, Testx, Testy);
	float a = ImplicitLine(Cx, Cy, Bx, By, Testx, Testy);

	return { b / beta, y / gamma, a / alpha };
}

Matrix4 Identity() {

	Matrix4 m{};
	m.points[0] = 1.0f;
	m.points[5] = 1.0f;
	m.points[10] = 1.0f;
	m.points[15] = 1.0f;
	return m;
}

Matrix4 Projection(float nearZ, float farZ, float aspect, float fov) {

	// All these values are meant to fit in the range of Z for NDC space

	float FOV = fov * (PI / 180.0f);
	float f = 1.0f / tanf(FOV / 2.0f);

	Matrix4 m{};
	
	m.xx = f / aspect;
	m.yy = f;
	m.zz = farZ / (farZ - nearZ);
	m.zw = 1.0f;
	m.wz = -(farZ * nearZ) / (farZ - nearZ);

	return m;


}

Matrix4 Inverse(Matrix4 v) {

	Matrix4 m;
	
	//Copy all the data
	for (int i = 0; i < 16; i++)
	{
		m.points[i] = v.points[i];
	}

	// xy to yx
	v.xy = v.yx;
	v.yx = m.xy;

	// xz to zx
	v.xz = v.zx;
	v.zx = m.xz;

	//yz to zy
	v.yz = v.zy;
	v.zy = m.yz;

	float tx = v.wx;
	float ty = v.wy;
	float tz = v.wz;

	v.wx = -(tx * v.xx + ty * v.yx + tz * v.zx);
	v.wy = -(tx * v.xy + ty * v.yy + tz * v.zy);
	v.wz = -(tx * v.xz + ty * v.yz + tz * v.zz);

	v.ww = 1.0f;

	return v;
}

Matrix4 translate(float x, float y, float z) {
	Matrix4 m = Identity();
	m.wx = x;
	m.wy = y;
	m.wz = z;
	return m;
}

Matrix4 scale(float s, Matrix4 m) {

	m.xx = s;
	m.yy = s;
	m.zz = s;
	return m;
}

//Uses the Y axis to rotate
Matrix4 RotX(float deg) {

	float rad = deg * PI / 180;
	float c = cosf(rad);
	float s = sinf(rad);

	Matrix4 m = Identity();
	m.yy = c; m.yz = -s;
	m.zy = s; m.zz = c;
	return m;
}
//Uses the X axis to rotate
Matrix4 RotY(float deg) {

	float rad = deg * PI / 180;
	float c = cosf(rad);
	float s = sinf(rad);

	Matrix4 m = Identity();
	m.xx = c; m.xz = s;
	m.zx = -s; m.zz = c;
	return m;
}

Matrix4 RotZ(float deg) {

	float rad = deg * PI / 180;
	float c = cosf(rad);
	float s = sinf(rad);

	Matrix4 m = Identity();
	m.xx = -c; m.xy = -s;
	m.yx = s; m.yy = -c;
	return m;
}

unsigned idx(int x, int y, int rowLenght) {

	return (y * rowLenght) + x;
}

//Will be used in parametric line drawing to convert the 3D points to 2D points
int NDC_X(float ndc_x) {

	return int(((ndc_x * 0.5f) + 0.5f) * (SCREEN_W - 1) + 0.5f);
}

int NDC_Y(float ndc_y) {

	float y = ((1.0f - ndc_y) * float((0.5f * SCREEN_H - 1)));
	return (int)round(y);
}

