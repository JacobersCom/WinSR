#pragma once
#include "Vertex.hpp"

uint32_t(*PixelShader)(uint32_t color);
float* (*VertexShader)(Vertex& vert);