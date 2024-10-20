#pragma once
#include "FSM.h"

class ShopKeeperFSM : public FSM, public std::enable_shared_from_this<ShopKeeperFSM>
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(ShopKeeperFSM)
	//PROPERTY(m_curEnumState)
	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ShopKeeperFSM, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	enum class ShopKeeper
	{
		idle,
		calling,
		doubt
	};

	void Start();

	virtual void OnStateEnter() override;	//���� ���Խ� �ѹ��� �߻��ϴ� �Լ�
	virtual void OnStateExit() override;	//���� ��ȭ�� �߻��ϴ� �Լ�

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	void ChangeState(ShopKeeperFSM::ShopKeeper state);

	virtual std::shared_ptr<Component> Clone() const override;

	ShopKeeper GetEnumState() const { return m_curEnumState; }

	// Ǫ�� ���̺� ��ũ��Ʈ�� �����ϱ� ���ؼ� ���
	double* GetPointerSense();
	double GetSense();

private:
	std::unordered_map<ShopKeeperFSM::ShopKeeper, std::shared_ptr<State>> m_shopkeeperStateList;
	ShopKeeper m_curEnumState;

	double m_sense = 0;
};
