#pragma once
#include "IManager.h"
#include "EntityDefine.h"

union CollisionID
{
	struct
	{
		UINT IDcollder1;
		UINT IDcollder2;
	};
	ULONGLONG IDs;
};

/// <summary>
/// FixedUpdate()로 OnTriggerXXX OnCollisionXXX를 호출하는 매니저
/// 
/// Collision : Static-Dynamic / Dynamic-Static / Dynamic-Dynamic / Dynamic-Kinematic / Kinematic-Dynamic
/// Trigger 
/// 
/// Static : DynamicTrigger / KinematicTrigger
/// Dynamic : StaticTrigger / DynamicTrigger / KinematicTrigger
/// Kinematic : StaticTrigger / DynamicTrigger / KinematicTrigger
/// StaticTrigger : Dynamic / Kinematic / DynamicTrigger / KinematicTrigger
/// DynamicTrigger : Static / Dynamic / Kinematic / StaticTrigger / DynamicTrigger / KinematicTrigger
/// KinematicTrigger : Static / Dynamic / Kinematic / StaticTrigger / DynamicTrigger / KinematicTrigger
/// </summary>
class 
	SeoYoung : public IManager
{
public:
	CollisionManagerSeoYoung() = default;
	~CollisionManagerSeoYoung() = default;

	virtual void Initialize() override;
	virtual void Start() override {};

	void FixedUpdate(double fixed_dt);
	virtual void Update(double dt) override {};
	virtual void Finalize() override {};

public:
	void AddCollider(class BoxCollider* collider);
	void RemoveCollider(class BoxCollider* collider);

private:
	void SetLayerCollisionMatrix(LayerCollision layer1, LayerCollision layer2);
	void FixedUpdateLayer(LayerCollision layer1, LayerCollision layer2);

private:
	bool m_layerCollisionMatrix[static_cast<UINT>(LayerCollision::End)][static_cast<UINT>(LayerCollision::End)] = { 0, };
	std::vector<class BoxCollider*> m_boxColliders[static_cast<UINT>(LayerCollision::End)];
	std::map<ULONGLONG, bool> m_mapCollisionInfo;
	UINT m_colliderIDToAssign = 0;
};

