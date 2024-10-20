#pragma once
#include <Entity.h>
#include "IGraphics.h"


/// <summary>
/// 경비병 프리팹
/// 2024.02.15
/// 작성 : 유승운
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
	//시야 거리
	int m_range = 10;

    std::vector<Vector3> m_patrolPath;
};

