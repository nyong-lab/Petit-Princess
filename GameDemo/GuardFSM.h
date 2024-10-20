#pragma once
#include "FSM.h"
#include <stack>


/// <summary>
/// test�� ��� FSM
/// 
/// </summary>
class GuardFSM : public FSM, public std::enable_shared_from_this<GuardFSM>
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(GuardFSM)
	PROPERTY(m_combackPoint)
	//PROPERTY(m_currentState)
	//PROPERTY(m_preState)
	PROPERTY(m_patrolPath)	//std::vector<Vector3> �̰ŷ� �����Ϳ��� �������ٰǰ�?
	PROPERTY(cheat)
	PROPERTY(m_guardSpeed)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GuardFSM, m_name, m_UID)
	void Serialize(nlohmann::json& json) override;
	void Deserialize(const nlohmann::json& json) override;

public:

	enum class Guard
	{
		idle = 0,
		patrol,
		chasing,
		called,
		search,
		fishinged,
		comeback,
		set,

		end
	};

	GuardFSM();
	~GuardFSM();

	void Start();

	virtual void OnStateEnter() override;	//���� ���Խ� �ѹ��� �߻��ϴ� �Լ�
	virtual void OnStateExit() override;	//���� ��ȭ�� �߻��ϴ� �Լ�

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	void ChangeState(GuardFSM::Guard state);

	virtual std::shared_ptr<Component> Clone() const override;

	void SetDestination(Vector3 destination);
	void SetPatrolPath(Vector3 destination);
	Guard GetCurState() const { return m_currentState; }

	Point CalcPoint(Vector3 point);

	// ������ ����
	void UpdateGuardSpeed();


private:
	std::unordered_map<GuardFSM::Guard, std::shared_ptr<State>> m_guardStateList;
	Guard m_preState;
	Guard m_currentState;

/// A star
	void Astar(Vector3 goal);
	bool IsValid(Vector3 point);
	bool IsValid(Point point);
	double CalcHuristic(Point pos, Point destination);
	void PathTrace(std::stack<AstarNode> path);


	//������ġ
	Vector3 m_curPoint;
	//������
	Vector3 m_destination;
	//���� ����
	Vector3 m_combackPoint;
	//�������
	std::vector<Vector3> m_patrolPath;

	//�׸����
	int mapsize = 151;
	int(*grid)[151];
	//�湮 ���
	bool** visitMap;
	//���� ��带 ���� ��
	AstarNode**map;
	//�������
	std::stack<AstarNode> m_path;

	bool cheat;


	// �����Ϳ� ��� ��� ���ǵ�
	// �� ��������� �ƹ� �ǹ� ������
	// �����Ϳ��� �� ���� �����Ѵٸ� ���� GuardState Ŭ������ �ִ� ���ǵ尡 ����ǵ��� ����
	float m_guardSpeed = 0.f;
};

