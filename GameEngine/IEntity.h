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

	//������Ʈ�� �������ڴ�
	//IComponent GetComponent(������Ʈ�� Ư���� �� �ִ� ����) const;

private:

};


