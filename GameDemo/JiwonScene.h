#pragma once
#include <Scene.h>

// ����, ��� ���� �ʱ� ��ġ �밡�ٸ� ���� �̳Ѱ� ����ü
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
	std::vector<EntityPosData> m_shopData;	// ���� ��ġ�� ����
	std::vector<EntityPosData> m_guardData;	// ����� �ʱ� ��ġ�� ����
	std::vector<Vector3> m_bushData;		// �ν� ��ġ
	std::vector<Vector3> m_potionData;		// ���� ���� ��ġ

};