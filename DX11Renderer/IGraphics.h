#pragma once

#include <windows.h>
#include <memory>
#include <string>
#include <vector>
#include <any>

#include "GraphicsTransfer.h"
using namespace GraphicsTransfer;

class IDevice;
class IGraphics;
class CameraModule;

/// <summary>
/// 그래픽스 인터페이스
/// 
/// 24.01.18
/// 박연하
/// </summary>
class IGraphics
{
public:
	virtual ~IGraphics() = default;

public:
// 생성자와 소멸자 ---------------------------------------------------------------------------------
	virtual void Initialize(HWND hwnd) abstract;
	virtual void Finalize() abstract;

// 그래픽스 엔진 업데이트 ---------------------------------------------------------------------------
	virtual void Resize(int width, int height) abstract;
	virtual void Update(float deltaTime) abstract;

// 그리기 관련 함수 --------------------------------------------------------------------------------
	virtual void BeginDraw() abstract;
	virtual void DefferedDrawTest() abstract;
	virtual void EndDraw() abstract;

	virtual void FillBackground(int colorCode, float alpha) abstract;
	virtual void FillBackground(float r, float g, float b, float alpha) abstract;
	virtual void DrawChar(std::wstring text, Vector2 pos, UINT fontsize = 20, Vector4 color = Vector4(1.f,1.f,1.f,1.f), Vector2 boxSize = Vector2(300, 50)) abstract;

// 애니메이션 관련 함수
	/// <summary> modelKey에 해당하는 model의 animationName 재생 </summary>
	virtual void AnimationControl(std::wstring modelKey, std::wstring animationName) abstract;
	/// <summary> true 보내면 일시정지 false 보내면 이어서 재생 </summary>
	virtual void AnimationStop(std::wstring modelKey, bool isStop) abstract;
	/// <summary> 애니메이션이 끝난 상태면 true 반환 </summary>
	virtual bool IsAnimationEnd(std::wstring modelKey) abstract;
	 
// 렌더 관련 함수 ----------------------------------------------------------------------------------
	virtual std::wstring LoadModel(ModelKey modelKey) abstract;
	virtual void ReleaseModel() abstract;

	virtual void ObjectRender(ModelData modelData) abstract;
	virtual void UIRender(ModelData uiData) abstract;

	virtual void LightRender(DirectionalLightData lightData) abstract;
	virtual void LightRender(PointLightData lightData) abstract;
	virtual void SetWorldLightProperty(LightWorldProperty property) abstract;

// 랜더 설정 관련 함수 ------------------------------------------------------------------------------
	virtual void SetAlphaBlendingMode(bool enable) abstract;
	virtual void SetZBufferUsing(bool enable) abstract;

// IMGUI에 보내야 함
	virtual std::any GetDeviceManager() abstract;
};

namespace DXExport
{
	/// <summary>
	/// 꼭 한번만 호출하셔야 합니다...
	/// 싱글톤 아님....
	/// </summary>
	std::shared_ptr<IGraphics> GetInstance();
}