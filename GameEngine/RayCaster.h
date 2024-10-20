#pragma once
#include "Component.h"
#include "GraphicsTransfer.h"
#include "EntityDefine.h"
#include "GraphicsTransfer.h"

struct RayCastInfo;
class Collider;

class RayCaster : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(RayCaster)
		PROPERTY(m_startPosOffset)
		PROPERTY(m_unitDirOffset)
		PROPERTY(m_distance)
		PROPERTY(m_findMultiple)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RayCaster, m_name, m_UID, m_distance)
	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;


/// �����ڿ� �Ҹ��� -------------------------------------------------------------------------------------------
public:
	RayCaster();
	virtual ~RayCaster();

	virtual std::shared_ptr<Component> Clone() const override;
	virtual void Update(double dt);
	virtual void Render();
	virtual void DebugRender();

/// ��� �Լ� -------------------------------------------------------------------------------------------
public:
	void SetStartPosOffset(Vector3 pos) { m_startPosOffset = pos; }
	void SetDistance(float distance) { m_distance = distance; }
	void SetUnitDirectionOffset(Vector3 dir);
	void SetProperty(Vector3 startPos, Vector3 unitDirection, float distance);

	RayCastInfo RaycastClosest();
	std::vector<RayCastInfo> RaycastMultiple();

/// ��� ���� -------------------------------------------------------------------------------------------
private:
	Vector3 m_startPosOffset;
	Vector3 m_unitDirOffset;
	float m_distance		= 10;

	bool m_findMultiple		= true;		//���ӱ��� ���� ����

	//�߰����� ������
	Vector3 m_rotatedDir;
	Vector3 m_translatedPos;

	/// ** Debug Render **
	bool m_isModelCreate = false;
	std::wstring m_modelKey = L"";
	GraphicsTransfer::ModelKey m_cloneModelKey = GraphicsTransfer::ModelKey::WireFrameRay;		 // Clone�� ���� ���� Ű
};

