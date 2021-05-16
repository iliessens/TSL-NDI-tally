#include "NDIlib_loader.h"

#include <csignal>
#include <string>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <atomic>
#include <stdexcept>

NDILib_loader::NDILib_loader() {
#ifdef _WIN32
	// We check whether the NDI run-time is installed
	const char* p_ndi_runtime_v4 = getenv(NDILIB_REDIST_FOLDER);
	if (!p_ndi_runtime_v4)
	{	// The NDI run-time is not yet installed. Let the user know and take them to the download URL.
		MessageBoxA(NULL, "Please install the NewTek NDI Runtimes to use this application.", "Runtime Warning.", MB_OK);
		ShellExecuteA(NULL, "open", NDILIB_REDIST_URL, 0, 0, SW_SHOWNORMAL);

		throw std::runtime_error("NDI runtime not installed");
	}

	// We now load the DLL as it is installed
	std::string ndi_path = p_ndi_runtime_v4;
	ndi_path += "\\" NDILIB_LIBRARY_NAME;

	// Try to load the library
	hNDILib = LoadLibraryA(ndi_path.c_str());

	// The main NDI entry point for dynamic loading if we got the librari
	const NDIlib_v4* (*NDIlib_v4_load)(void) = NULL;
	if (hNDILib)
		*((FARPROC*)&NDIlib_v4_load) = GetProcAddress(hNDILib, "NDIlib_v4_load");

	// If we failed to load the library then we tell people to re-install it
	if (!NDIlib_v4_load)
	{	// Unload the DLL if we loaded it
		if (hNDILib)
			FreeLibrary(hNDILib);

		// The NDI run-time is not installed correctly. Let the user know and take them to the download URL.
		MessageBoxA(NULL, "Please re-install the NewTek NDI Runtimes to use this application.", "Runtime Warning.", MB_OK);
		ShellExecuteA(NULL, "open", NDILIB_REDIST_URL, 0, 0, SW_SHOWNORMAL);
		throw std::runtime_error("Loading NDI library failed");
	}
#else
	std::string ndi_path;

	const char* p_NDI_runtime_folder = getenv(NDILIB_REDIST_FOLDER);
	if (p_NDI_runtime_folder)
	{
		ndi_path = p_NDI_runtime_folder;
		ndi_path += NDILIB_LIBRARY_NAME;
	}
	else ndi_path = NDILIB_LIBRARY_NAME;

	// Try to load the library
	void* hNDILib = dlopen(ndi_path.c_str(), RTLD_LOCAL | RTLD_LAZY);

	// The main NDI entry point for dynamic loading if we got the library
	const NDIlib_v4* (*NDIlib_v4_load)(void) = NULL;
	if (hNDILib)
		*((void**)&NDIlib_v4_load) = dlsym(hNDILib, "NDIlib_v4_load");

	// If we failed to load the library then we tell people to re-install it
	if (!NDIlib_v4_load)
	{	// Unload the library if we loaded it
		if (hNDILib)
			dlclose(hNDILib);

		printf("Please re-install the NewTek NDI Runtimes from " NDILIB_REDIST_URL " to use this application.");
		return 0;
	}
#endif

	// Lets get all of the DLL entry points
	p_NDILib = NDIlib_v4_load();

	// We can now run as usual
	if (!p_NDILib->initialize())
	{	// Cannot run NDI. Most likely because the CPU is not sufficient (see SDK documentation).
		// you can check this directly with a call to NDIlib_is_supported_CPU()
		printf("Cannot run NDI.");
		throw std::runtime_error("Error while initializing NDI library. Is your CPU supported?");
	}

}

const NDIlib_v4* NDILib_loader::get_NDI() {
	return this->p_NDILib;
}

NDILib_loader::~NDILib_loader() {
	// Not required, but nice
	p_NDILib->destroy();

	// Free the NDI Library
#if _WIN32
	FreeLibrary(hNDILib);
#else
	dlclose(hNDILib);
#endif
}