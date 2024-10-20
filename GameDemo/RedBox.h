#pragma once
#include <Entity.h>
class RedBox : public Entity
{
public:
	RedBox();
	RedBox(const std::shared_ptr<RedBox>& other);
	~RedBox();

public:
	virtual void Start() override;
};

class PinkBox : public Entity
{
public:
	PinkBox();
	PinkBox(const std::shared_ptr<PinkBox>& other);
	~PinkBox();

public:
	virtual void Start() override;
};

class YellowBox : public Entity
{
public:
	YellowBox();
	YellowBox(const std::shared_ptr<YellowBox>& other);
	~YellowBox();

public:
	virtual void Start() override;
};