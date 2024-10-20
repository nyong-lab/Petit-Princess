#include "GameDemo_pch.h"
#include "InspectorGui.h"
#include "Entity.h"
#include "Transform.h"
#include "imgui_internal.h"

#include "IGraphics.h"
#include "CameraModule.h"
#include "CameraManager.h"
#include "PathManager.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

#include "PlayerFSM.h"
#include "GuardFSM.h"
#include "ShopKeeperFSM.h"

#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "UIRenderer.h"

#include "MiniGameDefine.h"
#include "GraphicsTransfer.h"

#include "FoodTableScript.h"
#include "InterKeyScript.h"
#include "ShopKeeperDoubtScript.h"
#include "MerchantUI.h"
#include "ShopKeeperCallingScript.h"

using namespace GraphicsTransfer;

InspectorGui::InspectorGui()
	: m_entityNameBuffer{ 0 }
	, m_deltaTime(0)
	, m_selectBeginCombo(false)
	, m_componentComboSearchBuffer{ 0 }
	, m_wstringBuffer{ 0 }
	, m_enumComboSerchBuffer{ 0 }
{

}

void InspectorGui::Initialize(
	std::weak_ptr<IGraphics> graphics,
	std::weak_ptr<SceneManager> sceneManager,
	std::weak_ptr<CameraManager> cameraManager,
	std::weak_ptr<ResourceManager> gameResourceManager)
{
	m_graphics = graphics;
	m_cameraManager = cameraManager;
	m_sceneManager = sceneManager;
	m_gameResourceManager = gameResourceManager;

	TypeInfoManager* manager = TypeInfoManager::GetInstance();
	auto& typeInformations = manager->GetTypeInfomations();

	for (auto& iter : typeInformations)
	{
		TypeTag typeTag = iter.second.typeInfo->GetTypeTag();

		if (typeTag != TypeTag::Component)
		{
			continue;
		}

		if (iter.first != "class Transform")
		{
			if (iter.second.typeInfo->GetSuper())
			{
				if (iter.second.typeInfo->GetSuper()->GetName() == "Script")
				{
					std::string scriptName = iter.first;
					m_scriptStringList.push_back(scriptName);
				}
				else
				{
					std::string componentName = iter.first;
					m_componentStringList.push_back(componentName);
				}
			}
		}
	}

	if (m_componentStringList.size() == 0)
	{
		m_componentStringList.push_back("No components have been reflected.");
	}
	if (m_scriptStringList.size() == 0)
	{
		m_scriptStringList.push_back("No components have been reflected.");
	}

	/// TODO::���� - ���� ���� ��� 5
	/// ���÷����ϴ� enum class Ÿ���� �þ �� ���� �ϵ��ڵ��� �ؾ��Ѵ�. 
	/// ���� ���� ��� 1�� ����� ���� ��� �ް��ִµ� �����غ��� 
	/// ��� Ÿ�Ժ��� TypeInfo Ŭ������ ��ӹ޴� Ŭ������ ���� TypeInfoMacro���� TypeInfo ��ü�� ����� ���� �ƴ϶�
	/// ���� ��ũ�� �κп��� TypeCheck �������� �ϴ� ��ũ�θ� ������ Type���� Ŭ������ ������ָ� ���?
	/// 
	/// ����
	/// ����
	/// 


	/// ������Ʈ enum class �� �߰��� �� ���� �÷�����.

	/// BodyType
	m_enumClassStringList["BodyType"] = { "Static", "Dynamic" };

	/// ModelKey
	//m_enumClassStringList["GraphicsTransfer::ModelKey"] = { "None",
	//	// Character -----------------------
	//	"Player", "Patrol", "Merchant",
	//	// Store ---------------------------
	//	"BagleStore", "BaguettePointStore", "CupCakeStore", "CrabStore", "AppleStore", "HamStore",
	//	"WaterMelonStore", "FishStore", "SausageStore" , "BaguetteWheelStore" , "BananaStore",
	//	// Food ----------------------------
	//	"Meat", "Seafood" , "Bread" , "Fruit" , "Dessert",
	//	// Prop ----------------------------
	//	"Floor", "Wall", "Box1", "Box2", "Tree1", "Tree2", "Barrel", "Bush", "Fountain", "Grass", "Lamp", "Potion", "FoodTable"
	//};

	m_enumClassStringList["GraphicsTransfer::ModelKey"] = { "None",
		// Primitive ----------
		"Grid", "TextureBox", "RedBox", "YellowBox", "PinkBox", "WireFrameBox","WireFrameRay",
		"WireFrameSphere", "Swablu","SfBox", "Erika", "SampleTable", "SampleFood", "SampleStore",
		// Character -------------
		"Player", "Patrol", "Merchant",
		// Prop ------------------
		"Floor", "Wall" , "Box1" , "Box2" , "Tree1","Tree2", "Barrel" , "Bush" , 
		"Fountain" , "Grass","Lamp","Potion","FoodTable",
		// Building --------------
		"BagleStore", "BaguettePointStore", "CupCakeStore", "CrabStore", "AppleStore", "CartoonMeatStore",
		"HamStore", "WaterMelonStore", "FishStore", "SquidStore", "CakeStore",
		"SausageStore", "BaguetteWheelStore", "BananaStore",
		// Food ------------------
		"Meat", "Seafood" , "Bread" , "Fruit" , "Dessert",
		// QTE -------------------
		"QTE_InteractiveKey" , "QTE_MiniGame",
		// UI --------------------
		"UI", "UIGauge", "UI_Timer", "UIHungry", "UI_Main", "UI_Sense",
		"UI_Score", "UI_Minimap", "UI_IngamePopUp", "UI_ReVisitCoolTime", "UI_DeliciousFood",
		"UI_MinimapCurPos","UI_Merchant", "UI_EatSpot",
		// Prop ------------------
		"Candy", "Table", "Flag",
		// ��¿ �� ����.
		"UI_MiniMapIcon", "UI_Skill","UI_Black", "Candle1", "Candle2", "UI_IngameInterKey"
	};

	/// FoodType
	m_enumClassStringList["FoodType"] = { "None", "Bagle", "BaguettePoint", "CupCake" , "Crab" , "Apple"
	, "Ham" , "WaterMelon" , "Fish" , "Sausage" , "BaguetteWheel", "Banana" };

	// �޽����� ���� ���԰� Ǫ�����̺�� ���� ģ������ ��Ʈȭ ���Ѽ� �̴ϰ��� Ÿ���� �� �ʿ䰡 ��������.
	/*/// MiniGameType
	m_enumClassStringList["MiniGameType"] = { "None", "Bagle", "BaguettePoint", "CupCake" , "Crab" , "Apple" , "CartoonMeat"
	, "Ham" , "WaterMelon" , "Fish" , "Squid" , "Cake" , "Sausage", "BaguetteWheel", "Banana" };*/

	// ��ƼƼ enum class �� ���÷����� ���ϰ� ���� �����Ѵ�.
	/*m_enumClassStringList["Layers"] = { "None", "Background", "Player", "Monster", "Trap", "Object", "UI" };
	m_enumClassStringList["LayerCollision"] = { "Default", "UI", "Ground" , "Player", "Interactable", "Wall", "Enemy", "Coin", "Heart" };*/

	// �ؽ�Ʈ ������ �ѱ۰� ImGui �ѱ� �̽��� ���� �ؽ�Ʈ�� ���Ϸ� ���� �����͸� ���� ���� �������� 
	// �ؽ�Ʈ �������� �ؽ�Ʈ�� ������ �� �ֵ����ϱ� ���� �ؽ�Ʈ ���� ����Ʈ �������� 
	GetTextFileList();
}

void InspectorGui::Update(std::weak_ptr<Entity> selectEntity, double dt)
{
	m_deltaTime = dt;

	if (!m_isShowProjectPrefab)
		m_selectEntity = selectEntity;

	if (m_isShowInspectorGui)
	{
		ShowInspector();
	}
}

void InspectorGui::ShowInspector()
{

	ImGui::Begin("Inspector", &m_isShowInspectorGui);

	if (m_selectEntity.lock())
	{
		ReadEntityInformation();
	}

	ImGui::End();
}

void InspectorGui::ReadEntityInformation()
{
	// ������ ������� ���� �˸��� ����
	if (m_isShowProjectPrefab)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, m_projectPrefabColor);
		ImGui::Text("--------------------- Prefab Entity ---------------------");
		ImGui::TextWrapped("It's prefab data here, so changing the status can't make any difference.");
		ImGui::TextWrapped("Simply put, the status window control means nothing.");

		ImGui::PopStyleColor();

		ImGui::Spacing();
		ImGui::Spacing();
	}

	// ��ƼƼ �̸� ����
	ChangeEntityName();

	// ���̾� ����
	SelectLayer();

	// �浹 ���̾� ����
	SelectLayerCollision();

	// ��ƼƼ Ȱ��ȭ ���� üũ �ڽ�
	ActiveCheckBox();

	// ��ƼƼ �ø� üũ �ڽ�
	CullingEntityCheckBox();

	// ��ƼƼ ���� ����
	IsRenderEntity();

	if (!m_isShowProjectPrefab)
	{
		// ��ƼƼ ID , Children ��
		ImGui::Text("Entity ID : %u", m_selectEntity.lock()->GetID());
		ImGui::SameLine();
		ImGui::TextWrapped("Children: %i", m_selectEntity.lock()->GetComponent<Transform>()->GetChildren().size());
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	// ������Ʈ�� ���� ��� (Ʈ������ ���� ���. Destroy ��ư������ �Ž�����.)
	const auto& transform = m_selectEntity.lock()->GetComponent<Transform>();
	{
		m_currentViewComponent = transform;

		if (m_currentViewComponent.lock() == nullptr)
		{
			assert(!"������Ʈ�� ����� �߰����� �ʾҽ��ϴ�. Clone�Լ��� AddComponent �� Ȯ�����ּ���");
		}

		const TypeInfo& typeInfo = m_currentViewComponent.lock()->GetTypeInfo();

		ReadComponentInformation(typeInfo);
	}

	// ������Ʈ�� ���� �ε��� 
	// ������� �ʴٸ� ����� �ִٴ� ��
	std::type_index eraseIndex = MakeTypeIndexEmpty();

	// ������Ʈ�� ���� ���
	auto& components = m_selectEntity.lock()->GetComponents();
	for (auto& iter : components)
	{
		m_currentViewComponent = iter.second;

		const TypeInfo& typeInfo = m_currentViewComponent.lock()->GetTypeInfo();
		std::string componentName = typeInfo.GetName();

		if (componentName == "Transform")
		{
			continue;
		}

		if (m_currentViewComponent.lock() == nullptr)
		{
			assert(!"������Ʈ�� ����� �߰����� �ʾҽ��ϴ�. Clone�Լ��� AddComponent �� Ȯ�����ּ���");
		}

		ReadComponentInformation(typeInfo);

		//������Ʈ ���� (Ʈ������ ����)
		if (componentName == "Transform")
		{
			continue;
		}

		ImGui::SameLine();

		std::string deleteButton = "Destroy##";
		deleteButton += componentName;

		// Gui ������ ������ �� ��ǥ x - ���ڿ� ������ x ���� 
		// ImGui Cursor ��ǥ�� ����.
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Destory##").x);

		// �ش� ��ǥ�� ��ư ����
		std::type_index index = iter.first;
		if (ImGui::Button(deleteButton.c_str()))
		{
			eraseIndex = index;
		}
	}

	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	// ������Ʈ �߰�
	AddComponentCombo();

	// ������ ������Ʈ ����
	if (!IsTypeIndexEmpty(eraseIndex))
	{
		const auto& component = m_selectEntity.lock()->GetComponent(eraseIndex);

		m_selectEntity.lock()->DestroyComponent(eraseIndex);

		// Ư�� ������Ʈ�� � ������Ʈ�� �Բ� �־�߸� �ɷ��� ����� ģ���� �ִ�. (EX. �޽� �������� �޽� ����)
		// ������ 2�� ��ư ���������� �� ���� ������Ű�� ���� ��ƿ �ڵ� ����

		const TypeInfo& typeInfo = component->GetTypeInfo();
		std::string componentName = typeInfo.GetFullName();

		TypeInfoManager* manager = TypeInfoManager::GetInstance();
		TypeInfo* typeInfoMate = nullptr;
		std::type_index index = MakeTypeIndexEmpty();

		// ���� �� ������Ʈ�� �޽� ��������
		if (componentName == "class MeshRenderer")
		{
			index = manager->GetTypeIndex("class MeshFilter");

			// ¦���� �޽� ���͸� ������ �ִٸ� ���� ���� �����ش�.
			if (m_selectEntity.lock()->HasComponent(index))
			{
				m_selectEntity.lock()->DestroyComponent(index);
			}
		}

		/// UIRenderer ������ �ּ�
		//// ���� �� ������Ʈ�� �޽� ���Ͱ�
		//else if (componentName == "class MeshFilter")
		//{
		//	index = manager->GetTypeIndex("class MeshRenderer");

		//	// ¦���� �޽� ���͸� ������ �ִٸ� ���� ���� �����ش�.
		//	if (m_selectEntity.lock()->HasComponent(index))
		//	{
		//		m_selectEntity.lock()->DestroyComponent(index);
		//	}
		//}
	}

	ImGui::Spacing();
	ImGui::Spacing();
}

void InspectorGui::ReadComponentInformation(const TypeInfo& typeInfo)
{
	std::string name = typeInfo.GetName();
	auto& properties = typeInfo.GetProperties();

	// boxcollider�� ��ӹ޴� collider Ŭ������ ��������� ����
	if (name == "BoxCollider" || name == "CapsuleCollider" || name == "SphereCollider" || name == "PlaneCollider")
	{
		m_currentChildComponentName = name;
		const TypeInfo* superTypeInfo = typeInfo.GetSuper();
		ReadComponentInformation(*superTypeInfo);

	}

	//if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))

	if (name == "RayCaster")
		ImGui::PushID(m_currentViewComponent.lock()->GetUID());

	if (ImGui::TreeNode(name.c_str()))
	{
		for (auto& property : properties)
		{
			if (property->IsShowGui())
			{

				ReadPropertyInformation(property);
			}
			else
			{
				continue;
			}
		}

		if (properties.size() == 0)
		{
			// ���÷����� ������ ���ų� ī�װ��� �´� �Լ��� �������� �ʾ���
			// c++20 u8�� ImGui ȣȯ �ȵȴ�.
			ImGui::Spacing();
			ImGui::TextWrapped("No variables were reflected or function was not implemented for the category");
			ImGui::Spacing();
		}

		ImGui::TreePop();
	}

	if (name == "RayCaster")
		ImGui::PopID();
}

void InspectorGui::ReadPropertyInformation(const Property* property)
{
	switch (property->GetPropertyTypeCategory())
	{
	case PropertyTypeCategory::Primitive:
	{
		ReadPrimitiveType(property);
		break;
	}
	case PropertyTypeCategory::String:
	{
		ReadPrimiteveStringType(property);
		break;
	}
	case PropertyTypeCategory::Vector:
	{
		ReadSTD_VectorType(property);
		break;
	}
	case PropertyTypeCategory::Map:
	{
		ReadComplexType(property);

		break;
	}
	case PropertyTypeCategory::UnorderedMap:
	{
		ReadComplexType(property);
		break;
	}
	case PropertyTypeCategory::Class:
	{
		if (property->GetName() == "m_pbrSet")
		{
			ImGui::Spacing();
			ImGui::Spacing();

			m_pbrSet = static_pointer_cast<MeshFilter>(m_currentViewComponent.lock())->GetPBRSet();
			ReadComponentInformation(property->GetTypeInfo());
		}
		else
		{
			ReadComplexType(property);
		}

		break;
	}
	case PropertyTypeCategory::EnumClass:
	{
		ReadEnumClassType(property);
		break;
	}
	default:
	{
		assert(false, R"(Tpye�� ���� ������ �����ϴ�.
			���÷����� Ÿ�Կ� ���ؼ� PropertyRegister �б�� ī�װ��� �߰��� �������ϰų�,
			���� switch case�� �÷��ּ���)");
		break;
	}
	}
}

void InspectorGui::ReadPrimitiveType(const Property* property)
{
	std::string fullTypeName = property->GetTypeInfo().GetFullName();

	std::string fureName = property->GetName();
	fureName = StringUtils::EraseUnderbar(fureName);

	if (fullTypeName == "struct DirectX::SimpleMath::Vector3")
	{
		fureName = StringUtils::EraseLocal(fureName);

		// ImGui ���� SimpleMath�� ���Խ�Ű�� �浹����
		// ����� ���� �Լ��� ������ ������
		Vector3 vector = property->Get<Vector3>(m_currentViewComponent.lock().get()).GetValue();
		float fArray[] = { vector.x, vector.y, vector.z };

		// light ���� ������� drag float �� �ƴ϶� ���� ���� �ٲ۴�.
		if (fureName == "color")
		{
			if (ImGui::ColorEdit3(fureName.c_str(), fArray, 0.001f))
			{
				DirectX::SimpleMath::Vector3 setVector = { fArray[0], fArray[1], fArray[2] };
				property->Set<Vector3>(m_currentViewComponent.lock().get(), setVector);
			}
		}
		else if (fureName == "attenuation" || fureName == "unitDirOffset")
		{
			if (ImGui::DragFloat3(fureName.c_str(), fArray, 0.001f))
			{
				DirectX::SimpleMath::Vector3 setVector = { fArray[0], fArray[1], fArray[2] };
				property->Set<Vector3>(m_currentViewComponent.lock().get(), setVector);
			}
		}
		else
		{
			if (ImGui::DragFloat3(fureName.c_str(), fArray, 0.1f))
			{
				DirectX::SimpleMath::Vector3 setVector = { fArray[0], fArray[1], fArray[2] };
				property->Set<Vector3>(m_currentViewComponent.lock().get(), setVector);

				if (fureName == "position" || fureName == "scale")
				{
					m_selectEntity.lock()->GetComponent<Transform>()->Update(m_deltaTime);
					m_selectEntity.lock()->GetComponent<Transform>()->SetIsUpdated(false);
				}

				CheckColliderTypeAndInvokeFunction(property);
			}
		}

		/// Vector3 ���� ��ư
		if (fureName == "position" || fureName == "scale" || fureName == "color" || fureName == "attenuation" || fureName == "unitDirOffset"
			|| fureName == "startPosOffset")
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Reset##").x);

			std::string ResetButton = "Reset##";
			ResetButton += fureName;
			if (ImGui::Button(ResetButton.c_str()))
			{
				DirectX::SimpleMath::Vector3 setVector;
				if (fureName == "scale" || fureName == "color")
				{
					setVector = { 1.f,1.f,1.f };
				}
				else if (fureName == "attenuation")
				{
					setVector = { 1.f,0.5f,0.8f };
				}
				else if (fureName == "unitDirOffset")
				{
					setVector = { 0.f,0.f,1.f };
				}
				else
				{
					setVector = { 0.f,0.f,0.f };
				}

				property->Set<DirectX::SimpleMath::Vector3>(m_currentViewComponent.lock().get(), setVector);

				m_selectEntity.lock()->GetComponent<Transform>()->Update(m_deltaTime);
				m_selectEntity.lock()->GetComponent<Transform>()->SetIsUpdated(false);
			}
		}
	}

	else if (fullTypeName == "struct DirectX::SimpleMath::Vector2")
	{
		fureName = StringUtils::EraseLocal(fureName);

		Vector2 vector = property->Get<DirectX::SimpleMath::Vector2>(m_currentViewComponent.lock().get()).GetValue();
		float fArray[] = { vector.x, vector.y };

		if (ImGui::DragFloat2(fureName.c_str(), fArray, 0.1f))
		{
			Vector2 setVector = { fArray[0], fArray[1] };
			property->Set<Vector2>(m_currentViewComponent.lock().get(), setVector);
		}
	}
	else if (fullTypeName == "struct DirectX::SimpleMath::Vector4")
	{
		fureName = StringUtils::EraseLocal(fureName);

		if (property->GetOwnerTypeInfo().GetName() == "PBRSet")
		{
			Vector4 vector = property->Get<DirectX::SimpleMath::Vector4>(m_pbrSet).GetValue();
			float fArray[] = { vector.x, vector.y, vector.z, vector.w };

			if (ImGui::ColorEdit4(fureName.c_str(), fArray, 0.001f))
			{
				Vector4 setVector = { fArray[0], fArray[1],fArray[2], fArray[3] };
				property->Set<Vector4>(m_pbrSet, setVector);
			}

			/// PBRSet Vector4 ���� ��ư
			if (fureName == "baseColor")
			{
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Reset##").x);

				std::string ResetButton = "Reset##";
				ResetButton += fureName;
				if (ImGui::Button(ResetButton.c_str()))
				{
					DirectX::SimpleMath::Vector4 setVector;
					if (fureName == "baseColor")
					{
						setVector = { 1.f,1.f,1.f, 1.f };
					}
					else
					{
						setVector = { 0.f,0.f,0.f,0.f };
					}

					property->Set<Vector4>(m_pbrSet, setVector);
				}
			}
		}

		else if (fureName == "ambient" || fureName == "color")
		{
			Vector4 vector = property->Get<DirectX::SimpleMath::Vector4>(m_currentViewComponent.lock().get()).GetValue();
			float fArray[] = { vector.x, vector.y, vector.z, vector.w };

			if (ImGui::ColorEdit4(fureName.c_str(), fArray, 0.001f))
			{
				Vector4 setVector = { fArray[0], fArray[1],fArray[2], fArray[3] };
				property->Set<Vector4>(m_currentViewComponent.lock().get(), setVector);
			}
		}

		else
		{
			Vector4 vector = property->Get<DirectX::SimpleMath::Vector4>(m_currentViewComponent.lock().get()).GetValue();
			float fArray[] = { vector.x, vector.y, vector.z, vector.w };

			if (ImGui::DragFloat4(fureName.c_str(), fArray, 0.1f))
			{
				Vector4 setVector = { fArray[0], fArray[1],fArray[2], fArray[3] };
				property->Set<Vector4>(m_currentViewComponent.lock().get(), setVector);
			}
		}

		/// Vector4 ���� ��ư
		if (fureName == "ambient" || fureName == "baseColor")
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Reset##").x);

			std::string ResetButton = "Reset##";
			ResetButton += fureName;
			if (ImGui::Button(ResetButton.c_str()))
			{
				DirectX::SimpleMath::Vector4 setVector;
				if (fureName == "ambient" || fureName == "baseColor")
				{
					setVector = { 1.f,1.f,1.f, 1.f };
				}
				else
				{
					setVector = { 0.f,0.f,0.f,0.f };
				}

				property->Set<DirectX::SimpleMath::Vector4>(m_currentViewComponent.lock().get(), setVector);
			}
		}
	}

	else if (fullTypeName == "struct DirectX::SimpleMath::Quaternion")
	{
		Quaternion* data = property->Get<DirectX::SimpleMath::Quaternion>(m_currentViewComponent.lock().get()).GetPointer();
		Vector3 toEuler = QuaternionUtils::ToEulerAngles(*data) / (DirectX::XM_PI / 180.0f);

		if (ImGui::DragFloat3("rotation", &toEuler.x))
		{
			*data = QuaternionUtils::MyCreateFromYawPitchRoll(toEuler * (DirectX::XM_PI / 180.0f));
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Reset##").x);

		std::string ResetButton = "Reset##";
		ResetButton += fureName;
		if (ImGui::Button(ResetButton.c_str()))
		{
			if (fureName == "localRotation")
			{
				*data = { 0.f,0.f,0.f,1.f };
			}

			m_selectEntity.lock()->GetComponent<Transform>()->Update(m_deltaTime);
			m_selectEntity.lock()->GetComponent<Transform>()->SetIsUpdated(false);
		}

		//ImGui::Spacing();
		//ImGui::Spacing();

		//Quaternion* data = property->Get<DirectX::SimpleMath::Quaternion>(m_currentViewComponent.lock().get()).GetPointer();

		//static Vector3 euler = QuaternionUtils::ToEulerAngles(*data);

		//if (m_changeShowEuler)
		//{
		//	m_changeShowEuler = false;
		//	euler = QuaternionUtils::ToEulerAngles(*data);
		//}

		//ImGui::SetNextItemWidth(80.f);
		//ImGui::DragFloat("##x", &euler.x);

		//ImGui::SameLine();
		//ImGui::SetNextItemWidth(80.f);
		//ImGui::DragFloat("##y", &euler.y);

		//ImGui::SameLine();
		//ImGui::SetNextItemWidth(80.f);
		//ImGui::DragFloat("rotation##z", &euler.z);

		///*ImGui::SameLine();
		//ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("GetAnglesApply####").x);
		//if (ImGui::Button("GetAngles"))
		//{
		//	euler = QuaternionUtils::ToEulerAngles(*data);
		//}*/

		//ImGui::SameLine();
		//ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Apply##").x);
		//if (ImGui::Button("Apply"))
		//{
		//	m_changeShowEuler = true;
		//	euler.x = DirectX::XMConvertToRadians(euler.x);
		//	euler.y = DirectX::XMConvertToRadians(euler.y);
		//	euler.z = DirectX::XMConvertToRadians(euler.z);
		//	*data = Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
		//	//euler = QuaternionUtils::ToEulerAngles(*data);
		//}

		ImGui::SetNextItemWidth(75);
		if (ImGui::DragFloat("##xAxis", &m_currentAxis.x, 0.01f))
		{
			*data *= Quaternion::CreateFromAxisAngle(Vector3::UnitX, m_currentAxis.x - m_prevAxis.x);
			m_prevAxis.x = m_currentAxis.x;
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(75);
		if (ImGui::DragFloat("##yAxis", &m_currentAxis.y, 0.01f))
		{
			*data *= Quaternion::CreateFromAxisAngle(Vector3::UnitY, m_currentAxis.y - m_prevAxis.y);
			m_prevAxis.y = m_currentAxis.y;
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(75);
		if (ImGui::DragFloat("##zAxis", &m_currentAxis.z, 0.01f))
		{
			*data *= Quaternion::CreateFromAxisAngle(Vector3::UnitZ, m_currentAxis.z - m_prevAxis.z);
			m_prevAxis.z = m_currentAxis.z;
		}

		ImGui::SameLine();
		ImGui::Text("rotation(Axis)");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Axis Clear##").x);
		if (ImGui::Button("Axis Clear"))
		{
			m_currentAxis = Vector3::Zero;
			m_prevAxis = Vector3::Zero;
		}
	}

	else if (fullTypeName == "int")
	{
		ImGui::InputInt(fureName.c_str(), &property->Get<int>(m_currentViewComponent.lock().get()).GetValue());
	}
	else if (fullTypeName == "unsigned int")
	{
		ImGui::InputScalar(fureName.c_str(), ImGuiDataType_U32, &property->Get<unsigned int>(m_currentViewComponent.lock().get()).GetValue());
	}
	else if (fullTypeName == "float")
	{
		if (property->GetOwnerTypeInfo().GetName() == "PBRSet")
		{
			if (ImGui::DragFloat(fureName.c_str(), property->Get<float>(m_pbrSet).GetPointer(), 0.001f))
			{
				if (fureName == "metallic" || fureName == "roughness" || fureName == "AO")
				{
					float value = property->Get<float>(m_pbrSet).GetValue();

					if (value < 0.f)
					{
						value = 0.f;
						property->Set<float>(m_pbrSet, value);
					}

					if (value > 1.f)
					{
						value = 1.f;
						property->Set<float>(m_pbrSet, value);
					}
				}
			}
		}
		else if (fureName == "guardSpeed")
		{
			if (ImGui::DragFloat(fureName.c_str(), &property->Get<float>(m_currentViewComponent.lock().get()).GetValue(), 0.01f))
			{
				static_pointer_cast<GuardFSM>(m_currentViewComponent.lock())->UpdateGuardSpeed();
			}
		}

		else if (fureName == "range")
		{
			ImGui::DragFloat(fureName.c_str(), &property->Get<float>(m_currentViewComponent.lock().get()).GetValue(), 0.01f);
		}

		// ������ �Լ� ���÷��� ������.....
		else if (ImGui::DragFloat(fureName.c_str(), &property->Get<float>(m_currentViewComponent.lock().get()).GetValue(), 0.1f))
		{
			CheckColliderTypeAndInvokeFunction(property);
		}
	}
	else if (fullTypeName == "double")
	{
		ImGui::InputDouble(fureName.c_str(), property->Get<double>(m_currentViewComponent.lock().get()).GetPointer());
	}
	else if (fullTypeName == "bool")
	{
		if (property->GetOwnerTypeInfo().GetName() == "PBRSet")
		{
			ImGui::Checkbox(fureName.c_str(), property->Get<bool>(m_pbrSet).GetPointer());
		}
		else if (ImGui::Checkbox(fureName.c_str(), property->Get<bool>(m_currentViewComponent.lock().get()).GetPointer()))
		{
			CheckColliderTypeAndInvokeFunction(property);
		}
	}
	else
	{
		assert(!"���÷����� PrimitiveType �б⸦ �÷��� �մϴ�.  - Dongil");
	}
}

void InspectorGui::ReadPrimiteveStringType(const Property* property)
{
	std::string fullTypeName = property->GetTypeInfo().GetFullName();

	TypeInfoManager* manager = TypeInfoManager::GetInstance();
	std::type_index index = manager->GetTypeIndex(fullTypeName);

	std::string fureName = property->GetName();
	fureName = StringUtils::EraseUnderbar(fureName);

	if (index == GetTypeIndex<std::string>())
	{
		ImGui::Spacing();
		ImGui::Text("Type : std::string");
		ImGui::Text(property->Get<std::string>(m_currentViewComponent.lock().get()).GetValue().c_str());
		ImGui::Spacing();
	}
	else if (index == GetTypeIndex<std::wstring>())
	{
		// TextRenderer�� ����
		// Imgui ������ c++20 �浹�� �ѱ��� �ȵǱ⿡
		// ���Ϸ� ������ �����صΰ� �ҷ��ͼ� ���÷��� �� ���ִ� ��� ä�� (���� ���̵��)
		if (fureName == "text")
		{
			static int index = 0;

			if (ImGui::BeginCombo(fureName.c_str(), m_textList[index].c_str()))
			{
				// �� ����Ʈ 
				for (int i = 0; i < m_textList.size(); ++i)
				{
					// �� �׸� ���� ���� ������ ��ư�� ����
					const bool isSeleted = (index == i);

					// �����ߴٸ� ������ �ε����� ����
					if (ImGui::Selectable(m_textList[i].c_str(), isSeleted))
					{
						index = i;

						for (auto& entry : std::filesystem::directory_iterator(PathManager::GetTextDataPath()))
						{
							const auto& path = entry.path();

							if (std::filesystem::is_regular_file(path))
							{
								std::string fileName = path.filename().string();

								if (m_textList[i] == fileName)
								{
									std::ifstream readData(path);

									if (readData.is_open())
									{
										// ������ ���� ���� ���� �� ������ ����Ű�� �ݺ��ڸ� �����, 
										// �� �ݺ��ڵ� ������ ������ ���ڿ��� ����
										std::string fileContent((std::istreambuf_iterator<char>(readData)),
											std::istreambuf_iterator<char>());

										// JSON �Ľ�
										nlohmann::json jsonData = nlohmann::json::parse(fileContent);

										for (const auto& [typeName, data] : jsonData.items())
										{
											if (typeName == "Text")
											{
												std::string utf8Str = data;

												int size = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), static_cast<int>(utf8Str.size()), nullptr, 0);
												std::wstring wStr(size, 0);
												MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), static_cast<int>(utf8Str.size()), &wStr[0], size);

												property->Set<std::wstring>(m_currentViewComponent.lock().get(), wStr);

												break;
											}
										}
									}
								}
							}
						}
					}

					// ���õ� �׸� üũ ��ũ ǥ��
					if (isSeleted)
					{
						ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
					}
				}

				ImGui::EndCombo();
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("File Update##").x);

			std::string ResetButton = "File Update";
			if (ImGui::Button(ResetButton.c_str()))
			{
				GetTextFileList();
			}
		}


		//if (ImGui::InputText(fureName.c_str(), m_wstringBuffer, sizeof(m_wstringBuffer)))
		//{
		//	if (strlen(m_wstringBuffer) > 0)  // �ؽ�Ʈ�� �ִ� ��쿡�� ó��
		//	{
		//		std::string newName = m_wstringBuffer;
		//		std::wstring wName = StringUtils::ToWString(newName);

		//		property->Set<std::wstring>(m_currentViewComponent.lock().get(), wName);
		//	}
		//}

		/// Imgui::InputText�� wstring�� ���޴´�..
		/// Imgui ������ c++20 ���� ���� �ذ� ���߾���.
		//std::wstring wStr = property->Get<std::wstring>(m_currentViewComponent.lock().get()).GetValue();

		//std::vector<wchar_t> wStrBuffer(wStr.begin(), wStr.end());
		//wStrBuffer.push_back(L'\0');

		//// wchar_t �迭�� m_wstringBuffer�� ����
		//std::copy(wStrBuffer.begin(), wStrBuffer.end(), m_wstringBuffer);

		//if (ImGui::InputText(fureName.c_str(), m_wstringBuffer, sizeof(m_wstringBuffer)))
		//{
		//	// �Էµ� �ؽ�Ʈ�� �ִ� ��쿡�� ó��
		//	if (wcslen(m_wstringBuffer) > 0)
		//	{
		//		// wchar_t �迭�� std::wstring���� ��ȯ
		//		std::wstring wName(m_wstringBuffer);

		//		// �Ӽ��� ����
		//		property->Set<std::wstring>(m_currentViewComponent.lock().get(), wName);
		//	}
		//}

		/// �굵 ��������
		//std::wstring wStr = property->Get<std::wstring>(m_currentViewComponent.lock().get()).GetValue();

		//// std::wstring�� std::string���� ��ȯ
		//std::string strBuffer = StringUtils::ToString(wStr);

		//// char �迭�� ��ȯ
		//char m_charBuffer[256];
		//std::copy(strBuffer.begin(), strBuffer.end(), m_charBuffer);
		//m_charBuffer[strBuffer.size()] = '\0';  // null�� ������

		//if (ImGui::InputText(fureName.c_str(), m_charBuffer, sizeof(m_charBuffer)))
		//{
		//	// �Էµ� �ؽ�Ʈ�� �ִ� ��쿡�� ó��
		//	if (strlen(m_charBuffer) > 0)
		//	{
		//		// char �迭�� std::wstring���� ��ȯ
		//		std::wstring wName = StringUtils::ToWString(m_charBuffer);

		//		// �Ӽ��� ����
		//		property->Set<std::wstring>(m_currentViewComponent.lock().get(), wName);
		//	}
		//}
	}
	else
	{
		assert(!"�� �� ���� Ÿ���Դϴ�. - Dongil");
	}
}

// ���� ���� ��� 5 �� ���弱
// enum class �� �þ �� ���� �ϵ��ڵ��ؾ��Ѵ�.
// ���� �ڵ� ���� ����. ������ ����
void InspectorGui::ReadEnumClassType(const Property* property)
{
	std::string fullTypeName = property->GetTypeInfo().GetFullName();
	fullTypeName = StringUtils::EraseSpace(fullTypeName);

	std::string fureName = property->GetName();
	fureName = StringUtils::EraseUnderbar(fureName);

	if (fureName == "cloneModelKey")
		fureName = "ModelKey";

	int currentIndex = 0; // enum�� �ش��ϴ� �迭�� �ε���

	/*if (fullTypeName == "Layers")
	{
		currentIndex = static_cast<int>(property->Get<Layers>(m_currentViewComponent.lock().get()).GetValue());
	}
	else if (fullTypeName == "LayerCollision")
	{
		currentIndex = static_cast<int>(property->Get<LayerCollision>(m_currentViewComponent.lock().get()).GetValue());
	}*/
	if (fullTypeName == "BodyType")
	{
		currentIndex = static_cast<int>(property->Get<BodyType>(m_currentViewComponent.lock().get()).GetValue());
	}
	else if (fullTypeName == "GraphicsTransfer::ModelKey")
	{
		currentIndex = static_cast<int>(property->Get<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get()).GetValue());
	}
	else if (fullTypeName == "FoodType")
	{
		currentIndex = static_cast<int>(property->Get<FoodType>(m_currentViewComponent.lock().get()).GetValue());
	}
	else
	{
		assert(!"���÷����� enum class �� �����ϴ� �б⸦ �߰� �ؾ��ճ���. - ����");
	}


	std::vector<std::string> memberNames{}; // ����� �̸�

	// ���� ��ȸ�Ѵ�.
	for (auto& iter : m_enumClassStringList)
	{
		// ���� Ű ���� �ش��ϴ� enum class ���
		if (iter.first == fullTypeName)
		{
			// enum class�� ��ȸ�Ѵ�.
			for (auto& string : iter.second)
			{
				memberNames.push_back(string.c_str());
			}
		}
	}

	if (fureName == "ModelKey")
	{
		ImGui::Spacing();
		ImGui::Spacing();
	}



	if (ImGui::BeginCombo(fureName.c_str(), memberNames[currentIndex].c_str()))
	{
		// Combo Box �˻��� ���� �ؽ�Ʈ �Է� ������ �߰�
		if (ImGui::InputText("Search", m_enumComboSerchBuffer, sizeof(m_enumComboSerchBuffer)))
		{
			size_t size = std::strlen(m_enumComboSerchBuffer);

			for (int i = 0; i < size; ++i)
			{
				m_enumComboSerchBuffer[i] = std::tolower(m_enumComboSerchBuffer[i]);
			}
		}

		m_serchEnumName = m_enumComboSerchBuffer;

		// �˻�â �ʱ�ȭ ��ư
		if (!m_serchEnumName.empty())
		{
			ImGui::SameLine();

			if (ImGui::Button("Clear"))
			{
				m_enumComboSerchBuffer[0] = '\0';
			}
		}

		for (int i = 0; i < memberNames.size(); ++i)
		{
			// �� �׸� ���� ���� ������ ��ư�� ����
			const bool isSelected = (currentIndex == i);

			// enum �̸� �빮�� -> �ҹ��ڷ� ��ȯ
			std::string lowerName;
			for (int j = 0; j < memberNames[i].size(); ++j)
			{
				lowerName.push_back(std::tolower(memberNames[i][j]));
			}

			// �ҹ��ڷ� ��ȯ��Ų �̳� �̸����� �˻��� �̸��� �ִ��� ã��
			if (lowerName.find(m_serchEnumName) == std::string::npos)
			{
				continue;
			}

			if (ImGui::Selectable(memberNames[i].c_str(), isSelected))
			{
				m_selectBeginCombo = true;
				currentIndex = i;
			}

			// ���õ� �׸� üũ ��ũ ǥ��
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
			}
		}

		ImGui::EndCombo();
	}


	if (m_selectBeginCombo)
	{
		///// Layers
		//if (fullTypeName == "Layers")
		//{
		// 	m_selectBeginCombo = false;
		// 
		//	// m_enumClassStringList["Layers"] = { "None", "Background", "Player", "Monster", "Trap", "Object", "UI" };

		//	switch (currentIndex)
		//	{
		//	case 0:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::None);
		//		break;
		//	case 1:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::Background);
		//		break;
		//	case 2:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::Player);
		//		break;
		//	case 3:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::Monster);
		//		break;
		//	case 4:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::Trap);
		//		break;
		//	case 5:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::Object);
		//		break;
		//	case 6:
		//		property->Set<Layers>(m_currentViewComponent.lock().get(), Layers::UI);
		//		break;
		//	default:
		//		assert(!"case �б� �÷��ּ���. (Layers) - ����");
		//		break;
		//	}
		//}

		///// LayerCollision
		//else if (fullTypeName == "LayerCollision")
		//{
		// 	m_selectBeginCombo = false;
		// 
		//	// m_enumClassStringList["LayerCollision"] = { "Default", "UI", "Ground", "Player", "Interactable", "Wall", "Enemy", "Coin", "Heart" };

		//	switch (currentIndex)
		//	{
		//	case 0:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Default);
		//		break;
		//	case 1:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::UI);
		//		break;
		//	case 2:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Ground);
		//		break;
		//	case 3:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Player);
		//		break;
		//	case 4:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Interactable);
		//		break;
		//	case 5:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Wall);
		//		break;
		//	case 6:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Enemy);
		//		break;
		//	case 7:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Coin);
		//		break;
		//	case 8:
		//		property->Set<LayerCollision>(m_currentViewComponent.lock().get(), LayerCollision::Heart);
		//		break;
		//	default:
		//		assert(!"case �б� �÷��ּ���. (LayerCollision) - ����");
		//		break;
		//	}
		//}

		/// Collider
		if (fullTypeName == "BodyType")
		{
			m_selectBeginCombo = false;
			// m_enumClassStringList["BodyType"] = { "Static", "Dynamic" };

			switch (currentIndex)
			{
			case 0:
				property->Set<BodyType>(m_currentViewComponent.lock().get(), BodyType::Static);
				CheckColliderTypeAndInvokeFunction(property);
				break;
			case 1:
				property->Set<BodyType>(m_currentViewComponent.lock().get(), BodyType::Dynamic);
				CheckColliderTypeAndInvokeFunction(property);
				break;

			default:
				assert(!"case �б� �÷��ּ���. (BodyType) - ����");
				break;
			}
		}
		else if (fullTypeName == "GraphicsTransfer::ModelKey")
		{
			m_selectBeginCombo = false;

			switch (currentIndex)
			{
				//m_enumClassStringList["GraphicsTransfer::ModelKey"] = { "None",
				//	// Primitive ----------
				//	"Grid", "TextureBox", "RedBox", "YellowBox", "PinkBox", "WireFrameBox","WireFrameRay",
				//	"WireFrameSphere", "Swablu","SfBox", "Erika", "SampleTable", "SampleFood", "SampleStore"
				//	// Character -------------
				//	"Player", "Patrol", "Merchant",
				//	// Prop ------------------
				//	"Floor", "Wall" , "Box1" , "Box2" , "Tree1","Tree2", "Barrel" , "Bush" ,
				//	"Fountain" , "Grass","Lamp","Potion","FoodTable",
				//	// Building --------------
				//	"BagleStore", "BaguettePointStore", "CupCakeStore", "CrabStore", "AppleStore", "CartoonMeatStore",
				//	"HamStore", "WaterMelonStore", "FishStore", "SquidStore", "CakeStore",
				//	"SausageStore", "BaguetteWheelStore", "BananaStore",
				//	// Food ------------------
				//	"Meat", "Seafood" , "Bread" , "Fruit" , "Dessert",
				//	// QTE -------------------
				//	"QTE_InteractiveKey" , "QTE_MiniGame",
				//	// UI --------------------
				//	"UI", "UIGauge", "UI_Timer", "UIHungry", "UI_Main", "UI_Sense",
				//	"UI_Score", "UI_Minimap", "UI_IngamePopUp", "UI_ReVisitCoolTime", "UI_DeliciousFood",
				//	"UI_MinimapCurPos","UI_Merchant"
				//};

			case 0:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::None);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
				// Primitive ----------
			case 1:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Grid);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 2:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::TextureBox);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 3:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::RedBox);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 4:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::YellowBox);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 5:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::PinkBox);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 6:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::WireFrameBox);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 7:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::WireFrameRay);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 8:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::WireFrameSphere);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 9:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Swablu);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 10:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::SfBox);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 11:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Erika);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 12:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::SampleTable);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 13:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::SampleFood);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 14:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::SampleStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
				// Character -------------
			case 15:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Player);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 16:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Patrol);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 17:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Merchant);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
				// Prop ------------------
			case 18:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Floor);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 19:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Wall);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 20:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Box1);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 21:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Box2);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 22:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Tree1);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 23:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Tree2);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 24:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Barrel);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 25:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Bush);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 26:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Fountain);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 27:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Grass);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 28:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Lamp);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 29:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Potion);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 30:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::FoodTable);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
				// Building --------------
			case 31:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::BagleStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::BagleStore);
				break;
			case 32:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::BaguettePointStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::BaguettePointStore);
				break;
			case 33:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::CupCakeStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::CupCakeStore);
				break;
			case 34:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::CrabStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::CrabStore);
				break;
			case 35:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::AppleStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::AppleStore);
				break;
			case 36:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::CartoonMeatStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::CartoonMeatStore);
				break;
			case 37:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::HamStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::HamStore);
				break;
			case 38:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::WaterMelonStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::WaterMelonStore);
				break;
			case 39:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::FishStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::FishStore);
				break;
			case 40:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::SquidStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::SquidStore);
				break;
			case 41:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::CakeStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::CakeStore);
				break;
			case 42:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::SausageStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::SausageStore);
				break;
			case 43:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::BaguetteWheelStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::BaguetteWheelStore);
				break;
			case 44:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::BananaStore);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				FoodStoreSet(ModelKey::BananaStore);
				break;
				// Food ------------------
			case 45:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Meat);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 46:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Seafood);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 47:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Bread);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 48:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Fruit);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 49:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Dessert);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
				// QTE -------------------
			case 50:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::QTE_InteractiveKey);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 51:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::QTE_MiniGame);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
				// UI --------------------
			case 52:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 53:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UIGauge);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 54:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Timer);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 55:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UIHungry);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 56:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Main);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 57:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Sense);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 58:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Score);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 59:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Minimap);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 60:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_IngamePopUp);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 61:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_ReVisitCoolTime);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 62:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_DeliciousFood);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 63:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_MinimapCurPos);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 64:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Merchant);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 65:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_EatSpot);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 66:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Candy);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 67:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Table);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 68:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Flag);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;			
			case 69:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_MiniMapIcon);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 70:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Skill);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 71:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::UI_Black);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 72:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Candle1);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			case 73:
				property->Set<GraphicsTransfer::ModelKey>(m_currentViewComponent.lock().get(), GraphicsTransfer::ModelKey::Candle2);
				m_selectEntity.lock()->GetComponent<MeshFilter>()->UpdateModelKey();
				break;
			default:
				assert(!"case �б� �÷��ּ���. (ModelKey) - ����");
				break;
			}
		}
		else if (fullTypeName == "FoodType")
		{
			m_selectBeginCombo = false;

			switch (currentIndex)
			{
				/// FoodType
				m_enumClassStringList["FoodType"] = { "None", "Bagle", "BaguettePoint", "CupCake" , "Crab" , "Apple" , "CartoonMeat"
				, "Ham" , "WaterMelon" , "Fish" , "Squid" , "Cake" , "Sausage" , "BaguetteWheel", "Banana" };

			case 0:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::None);
				break;
			case 1:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Bagle);
				break;
			case 2:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::BaguettePoint);
				break;
			case 3:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::CupCake);
				break;
			case 4:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Crab);
				break;
			case 5:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Apple);
				break;
			case 6:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::CartoonMeat);
				break;
			case 7:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Ham);
				break;
			case 8:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::WaterMelon);
				break;
			case 9:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Fish);
				break;
			case 10:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Squid);
				break;
			case 11:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Cake);
				break;
			case 12:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Sausage);
				break;
			case 13:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::BaguetteWheel);
				break;
			case 14:
				property->Set<FoodType>(m_currentViewComponent.lock().get(), FoodType::Banana);
				break;
			default:
				assert(!"case �б� �÷��ּ���. (MiniGameType) - ����");
				break;
			}
		}
		else
		{
			assert(!"���÷����� enum class �� �����ϴ� �б⸦ �߰� �ؾ��ճ���. - ����");
		}
	}
}



void InspectorGui::ReadSTD_VectorType(const Property* property)
{
	std::string fullTypeName = property->GetTypeInfo().GetFullName();

	std::string fureName = property->GetName();
	fureName = StringUtils::EraseUnderbar(fureName);

	if (fureName == "patrolPath")
	{
		std::vector<Vector3> patrolPaths = property->Get<std::vector<Vector3>>(m_currentViewComponent.lock().get()).GetValue();

		static float value[3] = {};
		ImGui::InputFloat3("##PatrolPath", value);

		ImGui::SameLine();

		if (ImGui::Button("Make patrol path"))
		{
			patrolPaths.push_back(Vector3{ value[0], value[1], value[2] });
			property->Set<std::vector<Vector3>>(m_currentViewComponent.lock().get(), patrolPaths);
			value[0] = {};
			value[1] = {};
			value[2] = {};
		}

		static std::vector<std::string>  patrolPathList;
		patrolPathList.clear();
		for (auto& patrolPath : patrolPaths)
		{
			patrolPathList.push_back(std::to_string(patrolPath.x) + ", " + std::to_string(patrolPath.y) + ", " + std::to_string(patrolPath.z));
		}

		if (patrolPathList.size())
		{
			static int index = 0;
			if (ImGui::BeginCombo("##PatrolPathList", patrolPathList[index].c_str()))
			{
				// ������Ʈ ����Ʈ ��ȸ
				for (int i = 0; i < patrolPathList.size(); ++i)
				{
					// �� �׸� ���� ���� ������ ��ư�� ����
					const bool isSeleted = (index == i);

					// �����ߴٸ� ������ �ε����� ����
					if (ImGui::Selectable(patrolPathList[i].c_str(), isSeleted))
					{
						index = i;
					}

					// ���õ� �׸� üũ ��ũ ǥ��
					if (isSeleted)
					{
						ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
					}
				}

				ImGui::EndCombo();
			}

			ImGui::SameLine();

			if (ImGui::Button("Destroy patrol path"))
			{
				patrolPaths.erase(patrolPaths.begin() + index);
				index = 0;

				property->Set<std::vector<Vector3>>(m_currentViewComponent.lock().get(), patrolPaths);
			}
		}
	}
}

void InspectorGui::ReadComplexType(const Property* property)
{
	ImGui::Spacing();

	std::string str = "Type : " + property->GetTypeInfo().GetFullName();
	ImGui::Text(str.c_str());
	//ImGui::TextWrapped(str.c_str());

	str = "Name : ";
	str += +property->GetName();
	ImGui::Text(str.c_str());
	//ImGui::TextWrapped(str.c_str());

	ImGui::Spacing();
}

void InspectorGui::ChangeEntityName()
{
	strcpy_s(m_entityNameBuffer, m_selectEntity.lock()->GetName().c_str());

	if (ImGui::InputText("Entity Name", m_entityNameBuffer, sizeof(m_entityNameBuffer)))
	{
		if (strlen(m_entityNameBuffer) > 0)  // �ؽ�Ʈ�� �ִ� ��쿡�� ó��
		{
			std::string newName = m_entityNameBuffer;
			m_selectEntity.lock()->SetName(m_entityNameBuffer);
		}
		else
		{
			m_selectEntity.lock()->SetName(" ");
		}
	}
}

void InspectorGui::ActiveCheckBox()
{
	bool isActive = m_selectEntity.lock()->IsActive();
	if (ImGui::Checkbox("Active On", &isActive))
		m_selectEntity.lock()->SetActive(isActive);
}

void InspectorGui::CullingEntityCheckBox()
{
	bool isCulling = m_selectEntity.lock()->IsCullingOffAll();
	if (ImGui::Checkbox("Cull Off All", &isCulling))
		m_selectEntity.lock()->SetCullingOffAll(isCulling);
}

void InspectorGui::SelectLayer()
{
	const char* bufferNames[] = { "None", "Background", "Player", "Monster", "Trap" ,"Object", "UI" , "CallingRange" };
	int bufferSelect = static_cast<int>(m_selectEntity.lock()->GetLayer());
	if (ImGui::Combo("Layer", &bufferSelect, bufferNames, 8))
	{
		m_selectEntity.lock()->SetLayer(static_cast<Layers>(bufferSelect));
	}
}

void InspectorGui::SelectLayerCollision()
{
	const char* bufferNames[] = { "Default", "UI", "Ground", "Player", "Interactable" ,"Wall", "Enemy", "Coin", "Heart","CallingRange" };
	/*int bufferSelect = static_cast<int>(m_selectEntity.lock()->GetLayerCollision());
	if (ImGui::Combo("LayerCollision", &bufferSelect, bufferNames, 9))
	{
		m_selectEntity.lock()->SetLayerCollision(static_cast<LayerCollision>(bufferSelect));
	}*/

	int bufferSelect = 0;

	LayerCollision entityLayerCollision = m_selectEntity.lock()->GetLayerCollision();
	for (int i = 0; i < 10; ++i)
	{
		if ((static_cast<int>(entityLayerCollision) & (1 << i)) != 0)
		{
			bufferSelect = i;
			break;
		}
	}

	if (ImGui::Combo("LayerCollision", &bufferSelect, bufferNames, 10))
	{
		m_selectEntity.lock()->SetLayerCollision(static_cast<LayerCollision>(1 << bufferSelect));
	}
}

void InspectorGui::AddComponentCombo()
{
	// ������Ʈ �޺�
	ComponentCombo();

	// ��ũ��Ʈ �޺�
	ScriptCombo();
}

void InspectorGui::GetTextFileList()
{
	m_textList.clear();

	for (auto& entry : std::filesystem::directory_iterator(PathManager::GetTextDataPath()))
	{
		const auto& path = entry.path();
		std::string fileName = path.filename().string();
		m_textList.push_back(fileName);
	}
}

/// <summary>
/// ���� ���� ���͸� �����ϸ� �׿� ���õ� ģ������ �����Ѵ�.
/// ��Ʈȭ �����ֱ�.
/// </summary>
/// <param name="storeKey">���� ���� �޽� ����</param>
void InspectorGui::FoodStoreSet(ModelKey storeKey)
{
	int size = m_currentViewComponent.lock()->GetTransform()->GetChildren().size();
	for (int i = size - 1; i >= 0; --i)
	{
		const auto& entity = m_currentViewComponent.lock()->GetTransform()->GetChild(i).lock()->GetOwner();
		m_sceneManager.lock()->GetCurScene()->DestroyEntity(entity);
	}

	/// ���� ����
	m_currentViewComponent.lock()->GetOwner().lock()->AddComponent<BoxCollider>()->SetSize({7.f,7.5f,5.8f});
	m_currentViewComponent.lock()->GetOwner().lock()->AddComponent<BoxCollider>()->SetOffset({ -0.7,3.5,0.2 });
	m_currentViewComponent.lock()->GetOwner().lock()->SetLayerCollision(LayerCollision::Store);

	/// ��ȣ�ۿ� Ű
	std::shared_ptr<Entity> interactiveKey = std::make_shared<Entity>("InteractiveKey");
	m_sceneManager.lock()->GetCurScene()->AddEntity(interactiveKey);
	interactiveKey->AddComponent<InterKeyScript>();
	interactiveKey->AddComponent<UIRenderer>("InteractiveKeyRenderer");
	interactiveKey->AddComponent<MeshFilter>("InteractiveKeyMesh")->SetModelKey(ModelKey::QTE_InteractiveKey);

	/// ����
	std::shared_ptr<Entity> merchant = std::make_shared<Entity>("Merchant");
	m_sceneManager.lock()->GetCurScene()->AddEntity(merchant);
	merchant->AddComponent<ShopKeeperFSM>();
	merchant->AddComponent<ShopKeeperDoubtScript>();
	merchant->AddComponent<MeshRenderer>("MerchantRenderer");
	merchant->AddComponent<MeshFilter>("MerchantrMesh")->SetModelKey(ModelKey::Merchant);
	merchant->SetLayerCollision(LayerCollision::Merchant);

	/// ������ UI
	auto merchantUI = std::make_shared<Entity>("MerchantUI");
	m_sceneManager.lock()->GetCurScene()->AddEntity(merchantUI);
	merchant->GetComponent<Transform>()->SetChild(merchantUI->GetComponent<Transform>());
	merchantUI->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Merchant);
	merchantUI->AddComponent<UIRenderer>();
	merchantUI->AddComponent<MerchantUI>();

	///������ ȣ�����
	auto callingRange = std::make_shared<Entity>("callingRange");
	m_sceneManager.lock()->GetCurScene()->AddEntity(callingRange);
	callingRange->AddComponent<Transform>();
	callingRange->AddComponent<MeshRenderer>();
	callingRange->AddComponent<SphereCollider>();
	callingRange->GetComponent<SphereCollider>()->SetRadius(10.f);
	callingRange->GetComponent<SphereCollider>()->SetDisableGravity(true);
	callingRange->GetComponent<SphereCollider>()->SetIsTrigger(true);
	callingRange->AddComponent<ShopKeeperCallingScript>();

	///ȣ����� ���� ��ƼƼ�� ���̱�
	merchant->GetComponent<Transform>()->SetChild(callingRange->GetComponent<Transform>());
	callingRange->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));

	int m_range = 10;
	///������ ��ġ ����
//���� ray
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(0)->SetUnitDirectionOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(0)->SetStartPosOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(0)->SetDistance(m_range);

	//���� 15��
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(1)->SetUnitDirectionOffset(Vector3(-0.26, 0, 1));
	merchant->GetComponent<RayCaster>(1)->SetStartPosOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(1)->SetDistance(m_range);

	//������ 15��
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(2)->SetUnitDirectionOffset(Vector3(0.26, 0, 1));
	merchant->GetComponent<RayCaster>(2)->SetStartPosOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(2)->SetDistance(m_range);

	//���� 7.5��
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(3)->SetUnitDirectionOffset(Vector3(-0.13, 0, 1));
	merchant->GetComponent<RayCaster>(3)->SetStartPosOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(3)->SetDistance(m_range);

	//������ 7.5��
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(4)->SetUnitDirectionOffset(Vector3(0.13, 0, 1));
	merchant->GetComponent<RayCaster>(4)->SetStartPosOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(4)->SetDistance(m_range);

	/// ���� ���̺�
	std::shared_ptr<Entity> foodTable = std::make_shared<Entity>("FoodTable");
	m_sceneManager.lock()->GetCurScene()->AddEntity(foodTable);
	foodTable->AddComponent<FoodTableScript>();
	foodTable->AddComponent<SphereCollider>();
	foodTable->AddComponent<MeshRenderer>("TableRenderer");
	foodTable->AddComponent<MeshFilter>("TableMesh")->SetModelKey(ModelKey::FoodTable);

	/// ����
	std::shared_ptr<Entity> food = std::make_shared<Entity>("Food");
	m_sceneManager.lock()->GetCurScene()->AddEntity(food);
	food->AddComponent<MeshRenderer>("FoodRenderer");
	food->AddComponent<MeshFilter>("FoodMesh"); // ���� �޽����ʹ� �Ű����� storeKey�� Ȱ���Ͽ� �����ֱ�

	// ���� ���԰� ���������� ���� 
	// 1. ���� ���̺� ��ũ��Ʈ�� ���� Ÿ�� ���ϱ�
	// 2. ������ �޽� ���� ���ϱ�
	switch (storeKey)
	{
	case ModelKey::BagleStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Bagle);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Bread);
		//food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Bread); 
		break;
	case ModelKey::BaguettePointStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::BaguettePoint);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Bread);
		break;
	case ModelKey::CupCakeStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::CupCake);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Dessert);
		break;
	case ModelKey::CrabStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Crab);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Seafood);
		break;
	case ModelKey::AppleStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Apple);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Fruit);
		break;
	case ModelKey::CartoonMeatStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::CartoonMeat);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Meat);
		break;
	case ModelKey::HamStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Ham);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Meat);
		break;
	case ModelKey::WaterMelonStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::WaterMelon);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Fruit);
		break;
	case ModelKey::FishStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Fish);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Seafood);
		break;
	case ModelKey::SquidStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Squid);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Seafood);
		break;
	case ModelKey::CakeStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Cake);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Dessert);
		break;
	case ModelKey::SausageStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Sausage);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Meat);
		break;
	case ModelKey::BaguetteWheelStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::BaguetteWheel);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Bread);
		break;
	case ModelKey::BananaStore:
		foodTable->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Banana);
		food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::Fruit);
		break;
	default:
		assert(!"�� Ű �� ���� -����");
	}

	/// ��������
	// ���� ������ �ڽ����� ���� (����)
	m_selectEntity.lock()->GetComponent<Transform>()->SetChild(merchant->GetComponent<Transform>());

	// ���� ������ �ڽ����� ���� (���� ���̺�)
	m_selectEntity.lock()->GetComponent<Transform>()->SetChild(foodTable->GetComponent<Transform>());

	// ���� ���̺��� �ڽ����� ���� (��ȣ�ۿ� Ű)
	foodTable->GetComponent<Transform>()->SetChild(interactiveKey->GetComponent<Transform>());

	// ���� ���̺��� �ڽ����� ���� (����)
	foodTable->GetComponent<Transform>()->SetChild(food->GetComponent<Transform>());

	Vector3 pos = m_currentViewComponent.lock()->GetTransform()->GetWorldPosition();

	/// ���� ���� (��ġ, ȸ��, ũ��)
	// ����
	merchant->GetComponent<Transform>()->AddPosition(Vector3(-0.5f, 0.f, -3.f)+ pos);
	merchant->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	// ���� ���̺�
	foodTable->GetComponent<Transform>()->AddPosition(Vector3(-0.5f, 0.f, -4.f)+ pos);
	foodTable->GetComponent<SphereCollider>()->SetRadius(3.f);
	foodTable->GetComponent<SphereCollider>()->SetDisableGravity(true);
	foodTable->GetComponent<SphereCollider>()->SetIsTrigger(true);

	// ����
	//food->GetComponent<Transform>()->AddPosition(Vector3(-0.5f, 0.f, -4.f));

	// ��ȣ�ۿ� Ű
	//interactiveKey->GetComponent<Transform>()->SetLocalPosition( Vector3{0 ,2.2,0 });
	//interactiveKey->GetComponent<Transform>()->AddPosition( Vector3{ 0,2.2, 0});
	interactiveKey->GetComponent<Transform>()->SetLocalScale( Vector3{ 0.01,0.01,0.01 });

	/// Start ȣ��
	merchant->Start();
	interactiveKey->Start();
	foodTable->Start();
	food->Start();
	merchantUI->Start();
	callingRange->Start();

	/// ���� ���� ������ ����
	// ���� ���̺� ��ũ��Ʈ (��ġ ������) - ����FSM (��ġ ������)
	foodTable->GetComponent<FoodTableScript>()->SetPointerSense(merchant->GetComponent<ShopKeeperFSM>()->GetPointerSense());

	// ��ȣ�ۿ� Ű ��ũ��Ʈ (��� ���� ��ũ��Ʈ) - ���� ���̺� ��ũ��Ʈ
	interactiveKey->GetComponent<InterKeyScript>()->m_foodTableScript = foodTable->GetComponent<FoodTableScript>();
}

void InspectorGui::IsRenderEntity()
{
	bool isRender = m_selectEntity.lock()->IsRender();
	if (ImGui::Checkbox("Is Render", &isRender))
		m_selectEntity.lock()->SetRender(isRender);
}

void InspectorGui::SearchComponentCombo()
{
	// Combo Box �˻��� ���� �ؽ�Ʈ �Է� ������ �߰�
	if (ImGui::InputText("Search Component", m_componentComboSearchBuffer, sizeof(m_componentComboSearchBuffer)))
	{
		size_t size = std::strlen(m_componentComboSearchBuffer);

		for (int i = 0; i < size; ++i)
		{
			m_componentComboSearchBuffer[i] = std::tolower(m_componentComboSearchBuffer[i]);
		}
	}

	m_searchComponentName = m_componentComboSearchBuffer;

	// �˻�â �ʱ�ȭ ��ư
	if (!m_searchComponentName.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
		{
			m_componentComboSearchBuffer[0] = '\0';
		}
	}
}

//void InspectorGui::DestroyComponentButton(std::vector<std::string> destroyComponentList)
//{
//	if (destroyComponentList.size() == 0)
//		return;
//
//	static int destoryIndex = 0;
//	std::string componentName = destroyComponentList[destoryIndex];
//
//	if (ImGui::BeginCombo("##DestroyComponentCombo", componentName.c_str()))
//	{
//		for (int i = 0; i < destroyComponentList.size(); ++i)
//		{
//			// �� �׸� ���� ���� ������ ��ư�� ����
//			const bool isSeleted = (destoryIndex == i);
//
//			componentName = destroyComponentList[i];
//			if (ImGui::Selectable(componentName.c_str(), isSeleted))
//				destoryIndex = i;
//
//			// ���õ� �׸� üũ ��ũ ǥ��
//			if (isSeleted)
//				ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
//		}
//
//		ImGui::EndCombo();
//	}
//
//	ImGui::SameLine();
//
//	if (ImGui::Button("Destroy Component"))
//	{
//		std::string componentName = "class ";
//		componentName += destroyComponentList[destoryIndex];
//
//		TypeInfoManager* manager = TypeInfoManager::GetInstance();
//		auto index = manager->GetTypeIndex(componentName);
//
//		m_selectEntity.lock()->DestroyComponent(index);
//
//		if (destoryIndex - 1 >= 1)
//		{
//			--destoryIndex;
//		}
//
//		// Ư�� ������Ʈ�� � ������Ʈ�� �Բ� �־�߸� �ɷ��� ����� ģ���� �ִ�. (EX. �޽� �������� �޽� ����)
//		// ������ 2�� ��ư ���������� �� ���� ������Ű�� ���� ��ƿ �ڵ� ����
//		TypeInfo* typeInfoMate = nullptr;
//
//		// ���� �� ������Ʈ�� �޽� ��������
//		if (componentName == "class MeshRenderer")
//		{
//			index = manager->GetTypeIndex("class MeshFilter");
//
//			// ¦���� �޽� ���͸� ������ �ִٸ� ���� ���� �����ش�.
//			if (m_selectEntity.lock()->HasComponent(index))
//			{
//				m_selectEntity.lock()->DestroyComponent(index);
//
//				if (destoryIndex - 1 >= 1)
//				{
//					--destoryIndex;
//				}
//			}
//		}
//
//		// ���� �� ������Ʈ�� �޽� ���Ͱ�
//		else if (componentName == "class MeshFilter")
//		{
//			index = manager->GetTypeIndex("class MeshRenderer");
//
//			// ¦���� �޽� �������� ������ �ִٸ� ���� ���� �����ش�.
//			if (m_selectEntity.lock()->HasComponent(index))
//			{
//				m_selectEntity.lock()->DestroyComponent(index);
//
//				if (destoryIndex - 1 >= 1)
//				{
//					--destoryIndex;
//				}
//			}
//		}
//	}
//}

void InspectorGui::ComponentCombo()
{
	static int addIndex = 0;
	std::string componentName = StringUtils::EraseSpace(m_componentStringList[addIndex]);

	if (ImGui::BeginCombo("##AddComponentCombo", componentName.c_str()))
	{
		// ������Ʈ �˻� â
		SearchComponentCombo();

		// ������Ʈ ����Ʈ ��ȸ
		for (int i = 0; i < m_componentStringList.size(); ++i)
		{
			// �� �׸� ���� ���� ������ ��ư�� ����
			const bool isSeleted = (addIndex == i);

			// ���÷������� ���� ǮŸ�� �̸����� ���� ������Ʈ �̸����� ��ȯ
			componentName = StringUtils::EraseSpace(m_componentStringList[i]);

			// ������Ʈ �̸� �빮�� -> �ҹ��ڷ� ��ȯ
			std::string lowerName;
			for (int j = 0; j < componentName.size(); ++j)
			{
				lowerName.push_back(std::tolower(componentName[j]));
			}
			// �ҹ��ڷ� ��ȯ��Ų ������Ʈ �̸����� �˻��� �̸��� �ִ��� ã��
			if (lowerName.find(m_searchComponentName) == std::string::npos)
			{
				continue;
			}

			// �����ߴٸ� ������ �ε����� ����
			if (ImGui::Selectable(componentName.c_str(), isSeleted))
			{
				addIndex = i;
			}

			// ���õ� �׸� üũ ��ũ ǥ��
			if (isSeleted)
			{
				ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
			}
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
	{
		std::string& componentName = m_componentStringList[addIndex];
		TypeInfoManager* manager = TypeInfoManager::GetInstance();
		TypeInfo* typeInfo = manager->GetTypeInfo(componentName);
		std::type_index typeIndex = manager->GetTypeIndex(componentName);

		// Ư�� ������Ʈ�� � ������Ʈ�� �Բ� �־�߸� �ɷ��� ����� ģ���� �ִ�.
		// ������ 2�� ��ư ���������� �� ���� ������Ű�� ���� mate ������
		TypeInfo* typeInfoMate = nullptr;
		std::type_index typeIndexMate = MakeTypeIndexEmpty();
		std::string mateName = "";

		// �߰� �� ������Ʈ�� �޽� ��������
		if (componentName == "class MeshRenderer")
		{
			typeIndexMate = manager->GetTypeIndex("class MeshFilter");

			// ¦���� �޽� ���͸� ������ ���� �ʴٸ� 
			if (!m_selectEntity.lock()->HasComponent(typeIndexMate))
			{
				// �޽� ���͵� �߰��� �ֱ� ���� Ÿ�� ��ü�� �����´�.
				typeInfoMate = manager->GetTypeInfo("class MeshFilter");
			}
		}

		/// UIRenderer ������ �ּ�
		//// �߰� �� ������Ʈ�� �޽� ���Ͱ�
		//else if (componentName == "class MeshFilter")
		//{
		//	typeIndexMate = manager->GetTypeIndex("class MeshRenderer");

		//	// ¦���� �޽� �������� ������ ���� �ʴٸ� 
		//	if (!m_selectEntity.lock()->HasComponent(typeIndexMate))
		//	{
		//		// �޽� �������� �߰��� �ֱ� ���� Ÿ�� ��ü�� �����´�.
		//		typeInfoMate = manager->GetTypeInfo("class MeshRenderer");
		//	}
		//}

		// �ݶ��̴��� �ߺ��ؼ� ������ �� ������ �Ѵ�.
		// ���� �� ����
		// �ڵ�ȭ�� �ڵ忡 Ư�� ���ܰ� ����ϱ� ������ �þ�� ��
		// �� ����� �ð��� �������� ��������..
		bool monster = true;
		if (componentName == "class BoxCollider" || componentName == "class CapsuleCollider" || componentName == "class SphereCollider" || componentName == "class PlaneCollider")
		{
			if (m_selectEntity.lock()->HasComponent<BoxCollider>() ||
				m_selectEntity.lock()->HasComponent<CapsuleCollider>() ||
				m_selectEntity.lock()->HasComponent<SphereCollider>() ||
				m_selectEntity.lock()->HasComponent<PlaneCollider>())
			{
				monster = false;

				CreateOpenPopup("Error : AddComponent##collider");

				ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing);
			}
		}
		else if (componentName == "class GuardFSM" || componentName == "class PlayerFSM" || componentName == "class ShopKeeperFSM")
		{
			if (m_selectEntity.lock()->HasComponent<GuardFSM>() ||
				m_selectEntity.lock()->HasComponent<PlayerFSM>() ||
				m_selectEntity.lock()->HasComponent<ShopKeeperFSM>())
			{
				monster = false;

				CreateOpenPopup("Error : AddComponent##fsm");

				ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing);
			}
		}

		// �ߺ��� ������Ʈ�� �߰����� �ʴ´�.
		if (!m_selectEntity.lock()->HasComponent(typeIndex) && monster)
		{
			/// ������Ʈ �߰�
			const auto& component = typeInfo->InvokdDefaultConstructor();
			m_selectEntity.lock()->AddComponent(component, typeIndex);
			component->Start();

			std::shared_ptr<Component> componentMate = nullptr;

			// ¦�� ������Ʈ�� �ִٸ� �߰��Ѵ�.
			if (typeInfoMate)
			{
				componentMate = typeInfoMate->InvokdDefaultConstructor();
				m_selectEntity.lock()->AddComponent(componentMate, typeIndexMate);

				componentMate->Start();
			}

			//// �߰��� ������Ʈ�� �ݶ��̴� Ŭ�����ų� FSM Ŭ������� Start �Լ��� ȣ�� �����ش�. (���ϸ� ����)
			//if (std::dynamic_pointer_cast<FSM>(component) != nullptr)
			//{
			//	component->Start();
			//}

			//// �𷺼ųΰ� ����Ʈ����Ʈ�� �׷��Ƚ��� ī�޶��� �����ϰ��ִ�. (Start �Լ�)
			//else if (componentName == "class DirectionalLight" || componentName == "class PointLight")
			//{
			//	component->Start();
			//}
		}

		// RayCaster �� ������ �߰� �� �� �ִ�.
		else if (componentName == "class RayCaster")
		{
			const auto& component = typeInfo->InvokdDefaultConstructor();
			m_selectEntity.lock()->AddComponent(component, typeIndex);

			component->Start();
		}

		else if (monster)
		{
			CreateOpenPopup("Error : AddComponent");

			ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing);
		}
	}

	CreateBeginPopup("Error : AddComponent##collider", "You can only have one collider regardless of the type.", "OK", { 120.f, 30.f }, { 100.f, 0.f });
	CreateBeginPopup("Error : AddComponent##fsm", "You can only have one FSM regardless of the type.", "OK", { 120.f, 30.f }, { 80.f, 0.f });
	CreateBeginPopup("Error : AddComponent", "This Component already exists.", "OK", { 120.f, 30.f }, { 30.f, 0.f });
}

void InspectorGui::ScriptCombo()
{
	static int addIndex = 0;
	std::string scriptName = StringUtils::EraseSpace(m_scriptStringList[addIndex]);

	if (ImGui::BeginCombo("##AddScriptCombo", scriptName.c_str()))
	{
		// ������Ʈ �˻� â
		SearchComponentCombo();

		// ������Ʈ ����Ʈ ��ȸ
		for (int i = 0; i < m_scriptStringList.size(); ++i)
		{
			// �� �׸� ���� ���� ������ ��ư�� ����
			const bool isSeleted = (addIndex == i);

			// ���÷������� ���� ǮŸ�� �̸����� ���� ������Ʈ �̸����� ��ȯ
			scriptName = StringUtils::EraseSpace(m_scriptStringList[i]);

			// ������Ʈ �̸� �빮�� -> �ҹ��ڷ� ��ȯ
			std::string lowerName;
			for (int j = 0; j < scriptName.size(); ++j)
			{
				lowerName.push_back(std::tolower(scriptName[j]));
			}
			// �ҹ��ڷ� ��ȯ��Ų ������Ʈ �̸����� �˻��� �̸��� �ִ��� ã��
			if (lowerName.find(m_searchComponentName) == std::string::npos)
			{
				continue;
			}

			// �����ߴٸ� ������ �ε����� ����
			if (ImGui::Selectable(scriptName.c_str(), isSeleted))
			{
				addIndex = i;
			}

			// ���õ� �׸� üũ ��ũ ǥ��
			if (isSeleted)
			{
				ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
			}
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Script"))
	{
		std::string& componentName = m_scriptStringList[addIndex];
		TypeInfoManager* manager = TypeInfoManager::GetInstance();
		TypeInfo* typeInfo = manager->GetTypeInfo(componentName);

		auto index = manager->GetTypeIndex(componentName);

		// �ߺ��� ������Ʈ�� �߰����� �ʴ´�.
		if (!m_selectEntity.lock()->HasComponent(index))
		{
			const auto& component = typeInfo->InvokdDefaultConstructor();
			m_selectEntity.lock()->AddComponent(component, index);
		}
		else
		{
			CreateOpenPopup("Error : AddScript");

			ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing);
		}
	}

	CreateBeginPopup("Error : AddScript", "This Script already exists.", "OK", { 120.f, 30.f }, { 10.f, 0.f });
}

/// ���� �ϵ� �ڵ� 
/// �Լ� ���÷��� ������ ������ �Ѵ�
/// �����Ϳ��� �ݶ��̴� �� ���� �� PSHX �ʿ� ������Ʈ ���������.
void InspectorGui::CheckColliderTypeAndInvokeFunction(const Property* property)
{
	// �ݶ��̴� �Լ� ȣ��
	if (property->GetOwnerTypeInfo().GetName() == "Collider")
	{
		if (m_currentChildComponentName == "BoxCollider")
		{
			m_selectEntity.lock()->GetComponent<BoxCollider>()->Collider::UpdatePhysXProperties();
		}
		else if (m_currentChildComponentName == "SphereCollider")
		{
			m_selectEntity.lock()->GetComponent<SphereCollider>()->Collider::UpdatePhysXProperties();
		}
		else if (m_currentChildComponentName == "CapsuleCollider")
		{
			m_selectEntity.lock()->GetComponent<CapsuleCollider>()->Collider::UpdatePhysXProperties();
		}
		else if (m_currentChildComponentName == "PlaneCollider")
		{
			m_selectEntity.lock()->GetComponent<PlaneCollider>()->Collider::UpdatePhysXProperties();
		}
		else
		{
			// ReadComponentInformation �Լ� ������ super �б� �÷����ϰ� 
			// ���⼭ elseif ���Ʒ��� �Ȱ��� �÷������ 
			assert(!"�ݶ��̴� �б� �߰� - ���� (���� �ϵ��ڵ�)");
		}
	}
	// �ݶ��̴��� ��ӹ޴� �ڽ�Ŭ���� �Լ� ȣ�� 
	else if (property->GetOwnerTypeInfo().GetName() == "BoxCollider")
	{
		m_selectEntity.lock()->GetComponent<BoxCollider>()->UpdatePhysXProperties();
	}
	else if (property->GetOwnerTypeInfo().GetName() == "SphereCollider")
	{
		m_selectEntity.lock()->GetComponent<SphereCollider>()->UpdatePhysXProperties();
	}
	else if (property->GetOwnerTypeInfo().GetName() == "CapsuleCollider")
	{
		m_selectEntity.lock()->GetComponent<CapsuleCollider>()->UpdatePhysXProperties();
	}
	else if (property->GetOwnerTypeInfo().GetName() == "PlaneCollider")
	{
		m_selectEntity.lock()->GetComponent<PlaneCollider>()->UpdatePhysXProperties();
	}
}
