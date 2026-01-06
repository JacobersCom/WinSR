#include "Math.hpp"

uint32_t Lerp(int start, float ratio, int deltaStart)
{
	return start + ratio * deltaStart;
}