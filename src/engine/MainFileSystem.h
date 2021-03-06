/**
\author		Korotkov Andrey aka DRON
\date		23.03.2016 (c)Korotkov Andrey

This file is a part of DGLE project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE.h" for more details.
*/

#pragma once

#include "Common.h"
#include "HDDFileSystem.h"
#include "DCPFileSystem.h"

struct TVFileSystem
{
	std::string ext;
	std::string discr;
	IFileSystem *fs;
	void *param;
	void (DGLE_API *pdc)(void *pParameter, IFileSystem *pVFS);

	TVFileSystem(const char *pcExt, const char *pcDiscr, IFileSystem *pVFS, void (DGLE_API *pDeleteCallback)(void *pParameter, IFileSystem *pVFS), void *pParameter)
	{
		ext		= ToUpperCase(pcExt);
		discr	= pcDiscr;
		fs		= pVFS;
		pdc		= pDeleteCallback;
		param	= pParameter;
	}
};

class CDummyFile;
class CDCPFileSystem;

class CMainFileSystem final : public CInstancedObj, public IMainFileSystem
{
	CDummyFile *_pDummyFile;

	CHDDFileSystem *_pHDDFS;
	CDCPFileSystem *_pDCPFS;

	std::vector<TVFileSystem> _clVFileSystems;
	std::string _strVFSsDescs;

	static bool DGLE_API _s_ConListVFS(void *pParameter, const char *pcParam);
	static void DGLE_API _s_FSDeleteCallback(void *pParameter, IFileSystem *pVFS);

public:
	
	CMainFileSystem(uint uiInstIdx);
	~CMainFileSystem();

	void UnregisterAndFreeAll();

	DGLE_RESULT DGLE_API LoadFile(const char *pcFileName, IFile *&prFile) override;
	DGLE_RESULT DGLE_API FreeFile(IFile *&prFile) override;
	DGLE_RESULT DGLE_API GetVirtualFileSystem(const char *pcVFSExtension, IFileSystem *&prVFS) override;
	DGLE_RESULT DGLE_API RegisterVirtualFileSystem(const char *pcVFSExtension, const char *pcDiscription, IFileSystem *pVFS, void (DGLE_API *pDeleteCallback)(void *pParameter, IFileSystem *pVFS), void *pParameter) override;
	DGLE_RESULT DGLE_API UnregisterVirtualFileSystem(const char *pcVFSExtension) override;
	DGLE_RESULT DGLE_API GetRegisteredVirtualFileSystems(char *pcTxt, uint &uiCharsCount) override;
	DGLE_RESULT DGLE_API GetVirtualFileSystemDescription(const char *pcVFSExtension, char *pcTxt, uint &uiCharsCount) override;

	DGLE_RESULT DGLE_API GetType(E_ENGINE_SUB_SYSTEM &eSubSystemType) override;

	IDGLE_BASE_IMPLEMENTATION(IMainFileSystem, INTERFACE_IMPL(IEngineSubSystem, INTERFACE_IMPL_END))
};