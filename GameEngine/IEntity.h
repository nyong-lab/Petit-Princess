#pragma once

class IGraphics;
class CameraModule;

class IEntity
{
public:
	virtual ~IEntity() {}

	virtual void Start() abstract;
	virtual void Render(double dt) abstract;
	virtual void DebugRender() abstract;
	virtual void FixedUpdate(double FixedTime) abstract;
	virtual void Update(double dt) abstract;
	virtual void Finalize() abstract;

	//컴포넌트를 가져오겠다
	//IComponent GetComponent(컴포넌트를 특정할 수 있는 변수) const;

private:

};


