#include "GameEngine_pch.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Entity.h"
#include "CameraManager.h"
#include "../GameDemo/FoodTableScript.h"
#include "../GameDemo/InterKeyScript.h"
#include "Transform.h"
#include "CameraManager.h"


using namespace GraphicsTransfer;

MeshRenderer::MeshRenderer()
	: Renderer()
{

}

MeshRenderer::MeshRenderer(const std::string& name)
	: Renderer(name)
{
	
}


void MeshRenderer::Update(double dt)
{

}

void MeshRenderer::Finalize()
{

}

void MeshRenderer::Render(double dt)
{
	auto pMesh = GetOwner().lock()->GetComponent<MeshFilter>();
	if (!pMesh) return;

	ModelData modelData;

	auto transform = GetTransform();

	auto camera = GetOwner().lock()->GetCameraManager().lock()->GetMainCamera();

	CbTransform cbtransform;
	cbtransform.world = transform->GetWorldMatrix();
	cbtransform.cameraWorld = camera->GetCameraWorldMatrix();
	cbtransform.view = camera->GetViewMatrix();
	cbtransform.proj = camera->GetProjMatrix();
	cbtransform.viewProj = camera->GetViewProjectionMatrix();

	CbMaterial cbMaterial;
	auto mat = pMesh->GetMaterial();

	cbMaterial.useMap[0] = mat.useBaseColorMap;
	cbMaterial.useMap[1] = mat.useNormalMap;
	cbMaterial.useMap[2] = mat.useMetallicMap;
	cbMaterial.useMap[3] = mat.useRoughnessMap;
	cbMaterial.useMap[4] = mat.useAOMap;

	cbMaterial.baseColor = mat.baseColor;
	cbMaterial.metallic = mat.metallic;
	cbMaterial.roughness = mat.roughness;
	cbMaterial.AO = mat.AO;

	modelData.modelKey = pMesh->GetModelKey();
	modelData.transform = cbtransform;
	modelData.meterial = cbMaterial;
	
	// 컬링 여부 결정
	if(m_owner.lock()->IsCullingOffAll() 
		|| pMesh->m_cullingOff
		|| IsInFrustum(pMesh, camera))
		m_owner.lock()->GetGraphics().lock()->ObjectRender(modelData);
}

bool MeshRenderer::IsInFrustum(std::shared_ptr<MeshFilter> filter, const std::weak_ptr<CameraModule> camera)
{
	Vector2 box = filter->m_cullingBox;
	Vector3 thist = GetTransform()->GetWorldPosition();
	Vector2 thisCenter = Vector2(thist.x, thist.z);

	Vector2 cameraBox = Vector2(105,65); //임시용!!!!!!
	Vector3 camerat = camera.lock()->GetPosition();
	Vector2 cameraCenter = Vector2(camerat.x, camerat.z + 30);

	Vector2& posi = thisCenter;
	Vector2& posj = cameraCenter;
	float heighti = box.y * 0.5f;
	float heightj = cameraBox.y * 0.5f;
	float widthi = box.x * 0.5f;
	float widthj = cameraBox.x * 0.5f;

	//AABB Check
	if (fabs(posi.x - posj.x) <= (widthi + widthj)
		&& fabs(posi.y - posj.y) <= (heighti + heightj))
	{
		return true;
	}

	return false;
}

std::shared_ptr<Component> MeshRenderer::Clone() const
{
	std::shared_ptr<MeshRenderer> clone = std::make_shared<MeshRenderer>();


	return clone;
}
