#pragma once
#include "ErrorLogger.h"

//��ũ�� ����� ���� HRESULT hr�� �̸� ���ǵǾ� �־�� �Ѵ�.

#ifndef NDEBUG
#define HR(hrcall) ErrorLogger::GetInstance()->Set(); if( FAILED (hr = (hrcall) ) ) ErrorLogger::GetInstance()->GetMessagesHR(hr, __LINE__, __FILE__ )
#define DRAW_CALL(call) ErrorLogger::GetInstance()->Set(); (call); ErrorLogger::GetInstance()->GetMessages(__LINE__, __FILE__ )

#else
#define HR(hrcall) (hrcall)
#define DRAW_CALL(call) (call)
#endif
