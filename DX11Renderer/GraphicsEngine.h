#pragma once
#include "IGraphics.h"
#include "Device.h"

#include "Mesh.h"
#include "BufferData.h"

#include "DXResourceManager.h"
#include "RenderPath.h"

class DXManager;
class DXResourceManager;

using namespace GraphicsTransfer;

/// <summary>
/// �׷��Ƚ� ���� ��ü!
/// 
/// �ռ���, �ڿ���
/// </summary>
class GraphicsEngine : public IGraphics
{
public:
	// �����ڿ� �Ҹ��� ---------------------------------------------------------------------------------
	GraphicsEngine();
	virtual ~GraphicsEngine();
	void Initialize(HWND hwnd);
	void Finalize();

	// �׷��Ƚ� ���� ������Ʈ ---------------------------------------------------------------------------
	void Update(float deltaTime) override;
	void Resize(int width, int height) override;

	// �׸��� ���� �Լ� --------------------------------------------------------------------------------
	void BeginDraw() override;
	void DefferedDrawTest() override;
	void EndDraw() override;
	
	void FillBackground(int colorCode, float alpha) override;
	void FillBackground(float r, float g, float b, float alpha) override;
	void DrawChar(std::wstring text, Vector2 pos, UINT fontsize = 20, Vector4 color = Vector4(1.f,1.f,1.f,1.f), Vector2 boxSize = Vector2(300, 50)) override;

	// ���� ���� ���� �Լ� ------------------------------------------------------------------------------
	void SetAlphaBlendingMode(bool enable);
;	void SetZBufferUsing(bool enable);

	// ���� ���� �Լ� -----------------------------------------------------------------------------------
	std::shared_ptr<ConstantBuffer> CreateConstantBuffer(void* addressKey, UINT bufferSize);
	void UpdateConstantBuffer(ComPtr<ID3D11Buffer> buffer, void* pData, UINT size);

	std::wstring LoadModel(ModelKey modelKey) override;
	void ReleaseModel() override;

	std::shared_ptr<Model> GetModel(ModelKey modelKey, std::wstring& newKey);

	void ObjectRender(ModelData modelData) override;
	void UIRender(ModelData uiData) override;
	void LightRender(DirectionalLightData lightData) override;
	void LightRender(PointLightData lightData) override;
	virtual void SetWorldLightProperty(LightWorldProperty property);

	void AnimationControl(std::wstring modelKey, std::wstring animationName) override;
	void AnimationStop(std::wstring modelKey, bool isStop) override;
	bool IsAnimationEnd(std::wstring modelKey) override;

	// Getter, Setter ---------------------------------------------------------------------------------
	std::shared_ptr<DXManager> GetDXManager()				{ return m_dxManager; }
	std::shared_ptr<DXResourceManager> GetResourceManager() { return m_resourceManager; }

	// Only GUI
	std::any GetDeviceManager() override					{ return m_device; }

// ��� ���� ----------------------------------------------------------------------------------------
private:
	std::vector<std::pair<std::wstring, std::shared_ptr<Model>>> m_activeModelList;

	HWND m_hwnd;
	int m_screenWidth;
	int m_screenHeight;
	bool m_isVsyncEnabled;

	std::shared_ptr<Device> m_device;
	std::shared_ptr<DXManager> m_dxManager;
	std::shared_ptr<DXResourceManager> m_resourceManager;
	std::unique_ptr<RenderPath> m_renderPath;

	UINT m_modelID = 0;
};
