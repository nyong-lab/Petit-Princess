#pragma once
#include <string>
#include <memory.h>
#include <vector>

/// <summary>
/// 2024.01.03
/// �Ŵ��� Ŭ������ ���� �߻� Ŭ����
/// �ۼ� : ���¿�
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