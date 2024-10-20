#pragma once
#include "Entity.h"
#include "IGraphics.h"

class CameraModule;

/// <summary>
/// Texture Box Prefab
/// �ؽ��� �� �÷��̾� �׽�Ʈ��
/// 
/// 24.01.26
/// �ڿ���
/// 
/// 24.01.28
/// [����] �赿��
/// Entity::Start ȣ�� �߰�
/// </summary>
class TextureBox : public Entity
{
public:
	TextureBox();
	TextureBox(const std::shared_ptr<TextureBox>& other);
	~TextureBox();

public:
	virtual void Start() override;
};
