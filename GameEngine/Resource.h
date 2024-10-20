#pragma once
#include "IResource.h"


/// <summary>
/// 2024.01.19
/// ���� ������ ���ҽ��� ��ü�� ���� ��� Ŭ����
/// </summary>

class Resource : public IResource
{
public:
	Resource();
	Resource(std::string name) : m_name(name) {};
	virtual ~Resource() = default;

	virtual void load(std::string path) override;
	virtual void SetName(std::string name) override { m_name = name; };
	virtual std::string  GetName() const override { return m_name; };


protected:
	//���ҽ� �������� �� ������
	std::string m_name;
};
