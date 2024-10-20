#pragma once
#pragma once

#include <string>
#include <dxgidebug.h>

#ifdef _DEBUG
#define DEBUG_MODE
#endif

/// <summary>
/// 그래픽스 디버깅 로그를 띄우기 위한 싱글톤 객체
/// debug mode 일 때만 싱글톤으로 생성되어 작동한다.
/// </summary>
class ErrorLogger
{
public:
	static ErrorLogger* GetInstance()
	{
#ifdef DEBUG_MODE
		static ErrorLogger errorLogger;
		return &errorLogger;
#else
		//#error "GetInstance should not be called in Release mode."
		return nullptr; // 릴리스 모드에서는 사용 불가능하도록 처리
#endif
	}

private:
	~ErrorLogger() {};

public:
	void Initialize();

	static void Log(std::wstring message);
	static void Log(HRESULT hr, std::wstring message);
	void Set() noexcept;
	void GetMessages(int line = __LINE__, const char* file = __FILE__) const;
	void GetMessagesHR(HRESULT hr, int line = __LINE__, const char* file = __FILE__) const;

private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};