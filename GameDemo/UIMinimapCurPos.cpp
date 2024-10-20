#include "GameDemo_pch.h"
#include "UIMinimapCurPos.h"

#include "UIRenderer.h"

 UIMinimapCurPos::UIMinimapCurPos()
	 : Entity("UIMinimapCurPos")
{

}

 UIMinimapCurPos::UIMinimapCurPos(const std::shared_ptr<UIMinimapCurPos>& other)
	 : Entity(other)
 {

 }

 UIMinimapCurPos::~UIMinimapCurPos()
 {

 }

 void UIMinimapCurPos::Start()
 {
	 auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	 auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	 pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_MinimapCurPos);

	 // 엔티티 Start 호출
	 Entity::Start();
 }
