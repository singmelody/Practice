#include "DXUT.h"
#include "Flock.h"
#include "Bird.h"


Flock::Flock(int numBirds)
{
	for (int i = 0; i < numBirds; ++i)
		m_Birds.push_back(new Bird(this));
}


Flock::~Flock(void)
{
	for (int i = 0; i < m_Birds.size(); ++i)
		SAFE_DELETE(m_Birds[i]);

	m_Birds.clear();
}

void Flock::Render(bool shadow)
{
	for (int i = 0; i < m_Birds.size(); ++i)
		m_Birds[i]->Render(false);
}

void Flock::GetNeighbors(Bird* pBird, float radius, std::vector<Bird*> neighbors)
{
	for (int i = 0; i < m_Birds.size(); ++i)
	{
		if(m_Birds[i] != pBird)
		{
			if(D3DXVec3LengthSq(&(m_Birds[i]->m_position - pBird->m_position)) < radius * radius )
				neighbors.push_back(m_Birds[i]);
		}
	}
}

void Flock::Update(float deltaTime)
{
	for (int i = 0; i < m_Birds.size(); ++i)
		m_Birds[i]->Update(deltaTime);
}
