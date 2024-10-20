#pragma once

/// <summary>
/// 2024.01.16
/// 리소스 타입의 인터페이스를 상속시킬 추상 클래스
/// 작성자 : 유승운
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
