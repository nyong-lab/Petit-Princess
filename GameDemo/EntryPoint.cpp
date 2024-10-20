#include "framework.h"

// 게임엔진 헤더
#include "GameProcess.h"
#include "DebugProcess.h"

// 메모리 누수 체크
#include <crtdbg.h>  

/// <summary>
/// 2023.12.20
/// 김동일 유승운
/// 게임 껍데기 시작!
/// 
/// 2023.01.24
/// 김동일 유승운 에디터 + 데모 합치기 (완)
/// </summary>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

#ifdef _DEBUG
	// 메모리 누수 잡자
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(94097);
#endif

#if _DEBUG
	std::unique_ptr<DebugProcess> gameProcess = std::make_unique<DebugProcess>(hInstance, nCmdShow);
#else
	std::unique_ptr<GameProcess> gameProcess = std::make_unique<GameProcess>(hInstance, nCmdShow);
#endif

	if (gameProcess->Initialize())
	{
		gameProcess->Loop();
	}

	gameProcess->Finalize();

	return 0;
}