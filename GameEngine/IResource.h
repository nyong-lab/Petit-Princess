#pragma once

/// <summary>
/// 2024.01.16
/// ���ҽ� Ÿ���� �������̽��� ��ӽ�ų �߻� Ŭ����
/// �ۼ��� : ���¿�
/// </summary>

class IResource
{
public:
	virtual ~IResource() = default;

	virtual void SetName(std::string name) abstract;
	virtual std::string  GetName() const abstract;
	virtual void load(std::string path) abstract;

private:
};
