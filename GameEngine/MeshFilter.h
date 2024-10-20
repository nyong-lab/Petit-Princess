#pragma once
#include "Component.h"

#include <vector>
#include <string>

#include "GraphicsTransfer.h"

class IGraphics;

/// <summary>
/// 게임엔진에서 메쉬는
/// 그래픽스 엔진의 리소스 KEY를 들고 있는다
/// 
/// 2024.01.24
/// 박연하
/// 
/// 수정 : 김동일 -> 생성자에 매니저들이 들어간 컴포넌트들은 씬 로드 하는 과정에서 
/// 기본 생성자로 컴포넌트를 만들기 때문에 추가로 그래픽스와 카메라를 밀어넣어주는 함수 구현 
/// 
/// 수정 : 김동일 -> 에디터에서 모델키를 변경했을 시 모델키에 맞는 SetModelKey를 해주는 업데이트 함수 추가
/// + 메쉬필터는 생성자에서 그래픽스를 받아오기에 에디터에서 이 컴포넌트를 추가할 때 그래픽스를 밀어넣어 줘야한다.
/// 
/// 수정 : 김동일 -> 클래스 내부 PBRSet 구조체 에디터에서 쓸 Get 함수 생성
/// </summary>
class MeshFilter : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MeshFilter)
		PROPERTY(m_cloneModelKey)
		PROPERTY(m_pbrSet)
		PROPERTY(m_cullingBox)
		PROPERTY(m_cullingOff)

	// gui 클래스에서 PBRSet 구조체를 변수로 들고있어야 하기에 public 선언
public:
	struct PBRSet
	{
		GENERATE_CLASS_TYPE_INFO(PBRSet)
			PROPERTY(useBaseColorMap)
			PROPERTY(useNormalMap)
			PROPERTY(useMetallicMap)
			PROPERTY(useRoughnessMap)
			PROPERTY(useAOMap)
			PROPERTY(baseColor)
			PROPERTY(metallic)
			PROPERTY(roughness)
			PROPERTY(AO)			

			// 시리얼라이즈
			NLOHMANN_DEFINE_TYPE_INTRUSIVE(PBRSet, useBaseColorMap, useNormalMap,
				useMetallicMap, useRoughnessMap, metallic, roughness )
			void Serialize(nlohmann::json& json);
		void Deserialize(const nlohmann::json& json);

		// 시리얼라이즈 할 때 private 멤버에 접근하기 위한 friend 선언
		friend class MeshFilter;

		// 위에 리플렉션 매크로에서 private 설정을 하기에 다시 public 선언
	public:
		void SetBaseColor(Vector4 color) { baseColor = Vector4(color.x / 255, color.y / 255, color.z / 255, color.w / 255); }

		bool useBaseColorMap = true;
		bool useNormalMap = false;
		bool useMetallicMap = false;
		bool useRoughnessMap = false;
		bool useAOMap = false;
		Vector4 baseColor = Vector4(1.f, 1.f, 1.f, 1.f);
		float metallic = 0.f;
		float roughness = 1.f;
		float AO = 1.f;
	};

private:
	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MeshFilter, m_name, m_UID, m_modelKey, m_cloneModelKey
	, m_cullingOff)
	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	MeshFilter();
	MeshFilter(const std::string& name);
	virtual ~MeshFilter() {}

public:
	// 에디터 GetSet 할 때 인스턴스 원본 주소를 그대로 가지고 있어야해서 포인터로 가져왔어요
	PBRSet* GetPBRSet() { return &m_pbrSet; }
	PBRSet GetMaterial() { return m_pbrSet; }

	void SetModelKey(GraphicsTransfer::ModelKey modelKey);
	std::wstring GetModelKey() { return m_modelKey; }

	std::shared_ptr<Component> Clone() const override;

	void UpdateModelKey();

public:
	// Graphics Key
	std::wstring m_modelKey = L"";

	// Clone을 위한 원본 키
	GraphicsTransfer::ModelKey m_cloneModelKey;

	PBRSet m_pbrSet;
	Vector2 m_cullingBox = Vector2(10,10);
	bool m_cullingOff = false;
};
