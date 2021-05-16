#pragma once

#include "Processing.NDI.Lib.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <dlfcn.h>
#endif

class NDILib_loader {
public:
	NDILib_loader();
	~NDILib_loader();

	const NDIlib_v4* get_NDI();

private: 
	HMODULE hNDILib;
	const NDIlib_v4* p_NDILib;
};