#pragma once
#include "SceneObject.h"

class TerrainGridElement : public SceneObject
{
public:
	TerrainGridElement(string barname, const XMFLOAT3 & pos, float x, float y);
	~TerrainGridElement(void);
	float COx;
	float COy;
};

