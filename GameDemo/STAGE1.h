#pragma once
#include <Scene.h>

class FoodTableScript;
class ShopKeeperFSM;


class STAGE1 :
    public Scene
{
public:
	STAGE1(std::wstring sceneName);

	void Start();
	void Update(double dt);

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

private:

	enum class Info
	{
		None = 0,

		Wall,		 //	�� 1
		AutoBush,	 // �ڵ��ν� 2
		InterBush,	 // �����ν� 3
		Potion,		 // ������ 4 
		store,		 // ���� 5
		Restaurant,  // ���� 6
		Plaza,		 // �м��� 7
		Guard,		 // ��� 8
		Player,		 // �÷��̾� 9 
		Streetlight, // ���ε� 10
		Tree,		 // ���� 11

		End
	};

	enum class DecoType
	{
		Grass, Tree1, Tree2, Candy, Table,
		Candle1, Candle2, Lamp, Flag
	};


	void CreateWall(int x, int y);
	void CreateAutoBush(int x, int y);
	void CreateInterBush(int x, int y);
	void CreatePotion(int x, int y);
	void CreateStore(int x, int y, int dir, int type);
	void CreateRestaurant(int x, int y, int dir, int type);
	void CreateFountain(int x, int y);
	void CreateGuard(int x, int y);
	void CreatePlayer(int x, int y);
	void CreateStreetLight(int x, int y);
	void CreateTree(int x, int y);
	
	void CreateMap(int info,int x, int y, int direction, int minigame);

	void CreateDeco(DecoType type, float x, float y, float yRotation = 0.f, float scale = 1.0f);


	std::vector<EntityPosData> m_shopData;
	std::vector<EntityPosData> m_guardData;

	// ����
	std::vector<std::shared_ptr<FoodTableScript>> m_tableScripts;
	std::vector<std::shared_ptr<ShopKeeperFSM>> m_shopkeeperFSMs;
};

