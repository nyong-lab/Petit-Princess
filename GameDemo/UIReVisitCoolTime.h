#pragma once

#include <Entity.h>

/// <summary>
/// ���� ��湮 ��Ÿ�� UI Ŭ����
/// �ۼ��� : �赿��
class UIReVisitCoolTime : public Entity
{
public:
	UIReVisitCoolTime();
	UIReVisitCoolTime(const std::shared_ptr<UIReVisitCoolTime>& other);
	~UIReVisitCoolTime();
public:
	virtual void Start() override;

};
