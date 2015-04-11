#include <windows.h>
#include <gl/gl.h>					// OpenGL functions
#include "./msdetours/detours.h"	// Need to replace with custom detours library 
#pragma comment(lib, "Opengl32.lib")

BOOL Found_An_Entity = FALSE;

typedef void (APIENTRY* Type_glBegin)(GLenum mode);
Type_glBegin Original_glBegin = 0;

static void APIENTRY Hooked_glBegin (GLenum mode) {
	// Wallhack
	if( ( mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN ) && Found_An_Entity == FALSE )
	{  
		glDepthRange( 0.0f, 0.5f );
		Found_An_Entity = TRUE;
	}
		
	if( ( mode == GL_POLYGON || mode == GL_TRIANGLES || mode == GL_QUADS || mode == GL_QUAD_STRIP ) && Found_An_Entity == TRUE )
	{
		glDepthRange( 0.0f, 1.0f );
		Found_An_Entity = FALSE;
	}  

	(Original_glBegin)(mode); 
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	 DisableThreadLibraryCalls((HMODULE)hinstDLL); 

	if (fdwReason == DLL_PROCESS_ATTACH) 
	{
		// Apply hooks
		Original_glBegin = (Type_glBegin)(DetourFunction((unsigned char*)GetProcAddress(LoadLibrary("opengl32.dll"), "glBegin"),(unsigned char*)Hooked_glBegin)); 
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		// Remove hooks
		Original_glBegin = (Type_glBegin)(DetourFunction((unsigned char*)GetProcAddress(LoadLibrary("opengl32.dll"), "glBegin"),(unsigned char*)Original_glBegin)); 
	}

	return true; 
}
