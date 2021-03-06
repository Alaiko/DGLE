//==============================================================================================================================//
//																																//
// Attention! This project is for debugging engine purpose only. It's not a tutorial nor sample it's only for engine developers.//
// You can do testing for any feature which you are working on here. And please do not commit this file!						//
//																																//
//==============================================================================================================================//

#include <DGLE.h>

#define OPENGL_LEGACY_BASE_OBJECTS
#include <DGLE_CoreRenderer.h>

#include <string>

#pragma comment(linker, "/defaultlib:opengl32.lib")
#pragma message("Linking with \"opengl32.lib\".")

DGLE_DYNAMIC_FUNC

using namespace DGLE;

#if defined(PLATFORM_WINDOWS)

#ifdef NDEBUG // for release build paths are configured to run executeble itself
#	define DLL_PATH				"..\\DGLE.dll"
#	define PLUGS_PATH			"..\\plugins\\"
#	ifdef _WIN64
#		define RESOURCE_PATH	"..\\..\\..\\..\\resources\\"
#	else
#		define RESOURCE_PATH	"..\\..\\..\\resources\\"
#	endif
#else // for debug builds paths are configured for debugging from DGLE project
#	ifdef _WIN64
#		define DLL_PATH			"..\\..\\..\\bin\\windows\\x64\\DGLE.dll"
#		define PLUGS_PATH		"..\\..\\..\\bin\\windows\\x64\\plugins\\"
#	else
#		define DLL_PATH			"..\\..\\..\\bin\\windows\\DGLE.dll"
#		define PLUGS_PATH		"..\\..\\..\\bin\\windows\\plugins\\"
#	endif
#	define RESOURCE_PATH		"..\\..\\..\\resources\\"
#endif

#else // PLATFORM_WINDOWS
#	error Unknown platform!
#endif

#define APP_CAPTION	"DevTest"
#define SCREEN_X 800
#define SCREEN_Y 600

IEngineCore *pEngineCore = NULL;
IInput *pInput = NULL;
IResourceManager *pResMan = NULL;
ICoreRenderer *pCoreRenderer = NULL;
IRender *pRender = NULL;
IRender2D *pRender2D = NULL;
IRender3D *pRender3D = NULL;
uint uiCounter = 0;

ITexture *pTex = NULL;
IModel *pModel = NULL;

void DGLE_API Init(void *pParameter)
{
	pEngineCore->GetSubSystem(ESS_INPUT, (IEngineSubSystem *&)pInput);
	pEngineCore->GetSubSystem(ESS_RESOURCE_MANAGER, (IEngineSubSystem *&)pResMan);
	pEngineCore->GetSubSystem(ESS_CORE_RENDERER, (IEngineSubSystem *&)pCoreRenderer);
	pEngineCore->GetSubSystem(ESS_RENDER, (IEngineSubSystem *&)pRender);

	//pCoreRenderer->ToggleStateFilter(false);

	pRender->SetClearColor(ColorGray());
	pRender->GetRender2D(pRender2D);
	pRender->GetRender3D(pRender3D);

	pResMan->Load(RESOURCE_PATH"tests\\npot_tex.bmp", (IEngineBaseObject *&)pTex, TEXTURE_LOAD_DEFAULT_2D);
	pResMan->GetDefaultResource(EOT_MODEL, (IEngineBaseObject *&)pModel);
}

void DGLE_API Free(void *pParameter)
{
}

void DGLE_API Update(void *pParameter)
{
	bool b_prsd;

	pInput->GetKey(KEY_ESCAPE, b_prsd);

	if (b_prsd)
		pEngineCore->QuitEngine();

	++uiCounter;
}

void DGLE_API Render(void *pParameter)
{
	pRender3D->MultMatrix(MatrixTranslate(TVector3(0.f, 0.f, -2.5f)));
	pRender3D->MultMatrix(MatrixRotate((float)uiCounter, TVector3(0.2f, 0.4f, 0.1f)));

	pTex->Bind();
	pModel->Draw();
	pRender3D->DrawAxes(2.f, false);

	uint w, h;
	pTex->GetDimensions(w, h);
	pTex->Draw2DSimple((SCREEN_X - (int)w) / 2, (SCREEN_Y - (int)h) / 2);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (GetEngine(DLL_PATH, pEngineCore))
	{
		if (SUCCEEDED(pEngineCore->InitializeEngine(NULL, APP_CAPTION, TEngineWindow(SCREEN_X, SCREEN_Y, false, false, MM_NONE, EWF_ALLOW_SIZEING), 33, (E_ENGINE_INIT_FLAGS)(EIF_LOAD_ALL_PLUGINS))))
		{
			pEngineCore->ConsoleVisible(true);
			pEngineCore->ConsoleExecute("core_profiler 1");
			
			pEngineCore->AddProcedure(EPT_INIT, &Init);
			pEngineCore->AddProcedure(EPT_FREE, &Free);
			pEngineCore->AddProcedure(EPT_UPDATE, &Update);
			pEngineCore->AddProcedure(EPT_RENDER, &Render);
			
			pEngineCore->StartEngine();
		}

		FreeEngine();
	}
	else
		MessageBox(NULL, "Couldn't load \"" DLL_PATH "\"!", APP_CAPTION, MB_OK | MB_ICONERROR | MB_SETFOREGROUND);

	return 0;
}