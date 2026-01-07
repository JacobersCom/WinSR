#pragma once
#include "Graphics_Math.h"
#include "Shader.h"




void DrawWithDepth(int x, int y, float depth, Pixel color) {
	
	if (x > SCREEN_W - 1 || y > SCREEN_H - 1 || x < 0 || y < 0) {
		return;
	}
	int index = idx(x, y, SCREEN_W);

	if (depth >= DEPTH_BUFFER[index]) return;
	
	DEPTH_BUFFER[index] = depth;
	COLOR_BUFFER[index] = color;
}

void Draw(int x, int y, Pixel color) {

	if (x > SCREEN_W - 1 || y > SCREEN_H - 1) return;
	int index = idx(x, y, SCREEN_W);

	COLOR_BUFFER[index] = color;
}

void FillBuffer() {

	for (unsigned i = 0; i < NUM_OF_PIXELS; i++)
	{
		COLOR_BUFFER[i] = 0x301934;
	}
}

void StarDraw() {

	Vertex* _StarField = new Vertex[NUM_OF_STARS];

	for (unsigned int i = 0; i < 3000; i++)
	{
		_StarField[i] = StarField[i];

		if (VertexShader)VertexShader(_StarField[i]);
		if (PixelShader)PixelShader(_StarField->color);
	}

	for (unsigned i = 0; i < NUM_OF_STARS; i++)
	{
		float x = NDC_X(_StarField[i].x);
		float y = NDC_Y(_StarField[i].y);
		float z = _StarField[i].z;
		Pixel color = _StarField->color;

		DrawWithDepth(x,y,z, color);
	}

	delete _StarField;
	
}

void BetterBruteTriangle(const Vertex& A, const Vertex& B, const Vertex& C, Pixel color) {

	Vertex copyA = A;
	Vertex copyB = B;
	Vertex copyC = C;

	Pixel copyColor = color;


	if (VertexShader) {
		VertexShader(copyA);
		VertexShader(copyB);
		VertexShader(copyC);
	}
	

	//Transfer points from 3D to 2D
	float Ax = float(NDC_X(copyA.x));
	float Ay = float(NDC_Y(copyA.y));
	float Az = copyA.z;
	float Au = copyA.u;
	float Av = copyA.v;


	float Bx = float(NDC_X(copyB.x));
	float By = float(NDC_Y(copyB.y));
	float Bz = copyB.z;
	float Bu = copyB.u;
	float Bv = copyB.v;

	float Cx = float(NDC_X(copyC.x));
	float Cy = float(NDC_Y(copyC.y));
	float Cz = copyC.z;
	float Cu = copyC.u;
	float Cv = copyC.v;


	//Calculate starting and ending values
	int startingX = (int)min(Ax, min(Bx, Cx));
	int startingY = (int)min(Ay, min(By, Cy));
	int endingX = (int)max(Ax, max(Bx, Cx));
	int endingY = (int)max(Ay, max(By, Cy));

	startingX = max(0, startingX);
	startingY = max(0, startingY);

	endingX = min(SCREEN_W - 1, endingX);
	endingY = min(SCREEN_H - 1, endingY);

	for (unsigned y = startingY; y <= endingY; y++)
	{
		float Py = (float)y + 0.5;

		for (unsigned x = startingX; x <= endingX; x++)
		{
			float Px = (float)x+ 0.5;

			Barycentric trigCoords = FindBarycentricCoordinates(Ax, Ay, Bx, By, Cx, Cy, Px, Py);

			if (trigCoords.b >= 0.0f && trigCoords.a >= 0.0f && trigCoords.c >= 0.0f &&
				trigCoords.b <= 1.0f && trigCoords.a <= 1.0f && trigCoords.c <= 1.0f) { 

				const float depth = Az * trigCoords.a + Bz * trigCoords.b  + Cz * trigCoords.c;
				
				float invW = trigCoords.a * copyA.invW + trigCoords.b * copyB.invW + trigCoords.c * copyC.invW;
				float invU = trigCoords.a * copyA.uW + trigCoords.b * copyB.uW + trigCoords.c * copyC.uW;
				float invV = trigCoords.a * copyA.vW + trigCoords.b * copyB.vW + trigCoords.c * copyC.vW;

				float u = invU / invW;
				float v = invV / invW;

 				unsigned int texture = PS_Text(u, v, StoneHenge_width, StoneHenge_height, StoneHenge_pixels);
				unsigned int lerpedColor = ColorLerp(copyA.color, copyB.color, copyC.color, trigCoords);
				unsigned finalColor = ColorMultiply(texture, lerpedColor);


				DrawWithDepth(x, y, depth, finalColor);
			}
		}
	}
}

void ParametricLines(const Vertex& start, const Vertex& end, Pixel color) {

	//Copies start and end points
	Vertex copy_start = start;
	Vertex copy_end = end;
	Pixel copy_color = color;

	//Will only mod the copies. Should happen before reading pos
	if (VertexShader) {
		VertexShader(copy_start);
		VertexShader(copy_end);
	}

	//Covernt all endpoints to pixel values
	int x1 = NDC_X(copy_start.vertices[0]);
	int y1 = NDC_Y(copy_start.vertices[1]);

	int x2 = NDC_X(copy_end.vertices[0]);
	int y2 = NDC_Y(copy_end.vertices[1]);
	 
	int rocX = fabsf(x2 - x1);
	int rocY = fabsf(y2 - y1);
	//If X is greater than Y return X else check if they are equal if not check if return Y
	int greatestROC = (rocX > rocY) ? rocX :
				 (rocX == rocY) ? rocX : rocY;


	for (float i = 0; i < greatestROC; i++)
	{

		float t = i / greatestROC;
		int x = fabsf(round((x1 + t * (x2 - x1))));
		int y = fabsf(round((y1 + t * (y2 - y1))));
		Draw(x, y,  copy_color);

	}
}


void DrawGrid() {

	const float t = 0.5f;
	const int N = 10;

	Vertex A; A.x = -t; A.y = 0; A.z = -t; A.w = 1;
	Vertex B; B.x = +t; B.y = 0; B.z = -t; B.w = 1;
	Vertex C; C.x = +t; C.y = 0; C.z = +t; C.w = 1;
	Vertex D; D.x = -t; D.y = 0; D.z = +t; D.w = 1;

	const float deltaABx = (B.x - A.x) / float(N);
	const float deltaABz = (B.z - A.z) / float(N);
	const float deltaDCx = (C.x - D.x) / float(N);
	const float deltaDCz = (C.z - D.z) / float(N);

	const float deltaDAx = (D.x - A.x) / float(N);
	const float DeltaDAz = (D.z - A.z) / float(N);
	const float deltaCBx = (C.x - B.x) / float(N);
	const float deltaCBz = (C.z - B.z) / float(N);

	Vertex bottom = A;
	Vertex top = D;
	Vertex left = A;
	Vertex right = B;

	for (int i = 0; i <= N; ++i)
	{
		ParametricLines(bottom, top, 0xffffffff);

		ParametricLines(left, right, 0xffffffff);

		bottom.x += deltaABx; bottom.z += deltaABz;
		top.x += deltaDCx; top.z += deltaDCz;
		left.x += deltaDAx; left.z += DeltaDAz;
		right.x += deltaCBx; right.z += deltaCBz;

	}
	
}

void DrawCube() {


// -- Renders from the back forward --

// -- Back square --

	//Back top horizontal
	ParametricLines(CubeVertices[0],CubeVertices[1], 0xff00ff00);
	//Back bottom horizontal
	ParametricLines(CubeVertices[2], CubeVertices[3], 0xff00ff00);
	//Back left vertical
	ParametricLines(CubeVertices[0], CubeVertices[2], 0xff00ff00);
	//Back right vertical
	ParametricLines(CubeVertices[1], CubeVertices[3], 0xff00ff00);

// -- Front square --
	
	//Front top horizontal
	ParametricLines(CubeVertices[4], CubeVertices[5], 0xff00ff00);
	//Front bottom horizontal
	ParametricLines(CubeVertices[6], CubeVertices[7], 0xff00ff00);
	//Front left vertical
	ParametricLines(CubeVertices[4], CubeVertices[6], 0xff00ff00);
	//Front right vertical
	ParametricLines(CubeVertices[5], CubeVertices[7], 0xff00ff00);

// -- Left side  sqaure --
	ParametricLines(CubeVertices[0], CubeVertices[4], 0xff00ff00);
	ParametricLines(CubeVertices[2], CubeVertices[6], 0xff00ff00);

//	-- Right side square

	ParametricLines(CubeVertices[1], CubeVertices[5], 0xff00ff00);
	ParametricLines(CubeVertices[3], CubeVertices[7], 0xff00ff00);
}

void CubeWithTriangles() {

	//Front Face
	BetterBruteTriangle(TriangleVertices[0], TriangleVertices[1], TriangleVertices[2], 0xffff0000);
	BetterBruteTriangle(TriangleVertices[1], TriangleVertices[3], TriangleVertices[2], 0xffff0000);

	//Left Face
	BetterBruteTriangle(TriangleVertices[8], TriangleVertices[9], TriangleVertices[11], 0xffff00ff);
	BetterBruteTriangle(TriangleVertices[10], TriangleVertices[8], TriangleVertices[11], 0xffff00ff);

	//Right Face
	BetterBruteTriangle(TriangleVertices[12], TriangleVertices[13], TriangleVertices[15], 0xff00ff00);
	BetterBruteTriangle(TriangleVertices[14], TriangleVertices[12], TriangleVertices[15], 0xff00ff00);

	//Back Face
	BetterBruteTriangle(TriangleVertices[6], TriangleVertices[5], TriangleVertices[4], 0xffffff00);
	BetterBruteTriangle(TriangleVertices[6], TriangleVertices[7], TriangleVertices[5], 0xffffff00);

	//Bottom Face
	BetterBruteTriangle(TriangleVertices[16], TriangleVertices[17], TriangleVertices[18], 0xff00ffff);
	BetterBruteTriangle(TriangleVertices[18], TriangleVertices[19], TriangleVertices[17], 0xff00ffff);

	//Top Face
	BetterBruteTriangle(TriangleVertices[20], TriangleVertices[21], TriangleVertices[22], 0xff00ffff);
	BetterBruteTriangle(TriangleVertices[22], TriangleVertices[23], TriangleVertices[21], 0xff00ffff);
}


  
void clear() {
	
	for (unsigned i = 0; i < NUM_OF_PIXELS; i++)
	{
		COLOR_BUFFER[i] = 0x00000000;
	}
}

//For buffers that use float point values
void ClearDepthBuffer(float v) {

	for (unsigned i = 0; i < NUM_OF_PIXELS; i++)
	{
		DEPTH_BUFFER[i] = v;
	}
}

void copy() {

	for (unsigned i = 0; i < NUM_OF_PIXELS; i++)
	{
		SCREEN_ARRAY[i] = COLOR_BUFFER[i];
	}
}

void CopyStoneVetices(){
	
	for (unsigned i = 0; i <= 1456; i++)
	{
		STONE_HENGEV[i].x = 0.1f * StoneHenge_data[i].pos[0];
		STONE_HENGEV[i].y = 0.1f * StoneHenge_data[i].pos[1];
		STONE_HENGEV[i].z = 0.1f * StoneHenge_data[i].pos[2];
		STONE_HENGEV[i].w = 1.0f;

		STONE_HENGEV[i].u = StoneHenge_data[i].uvw[0];
		STONE_HENGEV[i].v = StoneHenge_data[i].uvw[1];

		STONE_HENGEV[i].nrm[0] = StoneHenge_data[i].nrm[0];
		STONE_HENGEV[i].nrm[1] = StoneHenge_data[i].nrm[1];
		STONE_HENGEV[i].nrm[2] = StoneHenge_data[i].nrm[2];

	}
	
	
}

void RenderModel() {


	const size_t count = sizeof(StoneHenge_indicies) / sizeof(StoneHenge_indicies[0]);

	//increments by 3 
	for (unsigned i = 0; i + 2 < count; i += 3)
	{

		

		const unsigned int i0 = StoneHenge_indicies[i + 0];
		const unsigned int i1 = StoneHenge_indicies[i + 1];
		const unsigned int i2 = StoneHenge_indicies[i + 2];

		Vertex A = STONE_HENGEV[i0];
		Vertex B = STONE_HENGEV[i1];
		Vertex C = STONE_HENGEV[i2];

		//if (VertexShader) {
		//	VertexShader(A);
		//	VertexShader(B);
		//	VertexShader(C);

		//}

		BetterBruteTriangle(A,B,C, STONE_HENGEV->color);
	}

}



