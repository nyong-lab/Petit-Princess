#pragma once
#include <string>
#include <memory.h>
#include <vector>

/// <summary>
/// 2024.01.03
/// 매니저 클래스를 만들 추상 클래스
/// 작성 : 유승운
/// </summary>
class IManager
{
public:
	virtual ~IManager() = default;

	virtual void Initialize() abstract;
	virtual void Start() abstract;
	virtual void Update(double dt) abstract;
	virtual void Finalize() abstract;

private:
};