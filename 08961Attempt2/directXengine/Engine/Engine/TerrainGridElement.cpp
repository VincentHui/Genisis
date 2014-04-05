#include "TerrainGridElement.h"


TerrainGridElement::TerrainGridElement(string barname, const XMFLOAT3 & pos, float x, float y):
	SceneObject(barname, pos), COx(x), COy(y)
{
	
}


TerrainGridElement::~TerrainGridElement(void)
{
}
