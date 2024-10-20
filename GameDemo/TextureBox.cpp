#include "GameDemo_pch.h"
#include "TextureBox.h"

#include "MeshRenderer.h"

TextureBox::TextureBox()
	: Entity("TextureBoxEntity")
{

}

TextureBox::TextureBox(const std::shared_ptr<TextureBox>& other)
	: Entity(other)
{
}

TextureBox::~TextureBox()
{

}

void TextureBox::Start()
{
	// 렌더러 추가
	auto pRenderer = AddComponent<MeshRenderer>("TextureBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("TextureBoxMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::TextureBox);

	// 엔티티 Start 호출
	Entity::Start();
}
