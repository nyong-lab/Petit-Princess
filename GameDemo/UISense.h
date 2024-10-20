#pragma once
#include <Entity.h>


/// <summary>
/// UI Senese
/// 상인 눈치 게이지 프리팹 클래스
/// 작성자 : 김동일
/// </summary>
class UISense : public Entity
{
public:
	UISense();
	UISense(const std::shared_ptr<UISense>& other);
	~UISense();
public:
	virtual void Start() override;
};
