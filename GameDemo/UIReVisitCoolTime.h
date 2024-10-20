#pragma once

#include <Entity.h>

/// <summary>
/// 상점 재방문 쿨타임 UI 클래스
/// 작성자 : 김동일
class UIReVisitCoolTime : public Entity
{
public:
	UIReVisitCoolTime();
	UIReVisitCoolTime(const std::shared_ptr<UIReVisitCoolTime>& other);
	~UIReVisitCoolTime();
public:
	virtual void Start() override;

};
