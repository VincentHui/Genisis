#pragma once
#include "Entity.h"
#include <unordered_map>
#include <string>

using namespace std;
class component
{
public:
	component(void);
	~component(void);
	void AddEntity(string p_name, Entity * p_entity);
private:
	unordered_map <string, Entity*> m_EntityList;
};

