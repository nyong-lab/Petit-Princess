#include "GameEngine_pch.h"
#include "StringUtils.h"


/// <summary>
/// wstring �� string ���� ��ȯ
/// </summary>
std::string StringUtils::ToString(std::wstring value)
{
	return std::string(value.begin(), value.end());
}

/// <summary>
/// string �� wstring ���� ��ȯ
/// </summary>
std::wstring StringUtils::ToWString(std::string value)
{
	return std::wstring(value.begin(), value.end());
}

/// <summary>
/// ���ڿ� str ���� comp �� ã�Ƽ� rep �� ��ü�ϴ� �Լ�
/// ���ڿ��� �����ϰ� �����Ѵ�.
/// </summary>
/// <param name="str">��� ���ڿ�</param>
/// <param name="comp">��ü�� �κ� ���ڿ�</param>
/// <param name="rep">comp�� ��ü�� �� �κ� ���ڿ�</param>
void StringUtils::Replace(std::string& str, std::string comp, std::string rep)
{
	std::string temp = str;

	size_t startPos = 0;

	// find �Լ��� ���н� npos �� ��ȯ
	while ((startPos = temp.find(comp, startPos)) != std::string::npos)
	{
		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	str = temp;
}

/// <summary>
/// ���ڿ� str ���� comp �� ã�Ƽ� rep �� ��ü�ϴ� �Լ�
/// ���ڿ��� �����ϰ� �����Ѵ�.
/// </summary>
/// <param name="str">��� ���ڿ�</param>
/// <param name="comp">��ü�� �κ� ���ڿ�</param>
/// <param name="rep">comp�� ��ü�� �� �κ� ���ڿ�</param>
void StringUtils::Replace(std::wstring& str, std::wstring comp, std::wstring rep)
{
	std::wstring temp = str;

	size_t startPos = 0;
	while ((startPos = temp.find(comp, startPos)) != std::wstring::npos)
	{
		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	str = temp;
}

/// <summary>
/// �־��� ���ڿ��� Ư�� ���ڿ��� �����ϴ��� ���θ� Ȯ���ϴ� �Լ�
/// </summary>
/// <param name="str">�־��� ���ڿ�</param>
/// <param name="comp">���� ���� ���� ���ڿ�</param>
/// <returns></returns>
bool StringUtils::StartsWith(std::string str, std::string comp)
{
	std::wstring::size_type index = str.find(comp);
	if (index != std::wstring::npos && index == 0)
		return true;

	return false;
}

bool StringUtils::StartsWith(std::wstring str, std::wstring comp)
{
	std::wstring::size_type index = str.find(comp);
	if (index != std::wstring::npos && index == 0)
		return true;

	return false;
}
 
std::string StringUtils::EraseUnderbar(const std::string& name)
{
	if (name[1] == '_')
	{
		return name.substr(2, name.size() - 1);
	}

	return name;
}
std::string StringUtils::EraseSpace(const std::string& name)
{
	// ù ��° ���� ã��
	size_t posSpace = name.find_first_of(" ");

	// ������ ���� ��쿡�� ���� ���ڿ� ��ȯ
	if (posSpace == std::string::npos) 
	{
		return name;
	}

	// Ű����� �� ���� ������ ������ �κ��� ��ȯ
	return name.substr(posSpace + 1);
}

std::string StringUtils::EraseLocal(const std::string& name)
{
	const std::string localKeyword = "local";

	size_t posLocal = name.find(localKeyword);
	if (posLocal == std::string::npos)
	{
		return name;
	}

	std::string tolowerName = name.substr(posLocal + localKeyword.length());
	tolowerName[0] = tolower(tolowerName[0]);

	return tolowerName;
}

void StringUtils::RemoveSubstring(std::string& main, const std::string eraseStr)
{
	size_t pos = main.find(eraseStr);
	if (pos != std::string::npos)
	{
		main.erase(pos, eraseStr.length());
	}
}

void StringUtils::RemoveSubstring(std::wstring& main, const std::wstring eraseStr)
{
	size_t pos = main.find(eraseStr);
	if (pos != std::wstring::npos)
	{
		main.erase(pos, eraseStr.length());
	}
}

