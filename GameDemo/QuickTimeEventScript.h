#pragma once
#include "Script.h"
#include "../GameDemo/MinigameDefine.h"

#include "MinigameDefine.h"

/// <summary>
/// 2024.02.14
/// 미니게임 플레이 중일 때 생성되는 QTE 스크립트.
/// FoodTableScript라는 미니게임 관리자 클래스와 미니게임 상태를 공유하며
/// 플레이어에 직접적인 키입력에 따른 상태를 이 클래스가 조작한다.
/// 
/// 작성자 : 박연하,김동일
/// </summary>
class QuickTimeEventScript : public Script
{
	/// 미니게임 이벤트 스크립트는 시리얼라이즈 / 리플렉션 하지 않는다.
	void Serialize(nlohmann::json& json) override {}
	void Deserialize(const nlohmann::json& json) override {}

public:
	QuickTimeEventScript() {}
	virtual ~QuickTimeEventScript() {}

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;

	virtual std::shared_ptr<Component> Clone() const override;

	// 처음 생성될때 할당되는 값
	void Initialize(std::shared_ptr<MiniGameInfo> miniGameInfo, FoodType foodType, double* merchantSense);

	// 스테이지가 증가함에 따라서 해당 스테이지에 맞는 미니게임 정보 할당
	void SetMiniGameInfo(std::shared_ptr<MiniGameInfo> miniGameInfo);

	// 버튼 아이콘 중심 좌표에서 범위 구하기 (progress clear 조건)
	void CalculateButtonRange(Vector2 pos);

	// 랜덤 키체인 만들기
	// 키체인이 있는 스테이지에서 m_keyChains의 사이즈가 0인지 체크하고, 사이즈가 0이라면 이 함수를 호출한다. 
	// 스테이지가 높아지면 FoodTableScirpt에서 SetMiniGameInfo함수를 호출 시켜주는데
	// 이 때, keyChains 벡터를 초기화 시켜주기에 사이즈가 0이되고 다시 키 체인이 필요한 곳에서 만들어준다.
	void MakeRandomKeyChains(int keyChainNum);

	// 키체인 타입
	// 매개변수로 들어온 int로 m_keychains 배열에 접근해서 값을 확인하고 그에 맞는 
	// 마우스 클릭에 따라서 progress 성공 여부를 체크한다.
	void CheckKeyChain(int keyChainLocation);

	// 포인트 타입
	// 마우스 클릭 위치에 따라서 성공 여부를 체크한다.
	void CheckPoint(Vector2 nextUIpos);

	// 드래그 타입 
	// 드래그 시작 위치의 좌표를 가지고서 범위를 체크한다.
	bool CheckDragStartPoint(Vector2 startDragPos);

	// 마우스 드래그 각도를 계산하여 성공 여부를 체크한다.
	void CheckDrag(Vector2 directuonStartPos);
	bool CheckDragValid(const Vector2& start, const Vector2& end);
	bool CalculateDragAngle(float angle);

	// 드래그 중심 좌표에서 범위 구하기 (드래그 시작 위치 인식 범위)
	void CalculateDragStartRange(Vector2 pos);

	// 미니게임 로직 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
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
	// 쉐이더로 데이터 보내기
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

	// 키 체인은 3개의 숫자를 들고 있을 수 있다.
	// 0 : 마우스 좌 클릭
	// 1 : 마우스 우 클릭
	// 2 : 클리어 된 키체인
	std::vector<int> m_keychains;
	KeyChainUIpos m_keychainsPosInfo;

	// 드래그 타입
	Vector2 m_dragStartPos;
	Vector2 m_dragEndPos;
	bool m_isDragging = false;
	bool m_isFailedStartPoint = false;

	// 상인 눈치게이지 
	double* m_merchantSense = nullptr;
	bool m_isMistakePoint = false;
	bool m_isMistakeKeyChain = false;
	bool m_isMakeKeyChain = false;
};

