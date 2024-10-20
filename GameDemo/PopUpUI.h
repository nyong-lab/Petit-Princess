#pragma once
#include "Script.h"

/// <summary>
/// �ռ���
/// 
/// �ΰ��� �˾� UI
/// </summary>
class PopUpUI : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(PopUpUI)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PopUpUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	PopUpUI();
	~PopUpUI();

// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------


// ��� ���� ----------------------------------------------------------------------------------------
private:
	std::weak_ptr<IGraphics> m_igraphics;

public:
	Vector2 m_pos = Vector2(1920 / 2,1080 / 2);
	Vector2 m_size = Vector2(1920, 1080);

	bool m_isOn = false;
};