#pragma once
#include <Entity.h>


/// <summary>
/// UI Senese
/// ���� ��ġ ������ ������ Ŭ����
/// �ۼ��� : �赿��
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
