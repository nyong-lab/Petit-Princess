#pragma once
#include <memory>
#include <stack>
#include <limits>


class FSM;

/// <summary>
/// Astar ���� ����ü
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
/// �⺻���� ���¸� ��Ÿ�� Ŭ����
/// �Ļ����� ������ ������Ʈ�� Ư�� ���¸� ������
/// 
/// state design pattern
/// 
/// 
/// �ۼ� ���¿�
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
/// ���� ������ ���¸� ��Ÿ����
/// ��ġ �������� ���� ��ȭ �� ���̴�
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

	//// ShopKeeperState�� ��ӹ޴� ģ������ ���
	//double& GetReferenceSense() { return m_sense; }

	//// Ǫ�� ���̺� ��ũ��Ʈ�� �����ϱ� ���ؼ� ���
	//double* GetPointerSense() { return &m_sense; }
	//double GetSense() { return m_sense; }

	void SetSense(double* sense) { m_sense = sense; }

protected:
	//��ġ ������ - � ���¿� ������ ������ �ϳ��� ��ġ�������� �����ؼ� ����ؾ��Ѵ�
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
	double m_duration = 2; //��ġ ������ �����ð�
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
/// ����� ���¸� ��Ÿ���� Ŭ����
/// ���� ��ȣ�ۿ����� ���°� �ٲ��(ex.������ ȣ��)
/// ���� ���¸� ��Ÿ�� �������̽� Ŭ����
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


	// ��ġ �̵�
	float m_moveSpeed = 10.f; // ���ǵ�

	float m_turnSpeed = 7.f;						// ������ȯ �ӵ�: ���� Ŭ���� ����
	Vector3 m_targetVector = { 0.f, 0.f, 0.f };		// ������ ����: ���� ��ο� ���� �ٷ� ��ȯ�ȴ�
	Vector3 m_headVector = { 0.f, 0.f, 1.f };		// ĳ���Ͱ� �ٶ󺸴� ����: m_targetVector�� ���� �ε巴�� ����

};

/// <summary>
/// ����� �⺻ ����
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
/// ����� ���� ����
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
/// ����� �߰� ����
/// �� ���¿����� called ��ȯ�� �ȵ�
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
/// ����� ���ο��� ȣ�� ���� ����
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
/// ����� �÷��̾ Ž���ϴ� ����
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
/// ����� �÷��̾����� ����
/// ������ ���ǿ� ���� �ٸ� ����?
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
/// ����� ������������ ���ư��� ����
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
/// �߰��� ȣ���� �ұ�
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