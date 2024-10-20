#include "GameEngine_pch.h"
#include "StringUtils.h"


/// <summary>
/// wstring 을 string 으로 변환
/// </summary>
std::string StringUtils::ToString(std::wstring value)
{
	return std::string(value.begin(), value.end());
}

/// <summary>
/// string 을 wstring 으로 변환
/// </summary>
std::wstring StringUtils::ToWString(std::string value)
{
	return std::wstring(value.begin(), value.end());
}

/// <summary>
/// 문자열 str 에서 comp 를 찾아서 rep 로 교체하는 함수
/// 문자열을 수정하고 변경한다.
/// </summary>
/// <param name="str">대상 문자열</param>
/// <param name="comp">교체할 부분 문자열</param>
/// <param name="rep">comp를 교체할 새 부분 문자열</param>
void StringUtils::Replace(std::string& str, std::string comp, std::string rep)
{
	std::string temp = str;

	size_t startPos = 0;

	// find 함수는 실패시 npos 를 반환
	while ((startPos = temp.find(comp, startPos)) != std::string::npos)
	{
		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	str = temp;
}

/// <summary>
/// 문자열 str 에서 comp 를 찾아서 rep 로 교체하는 함수
/// 문자열을 수정하고 변경한다.
/// </summary>
/// <param name="str">대상 문자열</param>
/// <param name="comp">교체할 부분 문자열</param>
/// <param name="rep">comp를 교체할 새 부분 문자열</param>
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
/// 주어진 문자열이 특정 문자열로 시작하는지 여부를 확인하는 함수
/// </summary>
/// <param name="str">주어진 문자열</param>
/// <param name="comp">시작 지점 예상 문자열</param>
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
	// 첫 번째 공백 찾기
	size_t posSpace = name.find_first_of(" ");

	// 공백이 없는 경우에는 원래 문자열 반환
	if (posSpace == std::string::npos) 
	{
		return name;
	}

	// 키워드와 그 뒤의 공백을 제외한 부분을 반환
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

