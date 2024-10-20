#pragma once


#include "BaseGui.h"

class Entity;

/// <summary>
/// ImGui에서 편의성을 위한 도킹 클래스
/// 작성자 : 김동일
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

