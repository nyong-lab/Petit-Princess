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
/// �׷��Ƚ� �������̽�
/// 
/// 24.01.18
/// �ڿ���
/// </summary>
class IGraphics
{
public:
	virtual ~IGraphics() = default;

public:
// �����ڿ� �Ҹ��� ---------------------------------------------------------------------------------
	virtual void Initialize(HWND hwnd) abstract;
	virtual void Finalize() abstract;

// �׷��Ƚ� ���� ������Ʈ ---------------------------------------------------------------------------
	virtual void Resize(int width, int height) abstract;
	virtual void Update(float deltaTime) abstract;

// �׸��� ���� �Լ� --------------------------------------------------------------------------------
	virtual void BeginDraw() abstract;
	virtual void DefferedDrawTest() abstract;
	virtual void EndDraw() abstract;

	virtual void FillBackground(int colorCode, float alpha) abstract;
	virtual void FillBackground(float r, float g, float b, float alpha) abstract;
	virtual void DrawChar(std::wstring text, Vector2 pos, UINT fontsize = 20, Vector4 color = Vector4(1.f,1.f,1.f,1.f), Vector2 boxSize = Vector2(300, 50)) abstract;

// �ִϸ��̼� ���� �Լ�
	/// <summary> modelKey�� �ش��ϴ� model�� animationName ��� </summary>
	virtual void AnimationControl(std::wstring modelKey, std::wstring animationName) abstract;
	/// <summary> true ������ �Ͻ����� false ������ �̾ ��� </summary>
	virtual void AnimationStop(std::wstring modelKey, bool isStop) abstract;
	/// <summary> �ִϸ��̼��� ���� ���¸� true ��ȯ </summary>
	virtual bool IsAnimationEnd(std::wstring modelKey) abstract;
	 
// ���� ���� �Լ� ----------------------------------------------------------------------------------
	virtual std::wstring LoadModel(ModelKey modelKey) abstract;
	virtual void ReleaseModel() abstract;

	virtual void ObjectRender(ModelData modelData) abstract;
	virtual void UIRender(ModelData uiData) abstract;

	virtual void LightRender(DirectionalLightData lightData) abstract;
	virtual void LightRender(PointLightData lightData) abstract;
	virtual void SetWorldLightProperty(LightWorldProperty property) abstract;

// ���� ���� ���� �Լ� ------------------------------------------------------------------------------
	virtual void SetAlphaBlendingMode(bool enable) abstract;
	virtual void SetZBufferUsing(bool enable) abstract;

// IMGUI�� ������ ��
	virtual std::any GetDeviceManager() abstract;
};

namespace DXExport
{
	/// <summary>
	/// �� �ѹ��� ȣ���ϼž� �մϴ�...
	/// �̱��� �ƴ�....
	/// </summary>
	std::shared_ptr<IGraphics> GetInstance();
}