#pragma once
#include "Component.h"

#include <vector>
#include <string>

#include "GraphicsTransfer.h"

class IGraphics;

/// <summary>
/// ���ӿ������� �޽���
/// �׷��Ƚ� ������ ���ҽ� KEY�� ��� �ִ´�
/// 
/// 2024.01.24
/// �ڿ���
/// 
/// ���� : �赿�� -> �����ڿ� �Ŵ������� �� ������Ʈ���� �� �ε� �ϴ� �������� 
/// �⺻ �����ڷ� ������Ʈ�� ����� ������ �߰��� �׷��Ƚ��� ī�޶� �о�־��ִ� �Լ� ���� 
/// 
/// ���� : �赿�� -> �����Ϳ��� ��Ű�� �������� �� ��Ű�� �´� SetModelKey�� ���ִ� ������Ʈ �Լ� �߰�
/// + �޽����ʹ� �����ڿ��� �׷��Ƚ��� �޾ƿ��⿡ �����Ϳ��� �� ������Ʈ�� �߰��� �� �׷��Ƚ��� �о�־� ����Ѵ�.
/// 
/// ���� : �赿�� -> Ŭ���� ���� PBRSet ����ü �����Ϳ��� �� Get �Լ� ����
/// </summary>
class MeshFilter : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MeshFilter)
		PROPERTY(m_cloneModelKey)
		PROPERTY(m_pbrSet)
		PROPERTY(m_cullingBox)
		PROPERTY(m_cullingOff)

	// gui Ŭ�������� PBRSet ����ü�� ������ ����־�� �ϱ⿡ public ����
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

			// �ø��������
			NLOHMANN_DEFINE_TYPE_INTRUSIVE(PBRSet, useBaseColorMap, useNormalMap,
				useMetallicMap, useRoughnessMap, metallic, roughness )
			void Serialize(nlohmann::json& json);
		void Deserialize(const nlohmann::json& json);

		// �ø�������� �� �� private ����� �����ϱ� ���� friend ����
		friend class MeshFilter;

		// ���� ���÷��� ��ũ�ο��� private ������ �ϱ⿡ �ٽ� public ����
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
	// ������ GetSet �� �� �ν��Ͻ� ���� �ּҸ� �״�� ������ �־���ؼ� �����ͷ� �����Ծ��
	PBRSet* GetPBRSet() { return &m_pbrSet; }
	PBRSet GetMaterial() { return m_pbrSet; }

	void SetModelKey(GraphicsTransfer::ModelKey modelKey);
	std::wstring GetModelKey() { return m_modelKey; }

	std::shared_ptr<Component> Clone() const override;

	void UpdateModelKey();

public:
	// Graphics Key
	std::wstring m_modelKey = L"";

	// Clone�� ���� ���� Ű
	GraphicsTransfer::ModelKey m_cloneModelKey;

	PBRSet m_pbrSet;
	Vector2 m_cullingBox = Vector2(10,10);
	bool m_cullingOff = false;
};
