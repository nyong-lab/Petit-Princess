#pragma once

#include <Entity.h>

/// <summary>
///  ���� ������ UI Ŭ����
/// �ۼ��� : �赿��
class UIDeliciousFood : public Entity
{
public:
	UIDeliciousFood();
	UIDeliciousFood(const std::shared_ptr<UIDeliciousFood>& other);
	~UIDeliciousFood();
public:
	virtual void Start() override;

};