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
	// ������ �߰�
	auto pRenderer = AddComponent<MeshRenderer>("TextureBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("TextureBoxMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::TextureBox);

	// ��ƼƼ Start ȣ��
	Entity::Start();
}
