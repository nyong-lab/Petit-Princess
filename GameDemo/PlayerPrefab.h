#pragma once
#include <Entity.h>
class PlayerPrefab :
    public Entity
{
public:

	PlayerPrefab();
	PlayerPrefab(const std::shared_ptr<PlayerPrefab>& other);
	~PlayerPrefab();

	void Start();
	void SetPos(Vector3 pos);

private:
};

