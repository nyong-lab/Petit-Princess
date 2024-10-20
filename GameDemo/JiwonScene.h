#pragma once
#include <Scene.h>

// 상점, 경비병 등의 초기 위치 노가다를 위한 이넘과 구조체
enum class EntityDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct EntityPosData
{
	Vector3 m_position;
	EntityDirection m_direction;
};

class JiwonScene : public Scene
{
public:
	JiwonScene(std::wstring sceneName);

	virtual void Start() override;
	void Update(double dt);

private:
	std::vector<EntityPosData> m_shopData;	// 상점 위치와 방향
	std::vector<EntityPosData> m_guardData;	// 경비병의 초기 위치와 방향
	std::vector<Vector3> m_bushData;		// 부쉬 위치
	std::vector<Vector3> m_potionData;		// 마법 물약 위치

};