#pragma once


/// <summary>
/// string 유틸리티 기능을 제공하는 클래스
/// 작성자 : 김동일
/// </summary>
class StringUtils
{
public:
	// string -> wstring  -  wstring -> string
	static std::wstring ToWString(std::string value);
	static std::string ToString(std::wstring value);

	// 문자열 교체
	static void Replace(std::string& str, std::string comp, std::string rep);
	static void Replace(std::wstring& str, std::wstring comp, std::wstring rep);

	// 특정 문자열로 시작하는지 여부 검사
	static bool StartsWith(std::string str, std::string comp);
	static bool StartsWith(std::wstring str, std::wstring comp);

	// m_ 제거
	// ex : m_test -> test
	static std::string EraseUnderbar(const std::string& name);

	// 공백 제거
	// ex : class Transform -> Transform
	static std::string EraseSpace(const std::string& name);

	// local 제거
	// ex : localPosition -> Position
	static std::string EraseLocal(const std::string& name);

	// 이름에서 원하는 스트링 찾아서 삭제
	// ex: Dongil.json -> Dongil
	static void RemoveSubstring(std::string& main, const std::string eraseStr);
	static void RemoveSubstring(std::wstring& main, const std::wstring eraseStr);
};

