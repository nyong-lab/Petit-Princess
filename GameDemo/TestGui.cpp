#include "GameDemo_pch.h"
#include "TestGui.h"

#include "CameraManager.h"
#include "CameraModule.h"
#include "InputManager.h"
#include "SceneManager.h"

TestGui::TestGui()
{

}

void TestGui::Initialize(std::weak_ptr<CameraManager> cameraManager, std::weak_ptr<SceneManager> sceneManager)
{
	m_cameraManager = cameraManager;
	m_sceneManager = sceneManager;

	m_enumClassStringList["ProjectionMode"] = { "Perspective", "Orthographic" };
}

void TestGui::Update()
{
	m_mainCamera = m_cameraManager.lock()->GetMainCamera();
	ShowTest();
}

void TestGui::ShowTest()
{
	ShowDebug();
	ShowCamera();
}

void TestGui::ShowDebug()
{
	ImGui::Begin("Debug");

	Vector3 cameraPos = m_cameraManager.lock()->GetMainCamera()->GetPosition();
	ImGui::Text("camera position : %f, %f %f", cameraPos.x, cameraPos.y, cameraPos.z);

	Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();
	ImGui::Text("mouse position : %d, %d", static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));

	std::wstring wSceneName = m_sceneManager.lock()->GetCurScene()->GetSceneName();
	std::string sceneName = "Scene Name : " + StringUtils::ToString(wSceneName);
	ImGui::Text(sceneName.c_str());

	ImGui::End();
}

void TestGui::ShowCamera()
{
	ImGui::Begin("Camera");

	TypeInfoManager* manager = TypeInfoManager::GetInstance();
	const auto& typeInfo = manager->GetTypeInfo("class CameraModule");

	if (ImGui::TreeNode("CameraModule"))
	{
		const auto& propertys = typeInfo->GetProperties();
		for (const auto* property : propertys)
		{
			ReadPropertyInformation(property);
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void TestGui::ReadPropertyInformation(const Property* property)
{
	switch (property->GetPropertyTypeCategory())
	{
	case PropertyTypeCategory::Primitive:
	{
		ReadPrimitiveType(property);
		break;
	}
	case PropertyTypeCategory::EnumClass:
	{
		ReadEnumClassType(property);
		break;
	}
	default:
	{
		assert(false, R"(Tpye에 대한 정보가 없습니다.
			리플렉션한 타입에 대해서 PropertyRegister 분기로 카테고리를 추가로 나눠야하거나,
			위에 switch case를 늘려주세요)");
		break;
	}
	}
}

void TestGui::ReadPrimitiveType(const Property* property)
{
	std::string fullTypeName = property->GetTypeInfo().GetFullName();

	std::string fureName = property->GetName();
	fureName = StringUtils::EraseUnderbar(fureName);

	if (fullTypeName == "struct DirectX::SimpleMath::Vector3")
	{
		fureName = StringUtils::EraseLocal(fureName);

		Vector3 vector = property->Get<Vector3>(m_mainCamera.lock().get()).GetValue();
		float fArray[] = { vector.x, vector.y, vector.z };

		if (ImGui::DragFloat3(fureName.c_str(), fArray, 0.1f))
		{
			DirectX::SimpleMath::Vector3 setVector = { fArray[0], fArray[1], fArray[2] };
			property->Set<Vector3>(m_mainCamera.lock().get(), setVector);
		}
	}

	else if (fullTypeName == "struct DirectX::SimpleMath::Vector2")
	{
		fureName = StringUtils::EraseLocal(fureName);

		Vector2 vector = property->Get<DirectX::SimpleMath::Vector2>(m_mainCamera.lock().get()).GetValue();
		float fArray[] = { vector.x, vector.y };

		if (ImGui::DragFloat2(fureName.c_str(), fArray, 0.1f))
		{
			Vector2 setVector = { fArray[0], fArray[1] };
			property->Set<Vector2>(m_mainCamera.lock().get(), setVector);
		}
	}

	else if (fullTypeName == "float")
	{
		ImGui::DragFloat(fureName.c_str(), &property->Get<float>(m_mainCamera.lock().get()).GetValue(), 0.1f);
	}

	else
	{
		assert(!"리플렉션한 PrimitiveType 분기를 늘려야 합니다.  - Dongil");
	}
}

void TestGui::ReadEnumClassType(const Property* property)
{
	std::string fullTypeName = property->GetTypeInfo().GetFullName();
	fullTypeName = StringUtils::EraseSpace(fullTypeName);

	std::string fureName = property->GetName();
	fureName = StringUtils::EraseUnderbar(fureName);

	int currentIndex = 0; // enum에 해당하는 배열의 인덱스

	if (fullTypeName == "ProjectionMode")
	{
		currentIndex = static_cast<int>(property->Get<ProjectionMode>(m_mainCamera.lock().get()).GetValue());
	}
	else
	{
		assert(!"리플렉션한 enum class 에 대응하는 분기를 추가 해야합나다. - 동일");
	}

	std::vector<std::string> memberNames{}; // 멤버들 이름

	// 맵을 순회한다.
	for (auto& iter : m_enumClassStringList)
	{
		// 맵의 키 값이 해당하는 enum class 라면
		if (iter.first == fullTypeName)
		{
			// enum class를 순회한다.
			for (auto& string : iter.second)
			{
				memberNames.push_back(string.c_str());
			}
		}
	}

	if (ImGui::BeginCombo(fureName.c_str(), memberNames[currentIndex].c_str()))
	{
		for (int i = 0; i < memberNames.size(); ++i)
		{
			// 각 항목에 대한 선택 가능한 버튼을 생성
			const bool isSelected = (currentIndex == i);
			if (ImGui::Selectable(memberNames[i].c_str(), isSelected))
			{
				currentIndex = i;
			}

			// 선택된 항목에 체크 마크 표시
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus(); // 선택된 항목에 초점을 맞춤
			}
		}

		ImGui::EndCombo();
	}


	/// ProjectionMode
	if (fullTypeName == "ProjectionMode")
	{
		// m_enumClassStringList["ProjectionMode"] = { "Perspective", "Orthographic"};

		switch (currentIndex)
		{
		case 0:
			property->Set<ProjectionMode>(m_mainCamera.lock().get(), ProjectionMode::Perspective);
			break;
		case 1:
			property->Set<ProjectionMode>(m_mainCamera.lock().get(), ProjectionMode::Orthographic);
			break;

		default:
			assert(!"case 분기 늘려주세요. (ProjectionMode) - 동일");
			break;
		}
	}
	else
	{
		assert(!"리플렉션한 enum class 에 대응하는 분기를 추가 해야합나다. - 동일");
	}
}

