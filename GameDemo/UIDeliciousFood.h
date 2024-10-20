#pragma once

#include <Entity.h>

/// <summary>
///  맛집 아이콘 UI 클래스
/// 작성자 : 김동일
class UIDeliciousFood : public Entity
{
public:
	UIDeliciousFood();
	UIDeliciousFood(const std::shared_ptr<UIDeliciousFood>& other);
	~UIDeliciousFood();
public:
	virtual void Start() override;

};