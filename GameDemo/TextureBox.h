#pragma once
#include "Entity.h"
#include "IGraphics.h"

class CameraModule;

/// <summary>
/// Texture Box Prefab
/// 텍스쳐 및 플레이어 테스트용
/// 
/// 24.01.26
/// 박연하
/// 
/// 24.01.28
/// [수정] 김동일
/// Entity::Start 호출 추가
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
