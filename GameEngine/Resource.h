#pragma once
#include "IResource.h"


/// <summary>
/// 2024.01.19
/// 내가 관리할 리소스의 객체의 구현 상속 클래스
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
	//리소스 공통으로 들어간 변수들
	std::string m_name;
};
