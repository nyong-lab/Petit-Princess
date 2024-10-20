#pragma once
/// <summary>
/// 2024.01.02
///	���𰡸� ���� Builder�� �������̽��� ���� �߻�Ŭ����
/// �ۼ� : ���¿�
/// </summary>
/// <typeparam name="T">���� ��ü Ÿ��</typeparam>
template<typename T>
class IBuilder
{
public:
	virtual ~IBuilder() {}

	virtual std::shared_ptr<T> Create() abstract;
};