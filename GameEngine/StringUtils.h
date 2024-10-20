#pragma once


/// <summary>
/// string ��ƿ��Ƽ ����� �����ϴ� Ŭ����
/// �ۼ��� : �赿��
/// </summary>
class StringUtils
{
public:
	// string -> wstring  -  wstring -> string
	static std::wstring ToWString(std::string value);
	static std::string ToString(std::wstring value);

	// ���ڿ� ��ü
	static void Replace(std::string& str, std::string comp, std::string rep);
	static void Replace(std::wstring& str, std::wstring comp, std::wstring rep);

	// Ư�� ���ڿ��� �����ϴ��� ���� �˻�
	static bool StartsWith(std::string str, std::string comp);
	static bool StartsWith(std::wstring str, std::wstring comp);

	// m_ ����
	// ex : m_test -> test
	static std::string EraseUnderbar(const std::string& name);

	// ���� ����
	// ex : class Transform -> Transform
	static std::string EraseSpace(const std::string& name);

	// local ����
	// ex : localPosition -> Position
	static std::string EraseLocal(const std::string& name);

	// �̸����� ���ϴ� ��Ʈ�� ã�Ƽ� ����
	// ex: Dongil.json -> Dongil
	static void RemoveSubstring(std::string& main, const std::string eraseStr);
	static void RemoveSubstring(std::wstring& main, const std::wstring eraseStr);
};

