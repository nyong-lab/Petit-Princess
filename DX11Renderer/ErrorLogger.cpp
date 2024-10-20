#include "renderpch.h"
#include "ErrorLogger.h"
#include <comdef.h>
#include "dxerr.h"
#pragma comment(lib, "dxguid.lib")


void ErrorLogger::Initialize()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// load the dll that contains the function DXGIGetDebugInterface 
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
		assert(false && L"dxgidebug 함수 dll을 불러오지 못함");

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
		assert(false && L"dxgidebug 함수 dll을 불러오지 못함");

	HRESULT hr = DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue);
	if (FAILED(hr))
		assert(false && L"dxgi 디버그 인터페이스 생성 실패");
}

void ErrorLogger::Log(std::wstring message)
{
	std::wstring errorMessage = L"Error : " + message;
	MessageBox(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error : " + message + L"\n" + error.ErrorMessage();
	MessageBox(NULL, error_message.c_str(), L"Error", MB_ICONERROR);

	// 다음과 같이 사용할 수 있다
	//ErrorLogger::Log(E_INVALIDARG, L"얍");
}

void ErrorLogger::Set() noexcept
{
	// set the index (next) so that the next all to GetMessages()
	// will only get errors generated after this call
	next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}


void ErrorLogger::GetMessages(int line /*= __LINE__*/, const char* file /*= __FILE__*/) const
{
	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		//get the size of message i in bytes
		hr = pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
		if (FAILED(hr)) assert(false && L"dxgi인포 큐에서 메세지 받아오기 실패");

		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		//get the message and push its description into the vector
		hr = pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
		if (FAILED(hr)) assert(false && L"dxgi인포 큐에서 메세지 받아오기 실패");
		messages.emplace_back(pMessage->pDescription);
	}

	if (!messages.empty())
	{
		//에러 메세지 출력
		std::string mes;
		mes += "[FILE]" + std::string(file);
		mes += "\n[LINE]"; mes += std::to_string(line); mes += "\n\n";

		for (auto& m : messages)
		{
			mes += m + "\n";
		}

		MessageBoxA(NULL, mes.c_str(), "GFX ERROR", MB_OK);
	}
}

void ErrorLogger::GetMessagesHR(HRESULT hr, int line /*= __LINE__*/, const char* file /*= __FILE__*/) const
{
	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		//get the size of message i in bytes
		hr = pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
		if (FAILED(hr)) assert(false && L"dxgi인포 큐에서 메세지 받아오기 실패");

		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		//get the message and push its description into the vector
		hr = pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
		if (FAILED(hr)) assert(false && L"dxgi인포 큐에서 메세지 받아오기 실패");
		messages.emplace_back(pMessage->pDescription);
	}

	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));

	std::ostringstream oss;
	oss << "Graphics Exception" << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << hr
		<< std::dec << " (" << (unsigned long)hr << ")" << std::endl
		<< "[Error String] " << DXGetErrorStringA(hr) << std::endl
		<< "[Description] " << buf << std::endl;

	std::string mes;
	if (!messages.empty())
	{
		//에러 메세지 출력
		for (auto& m : messages)
			mes += m + "\n";

		oss << "\n[Error Info]\n" << mes << std::endl << std::endl;
	}

	oss << "[File]" << file << "\n"
		<< "[Line]" << line;
	mes = oss.str();

	MessageBoxA(NULL, mes.c_str(), "GFX ERROR", MB_OK);
}