#pragma once
#include <string>

class IResource
{
public:
	IResource() {};
	virtual ~IResource() {};

public:
	virtual bool Load() abstract;
	virtual void UnLoad() abstract;

	std::wstring m_filePath;
};

