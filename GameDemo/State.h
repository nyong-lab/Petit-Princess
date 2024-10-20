#pragma once
#include <memory>
#include <stack>
#include <limits>


class FSM;

/// <summary>
/// Astar 계산용 구조체
/// </summary>
typedef std::pair<int, int> Point;
struct AstarNode
{
	Point pos;
	Point preNode;

	bool isDestinaition;
	bool isStart;

	double F;
	double H;
	double G;

	AstarNode() :pos(-1, -1), isDestinaition(false), isStart(false), F(DBL_MAX), H(DBL_MAX), G(DBL_MAX) {}
	AstarNode(int x, int y) :pos(x, y), isDestinaition(false), isStart(false), F(DBL_MAX), H(DBL_MAX), G(DBL_MAX) {}

	bool operator<(const AstarNode& node) const
	{
		if (this->F < node.F)
		{
			return true;
		}
		else if (this->F == node.F)
		{
			if (this->H < node.H)
			{
				return true;
			}
		}

		return false;
	}
};


/// <summary>
/// 2024.01.31
/// 기본적인 상태를 나타낼 클래스
/// 파생시켜 각각의 오브젝트의 특정 상태를 만들어내자
/// 
/// state design pattern
/// 
/// 
/// 작성 유승운
/// </summary>
class State
{

public:
	State(std::shared_ptr<FSM> fsm);
	virtual ~State();
	virtual void OnStateEnter() abstract;
	virtual void OnStateExit() abstract;
	virtual void Update(double dt) abstract;
	virtual void Finalize() abstract;

protected:
	std::shared_ptr<FSM> m_fsm;
};

#pragma region PlayerState
class PlayerState : public State
{
public:
	PlayerState(std::shared_ptr<FSM> fsm);
	virtual ~PlayerState();
	virtual void OnStateEnter() abstract;
	virtual void OnStateExit() abstract;
	virtual void Update(double dt) abstract;
	virtual void Finalize() abstract;
protected:
	void ReturnIdle();
private:

	static double m_hungry;
};

class PlayerIdle : public PlayerState
{
public:
	PlayerIdle(std::shared_ptr<FSM> fsm);
	virtual ~PlayerIdle();

	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

class PlayerEat : public PlayerState
{
public:
	PlayerEat(std::shared_ptr<FSM> fsm);
	~PlayerEat();

	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

class PlayerHide : public PlayerState
{
public:
	PlayerHide(std::shared_ptr<FSM> fsm);
	~PlayerHide();

	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

class PlayerThrow : public PlayerState
{
public:
	PlayerThrow(std::shared_ptr<FSM> fsm);
	~PlayerThrow();

	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

class PlayeRun : public PlayerState
{
public:
	PlayeRun(std::shared_ptr<FSM> fsm);
	~PlayeRun();

	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

class PlayerLifting: public PlayerState
{
public:
	PlayerLifting(std::shared_ptr<FSM> fsm);
	~PlayerLifting();

	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

#pragma endregion

#pragma region ShopKeeper
/// <summary>
/// 상점 주인의 상태를 나타낸다
/// 눈치 게이지에 따라 변화 할 것이다
/// </summary>
class ShopKeeperState : public State
{
public:
	ShopKeeperState(std::shared_ptr<FSM> fsm);
	virtual ~ShopKeeperState();
	virtual void OnStateEnter() abstract;
	virtual void OnStateExit() abstract;
	virtual void Update(double dt) abstract;
	virtual void Finalize() abstract;

	//// ShopKeeperState를 상속받는 친구들이 사용
	//double& GetReferenceSense() { return m_sense; }

	//// 푸드 테이블 스크립트에 연결하기 위해서 사용
	//double* GetPointerSense() { return &m_sense; }
	//double GetSense() { return m_sense; }

	void SetSense(double* sense) { m_sense = sense; }

protected:
	//눈치 게이지 - 어떤 상태에 들어가더라도 기존의 하나의 눈치게이지만 공유해서 사용해야한다
	double m_maxSense = 100;
	double* m_sense = nullptr;
};

class ShopKeeperIdle : public ShopKeeperState
{
public:
	ShopKeeperIdle(std::shared_ptr<FSM> fsm);
	virtual ~ShopKeeperIdle();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:
	//double m_subCoefficient = 2;
	double m_duration = 2; //눈치 게이지 유지시간
	double m_time = 0;
};

class ShopKeeperCalling : public ShopKeeperState
{
public:
	ShopKeeperCalling(std::shared_ptr<FSM> fsm);
	virtual ~ShopKeeperCalling();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

class ShopKeeperDoubt : public ShopKeeperState
{
public:
	ShopKeeperDoubt(std::shared_ptr<FSM> fsm);
	virtual ~ShopKeeperDoubt();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
private:
	//double addCoefficient = 2;
};
#pragma endregion

#pragma region Guard
/// <summary>
/// 경비병의 상태를 나타내는 클래스
/// 각종 상호작용으로 상태가 바뀐다(ex.상인의 호출)
/// 각종 상태를 나타낼 인터페이스 클래스
/// </summary>
class GuardState : public State
{
public:
	GuardState(std::shared_ptr<FSM> fsm);
	virtual ~GuardState();
	virtual void OnStateEnter() abstract;
	virtual void OnStateExit() abstract;
	virtual void Update(double dt) abstract;
	virtual void Finalize() override;

	void SetPath(std::stack<AstarNode> path);
	float GetMoveSpeed() const { return m_moveSpeed; }
	void SetMoveSpeed(float speed) { m_moveSpeed = speed; }

protected:
	void Move(double dt);

	std::stack<AstarNode> m_path;

	float m_dirX;
	float m_dirY;

	Vector3 m_curPoint;
	Vector3 m_destination;

	AstarNode prePath;
	AstarNode curPath;


	// 위치 이동
	float m_moveSpeed = 10.f; // 스피드

	float m_turnSpeed = 7.f;						// 방향전환 속도: 숫자 클수록 빠름
	Vector3 m_targetVector = { 0.f, 0.f, 0.f };		// 가려는 방향: 진행 경로에 따라 바로 전환된다
	Vector3 m_headVector = { 0.f, 0.f, 1.f };		// 캐릭터가 바라보는 방향: m_targetVector를 토대로 부드럽게 계산됨

};

/// <summary>
/// 경비병의 기본 상태
/// </summary>
class GuardIdle : public GuardState
{
public:
	GuardIdle(std::shared_ptr<FSM> fsm);
	virtual ~GuardIdle();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:
};

/// <summary>
/// 경비병의 순찰 상태
/// </summary>
class GuardPatrol : public GuardState
{
public:
	GuardPatrol(std::shared_ptr<FSM> fsm);
	virtual ~GuardPatrol();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:
};

/// <summary>
/// 경비병의 추격 상태
/// 이 상태에서는 called 변환이 안됨
/// </summary>
class GuardChasing : public GuardState
{
public:
	GuardChasing(std::shared_ptr<FSM> fsm);
	virtual ~GuardChasing();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
private:
	float m_timeUntilNextWhistle = -1.f;
};

/// <summary>
/// 경비병이 상인에게 호출 받은 상태
/// </summary>
class GuardCalled : public GuardState
{
public:
	GuardCalled(std::shared_ptr<FSM> fsm);
	virtual ~GuardCalled();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:

};

/// <summary>
/// 경비병이 플레이어를 탐색하는 상태
/// </summary>
class GuardSearch : public GuardState
{
public:
	GuardSearch(std::shared_ptr<FSM> fsm);
	virtual ~GuardSearch();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:
	double m_duration;
};

/// <summary>
/// 경비병이 플레이어한테 낚임
/// 던지는 물건에 따라 다른 상태?
/// </summary>
class GuardFishinged : public GuardState
{
public:
	GuardFishinged(std::shared_ptr<FSM> fsm);
	virtual ~GuardFishinged();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:
	bool isFishinged;
};

/// <summary>
/// 경비병이 원래구역으로 돌아가는 상태
/// </summary>
class GuardComeBack : public GuardState
{
public:
	GuardComeBack(std::shared_ptr<FSM> fsm);
	virtual ~GuardComeBack();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;
};

/// <summary>
/// 추격전 호루라기 불기
/// </summary>
class GuardSet : public GuardState
{
public:
	GuardSet(std::shared_ptr<FSM> fsm);
	virtual ~GuardSet();
	virtual void OnStateEnter() override;
	virtual void OnStateExit() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

private:
	double m_duration = 0;
};


#pragma endregion