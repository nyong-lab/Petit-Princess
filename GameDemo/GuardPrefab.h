#pragma once
#include <Entity.h>
#include "IGraphics.h"


/// <summary>
/// ��� ������
/// 2024.02.15
/// �ۼ� : ���¿�
/// </summary>
class GuardPrefab :
    public Entity
{
public:
    GuardPrefab();
    GuardPrefab(const std::shared_ptr<GuardPrefab>& other);
    ~GuardPrefab();

    void Start();

    void SetPatrolPath(Vector3 destination);
    void SetPos(Vector3 pos);

private:
	//�þ� �Ÿ�
	int m_range = 10;

    std::vector<Vector3> m_patrolPath;
};

