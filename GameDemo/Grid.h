#pragma once
#include "Entity.h"
#include "IGraphics.h"

class CameraModule;

/// <summary>
/// Grid Prefab
/// 
/// 24.01.25
/// �ڿ���
/// 
/// 24.01.28
/// [����] �赿��
/// Entity::Start ȣ�� �߰�
/// </summary>
class Grid : public Entity
{
public:
	Grid();
	~Grid();

public:
	virtual void Start() override;

private:
	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<CameraModule> m_camera;
};

