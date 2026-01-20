/*
MReturns is the beginning of the logging system for the MAUS engine. HELD within the MW namespace
- Goal
	- Have return codes for different vulkan and windows API error that are ambiguous 
*/

#ifndef MRETURN_H
#define MRETURN_H



namespace WM
{
	enum class MReturns
	{
		M_SUCCESS = 0,
		M_FAILURE = 1,
	};
}

#endif // MRETURN_H