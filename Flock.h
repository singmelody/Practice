#pragma once

#include <vector>
#include <d3dx9.h>

class Bird;

class Flock
{
public:
	Flock(int numBirds);
	~Flock(void);

	void Update(float deltaTime);
	void Render(bool shadow);
	void GetNeighbors(Bird* pBird, float radius, std::vector<Bird*> neighbors);

private:
	std::vector<Bird*> m_Birds;
};

