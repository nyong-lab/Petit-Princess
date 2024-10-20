#pragma once
#include <Entity.h>
class autoBush :
    public Entity
{
public:
	
	autoBush();
	autoBush(const std::shared_ptr<autoBush>& other);
	~autoBush();

	void Start();
	void SetPos(Vector3 pos);

private:

};

