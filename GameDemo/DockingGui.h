#pragma once


#include "BaseGui.h"

class Entity;

/// <summary>
/// ImGui���� ���Ǽ��� ���� ��ŷ Ŭ����
/// �ۼ��� : �赿��
/// </summary>
class DockingGui : public BaseGui
{
public:
	DockingGui() {};
	~DockingGui() {}

	void Update();

private:
	void CreateDockingWindow();
};

