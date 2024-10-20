#pragma once
#include <../GameEngine/Component.h>
#include "State.h"




/// <summary>
/// �ϴ� ���¸� �� �����
/// ���ٺ��� �ߺ��Ǵ°͵��ְ� �ƴѰ͵� �ְ�
/// </summary>
//enum class State
//{
//	//������
//	Idle,
//	patrol,
//	chasing,
//	called,
//	chasefinding, //������ �θ���
//	patrolfinding, // ������ �θ���
//	fishinged,
//	comeback,
//
//	//����
//	shopkeeperIdle, //�⺻
//	calling, //������ ȣ��
//	doubt, //��ġ ������ ���� ���� ����
//	angry, //��ġ ������ ���� ���� �̻�
//
//	//�÷��̾�
//	playerIdle,
//	Steel,
//	Eat,
//	hide,
//	fishing, //��� ����
//
//};


/// <summary>
/// 2024.01.31
/// �������� ���¸� �������Ѵ�
/// ���(�Ϲ�) / ���� / �߰� / ȣ�� / ���� / �θ���(�߰��� ��ǥ�� ã�� ����/ ���� �� �����Ÿ� �����̰� ��ǧ�� Ž����) / ������
/// 
/// ��� ĳ���͵��� ���¿� ���� �ٸ� �ִϸ��̼��� �Ұ��̴�
/// ������ ��ġ������������ ���°� ���ҰŰ�
/// �÷��̾�� Ű�Է°� �浹�� ���� ���°� ���Ұ��̴�
/// �ᱹ ���������� fsm�� ���� �ִ� �׷��� �ڵ� �ߺ� �������� Ŭ���� ���� ��ӹ���
/// 
/// state design pattern
/// 
/// �ۼ� ���¿�
/// </summary>
class FSM :
    public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(FSM)

	// Serialize
	void Serialize(nlohmann::json& json) abstract;
	void Deserialize(const nlohmann::json& json) abstract;

public:

	FSM();
	virtual ~FSM();

	virtual void OnStateEnter() abstract; //���� ���Խ� �ѹ��� �߻��ϴ� �Լ�
	virtual void OnStateExit() abstract; //���� ��ȭ�� �߻��ϴ� �Լ�

	virtual void Update(double dt) abstract;
	virtual void Render(double dt) abstract;
	virtual void Finalize() abstract;
	virtual std:: shared_ptr<Component> Clone() const abstract;

	std::shared_ptr<State> GetState() const { return m_curState; }


protected:
	void ChangeState(std::shared_ptr<State> state);
	std::shared_ptr<State> m_curState;

	///fsm condition, transition
	std::vector < std::vector<bool>> m_transition;
};

