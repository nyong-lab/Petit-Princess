#pragma once
#pragma once

#include <string>
#include <dxgidebug.h>

#ifdef _DEBUG
#define DEBUG_MODE
#endif

/// <summary>
/// �׷��Ƚ� ����� �α׸� ���� ���� �̱��� ��ü
/// debug mode �� ���� �̱������� �����Ǿ� �۵��Ѵ�.
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
		return nullptr; // ������ ��忡���� ��� �Ұ����ϵ��� ó��
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