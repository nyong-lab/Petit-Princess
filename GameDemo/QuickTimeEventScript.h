#pragma once
#include "Script.h"
#include "../GameDemo/MinigameDefine.h"

#include "MinigameDefine.h"

/// <summary>
/// 2024.02.14
/// �̴ϰ��� �÷��� ���� �� �����Ǵ� QTE ��ũ��Ʈ.
/// FoodTableScript��� �̴ϰ��� ������ Ŭ������ �̴ϰ��� ���¸� �����ϸ�
/// �÷��̾ �������� Ű�Է¿� ���� ���¸� �� Ŭ������ �����Ѵ�.
/// 
/// �ۼ��� : �ڿ���,�赿��
/// </summary>
class QuickTimeEventScript : public Script
{
	/// �̴ϰ��� �̺�Ʈ ��ũ��Ʈ�� �ø�������� / ���÷��� ���� �ʴ´�.
	void Serialize(nlohmann::json& json) override {}
	void Deserialize(const nlohmann::json& json) override {}

public:
	QuickTimeEventScript() {}
	virtual ~QuickTimeEventScript() {}

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;

	virtual std::shared_ptr<Component> Clone() const override;

	// ó�� �����ɶ� �Ҵ�Ǵ� ��
	void Initialize(std::shared_ptr<MiniGameInfo> miniGameInfo, FoodType foodType, double* merchantSense);

	// ���������� �����Կ� ���� �ش� ���������� �´� �̴ϰ��� ���� �Ҵ�
	void SetMiniGameInfo(std::shared_ptr<MiniGameInfo> miniGameInfo);

	// ��ư ������ �߽� ��ǥ���� ���� ���ϱ� (progress clear ����)
	void CalculateButtonRange(Vector2 pos);

	// ���� Űü�� �����
	// Űü���� �ִ� ������������ m_keyChains�� ����� 0���� üũ�ϰ�, ����� 0�̶�� �� �Լ��� ȣ���Ѵ�. 
	// ���������� �������� FoodTableScirpt���� SetMiniGameInfo�Լ��� ȣ�� �����ִµ�
	// �� ��, keyChains ���͸� �ʱ�ȭ �����ֱ⿡ ����� 0�̵ǰ� �ٽ� Ű ü���� �ʿ��� ������ ������ش�.
	void MakeRandomKeyChains(int keyChainNum);

	// Űü�� Ÿ��
	// �Ű������� ���� int�� m_keychains �迭�� �����ؼ� ���� Ȯ���ϰ� �׿� �´� 
	// ���콺 Ŭ���� ���� progress ���� ���θ� üũ�Ѵ�.
	void CheckKeyChain(int keyChainLocation);

	// ����Ʈ Ÿ��
	// ���콺 Ŭ�� ��ġ�� ���� ���� ���θ� üũ�Ѵ�.
	void CheckPoint(Vector2 nextUIpos);

	// �巡�� Ÿ�� 
	// �巡�� ���� ��ġ�� ��ǥ�� ������ ������ üũ�Ѵ�.
	bool CheckDragStartPoint(Vector2 startDragPos);

	// ���콺 �巡�� ������ ����Ͽ� ���� ���θ� üũ�Ѵ�.
	void CheckDrag(Vector2 directuonStartPos);
	bool CheckDragValid(const Vector2& start, const Vector2& end);
	bool CalculateDragAngle(float angle);

	// �巡�� �߽� ��ǥ���� ���� ���ϱ� (�巡�� ���� ��ġ �ν� ����)
	void CalculateDragStartRange(Vector2 pos);

	// �̴ϰ��� ���� �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
	void Bagle();
	void BaguettePoint();
	void CupCake();
	void Crab();
	void Apple();
	void CartoonMeat();
	void Ham();
	void WaterMelon();
	void Fish();
	void Squid();
	void Cake();
	void Sausage();
	void BaguetteWheel();
	void Banana();

public: 
	// ���̴��� ������ ������
	std::shared_ptr<MiniGameInfo> GetMiniGameInfo() { return m_miniGameInfo; }
	FoodType GetFoodType() { return m_foodType; }
	MiniGameType GetMiniGameType() { return m_miniGameType; }
	std::vector<int> GetKeyChains() { return m_keychains; }
	Vector2 GetUIPosition() { return m_uiPosition; }
	Vector2 GetUISize() { return m_uiSize; }
	DragDirection GetDragDirection() { return m_direction; }

	void SetMiniGameType(MiniGameType type);

private:
	std::shared_ptr<MiniGameInfo> m_miniGameInfo;
	FoodType m_foodType = FoodType::None;
	MiniGameType m_miniGameType;

	Vector2 m_uiPosition = Vector2(1920.f/2.f, 1080.f/2.f);
	Vector2 m_uiSize = Vector2(70.f, 70.f);
	Vector2 m_buttonSize = Vector2(70.f, 70.f);
	Vector2 m_leftTop;
	Vector2 m_rightBottom;
	DragDirection m_direction;

	// Ű ü���� 3���� ���ڸ� ��� ���� �� �ִ�.
	// 0 : ���콺 �� Ŭ��
	// 1 : ���콺 �� Ŭ��
	// 2 : Ŭ���� �� Űü��
	std::vector<int> m_keychains;
	KeyChainUIpos m_keychainsPosInfo;

	// �巡�� Ÿ��
	Vector2 m_dragStartPos;
	Vector2 m_dragEndPos;
	bool m_isDragging = false;
	bool m_isFailedStartPoint = false;

	// ���� ��ġ������ 
	double* m_merchantSense = nullptr;
	bool m_isMistakePoint = false;
	bool m_isMistakeKeyChain = false;
	bool m_isMakeKeyChain = false;
};

