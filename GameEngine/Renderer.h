#pragma once
#include "Component.h"
#include "CameraModule.h"
#include "IGraphics.h"
#include "MeshFilter.h"

class IGraphics;
class CameraModule;

/// <summary>
/// 수정 : 김동일 -> 생성자에 매니저들이 들어간 컴포넌트들은 씬 로드 하는 과정에서 
/// 기본 생성자로 컴포넌트를 만들기 때문에 추가로 그래픽스와 카메라를 밀어넣어주는 함수 구현 
/// </summary>
class Renderer : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Renderer)

	// Serialize
	virtual void Serialize(nlohmann::json& json) abstract;
	virtual void Deserialize(const nlohmann::json& json) abstract;

public:
	Renderer();
	Renderer(const std::string& name);
	virtual ~Renderer();

public:
	virtual void Update(double dt) abstract override;
	virtual void Finalize() override;
	virtual void Render(double dt) abstract override;

	virtual std::shared_ptr<Component> Clone() const override;
};

