#pragma once
#include "ErrorLogger.h"

//매크로 사용을 위해 HRESULT hr이 미리 정의되어 있어야 한다.

#ifndef NDEBUG
#define HR(hrcall) ErrorLogger::GetInstance()->Set(); if( FAILED (hr = (hrcall) ) ) ErrorLogger::GetInstance()->GetMessagesHR(hr, __LINE__, __FILE__ )
#define DRAW_CALL(call) ErrorLogger::GetInstance()->Set(); (call); ErrorLogger::GetInstance()->GetMessages(__LINE__, __FILE__ )

#else
#define HR(hrcall) (hrcall)
#define DRAW_CALL(call) (call)
#endif
