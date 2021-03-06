#pragma once
#ifndef __ENEMY_FIREBALLS_H__
#define __ENEMY_FIREBALLS_H__

#include "Enemies.h"
#include "Path.h"

class Enemy_Fireballs : public Enemies
{
public:

	// Constructor (x y coordinates in the world)
	Enemy_Fireballs(int x, int y);

	void Update() override;

private:

	Collider* colliderF = nullptr;

	Animation leftAnim;
	Animation rightAnim;
	Animation leftAnimhammer;
	Animation rightAnimhammer;
};

#endif

