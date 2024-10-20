#include "GameEngine_pch.h"
#include "FileUtils.h"

FileUtils::FileUtils()
{

}

FileUtils::~FileUtils()
{
	if (m_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
}

/// <summary>
/// �б� �Ǵ� ���� ��忡 ���� ������ �����ϰų� ����, ���� ������ �����Ѵ�.
/// ����� ������ m_Handle �� ��� �ִ´�.
/// </summary>
void FileUtils::Open(std::wstring filePath, FileMode mode)
{
	if (mode == FileMode::Write)
	{
		m_Handle = ::CreateFile
		(
			filePath.c_str(), // ���� ���
			GENERIC_WRITE, // ���� ����
			0,
			nullptr,
			CREATE_ALWAYS, // ������ ������ �ִٸ� ���� ���� ���ٸ� ���� �������
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}
	else
	{
		 m_Handle = ::CreateFile
		(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,  // ������ �ִ� ��쿡�� �д´�. ������ ���� ��ȯ
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}

	assert(m_Handle != INVALID_HANDLE_VALUE);
}

void FileUtils::Close()
{
	if (m_Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
}

void FileUtils::Write(void* data, unsigned int dataSize)
{
	// ���� üũ ����
	unsigned int numOfBytes = 0;

	bool writeData = ::WriteFile(m_Handle, data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr);

	// ������ �����Ͱ� �ϳ��� ���ٸ� numOfBytes �� 0 ����Ʈ �� ���̰�,
	// numOfBytes �� 0 �̶�� false �� ��ȯ�ؼ� assert
	assert(writeData && "���� ���� �������� ���� �߻�");
}

void FileUtils::Write(const std::string& data)
{
	// ���ڿ� ���� ������ ���� ���Ͽ� ����.
	unsigned int size = (unsigned int)data.size();
	Write(size);

	if (data.size() == 0)
		return;

	// ���ڿ��� ���Ͽ� ����.
	Write((void*)data.data(), size);
}

void FileUtils::Read(void** data, unsigned int dataSize)
{
	// ���� üũ ����
	unsigned int numOfBytes = 0;

	BOOL readData = ::ReadFile(m_Handle, *data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr);

	// ���� �����Ͱ� �ϳ��� ���ٸ� numOfBytes �� 0 ����Ʈ �� ���̰�,
	// numOfBytes �� 0 �̶�� false �� ��ȯ�ؼ� assert
	assert(readData && "���� �б� �������� ���� �߻�");
}

void FileUtils::Read(std::string& data)
{
	unsigned int size = Read<unsigned int>();

	if (size == 0)
		return;
	
	char* temp = new char[size + 1];
	temp[size] = NULL;
	Read((void**)&temp, size);
	data = temp;
	delete[] temp;
}
