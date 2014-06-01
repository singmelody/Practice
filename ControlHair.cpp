#include "DXUT.h"
#include "ControlHair.h"

ControlHair::ControlHair()
{

}

ControlHair* ControlHair::CreateTestHair(D3DXVECTOR3 startPos)
{
	ControlHair* newHair = new ControlHair();

	for (int y = 0; y < 4; ++y)
	{
		newHair->m_points.push_back(startPos + D3DXVECTOR3( ((rand()%100) / 100.0f) - 0.5f,
			(float)y, 
			((rand()%100) / 100.0f) - 0.5f));

		newHair->m_destPoints.push_back(newHair->m_points[y]);
	}

	return newHair;
}

