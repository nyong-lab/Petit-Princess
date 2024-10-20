#pragma once
#include <any>

/// <summary>
/// Device, Device Context를 관리하는 인터페이스
/// 
/// 24.01.23
/// 박연하
/// </summary>
class IDevice
{
	virtual void CreateDevice() abstract;
};