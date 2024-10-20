#pragma once
#include "FSM.h"
#include <stack>


/// <summary>
/// test용 경비병 FSM
/// 
/// </summary>
class GuardFSM : public FSM, public std::enable_shared_from_this<GuardFSM>
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(GuardFSM)
	PROPERTY(m_combackPoint)
	//PROPERTY(m_currentState)
	//PROPERTY(m_preState)
	PROPERTY(m_patrolPath)	//std::vector<Vector3> 이거로 에디터에서 지정해줄건가?
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

	virtual void OnStateEnter() override;	//상태 진입시 한번만 발생하는 함수
	virtual void OnStateExit() override;	//상태 변화에 발생하는 함수

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	void ChangeState(GuardFSM::Guard state);

	virtual std::shared_ptr<Component> Clone() const override;

	void SetDestination(Vector3 destination);
	void SetPatrolPath(Vector3 destination);
	Guard GetCurState() const { return m_currentState; }

	Point CalcPoint(Vector3 point);

	// 에디터 전용
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


	//현재위치
	Vector3 m_curPoint;
	//목적지
	Vector3 m_destination;
	//복귀 지점
	Vector3 m_combackPoint;
	//순찰경로
	std::vector<Vector3> m_patrolPath;

	//그리드맵
	int mapsize = 151;
	int(*grid)[151];
	//방문 경로
	bool** visitMap;
	//실제 노드를 담은 맵
	AstarNode**map;
	//최종경로
	std::stack<AstarNode> m_path;

	bool cheat;


	// 에디터에 띄울 경비병 스피드
	// 이 멤버변수는 아무 의미 없지만
	// 에디터에서 이 값을 수정한다면 실제 GuardState 클래스에 있는 스피드가 변경되도록 구현
	float m_guardSpeed = 0.f;
};

