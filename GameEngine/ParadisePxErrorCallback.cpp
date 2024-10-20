#include "GameEngine_pch.h"
#include "ParadisePxErrorCallback.h"

#include <foundation/PxAssert.h>
#include <foundation/PxString.h>
#include <foundation/PxThread.h>
#include <extensions/PxDefaultErrorCallback.h>

using namespace physx;

physx::ParadisePxErrorCallback::ParadisePxErrorCallback()
{

}

physx::ParadisePxErrorCallback::~ParadisePxErrorCallback()
{

}

void physx::ParadisePxErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	const char* errorCode = NULL;

	switch (code)
	{
	case PxErrorCode::eNO_ERROR:
		errorCode = "no error";
		break;
	case PxErrorCode::eINVALID_PARAMETER:
		errorCode = "invalid parameter";
		break;
	case PxErrorCode::eINVALID_OPERATION:
		errorCode = "invalid operation";
		break;
	case PxErrorCode::eOUT_OF_MEMORY:
		errorCode = "out of memory";
		break;
	case PxErrorCode::eDEBUG_INFO:
		errorCode = "info";
		break;
	case PxErrorCode::eDEBUG_WARNING:
		errorCode = "warning";
		break;
	case PxErrorCode::ePERF_WARNING:
		errorCode = "performance warning";
		break;
	case PxErrorCode::eABORT:
		errorCode = "abort";
		break;
	case PxErrorCode::eINTERNAL_ERROR:
		errorCode = "internal error";
		break;
	case PxErrorCode::eMASK_ALL:
		errorCode = "unknown error";
		break;
	}

	PX_ASSERT(errorCode);
	if (errorCode)
	{
		std::string mes = "[FILE] " + std::string(file) + "\n";
		mes += "[LINE] " + line; mes += "\n";
		mes += std::string(errorCode) + message;

		MessageBoxA(NULL, mes.c_str(), "PhysX Error", MB_OK);
		Sleep(100000);

		// in debug builds halt execution for abort codes
		//PX_ASSERT(e != PxErrorCode::eABORT);
		assert(code != PxErrorCode::eABORT && L"치명적인 문제 발생");

		// in release builds we also want to halt execution 
		// and make sure that the error message is flushed  
// 		while (code == PxErrorCode::eABORT)
// 		{
// 			MessageBoxA(NULL, mes.c_str(), "GFX ERROR", MB_OK);
// 			PxThread::sleep(1000);
// 		}
	}
}
