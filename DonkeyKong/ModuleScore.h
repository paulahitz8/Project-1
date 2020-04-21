#pragma once

#ifndef __SCENE_SCORE_H__
#define __SCENE_SCORE_H__

#include "Module.h"
#include "Animation.h"
#include "SDL_Rect.h"
#include "p2Point.h"

struct SDL_Texture;


class ModuleScore : public Module
{
private:
	iPoint position = { 0, 0 }; //x y aun no sabemos donde imprimirlo

	SDL_Rect number;


	SDL_Texture* backkTexture = nullptr;
	SDL_Texture* Texture0 = nullptr;
	SDL_Texture* Texture1 = nullptr;
	SDL_Texture* Texture2 = nullptr;
	SDL_Texture* Texture3 = nullptr;
	SDL_Texture* Texture4 = nullptr;
	SDL_Texture* Texture5 = nullptr;
	SDL_Texture* Texture6 = nullptr;
	SDL_Texture* Texture7 = nullptr;
	SDL_Texture* Texture8 = nullptr;
	SDL_Texture* Texture9 = nullptr;

	int scoremap[32][32] = { 0 };


public:
	ModuleScore(bool startEnabled);
	~ModuleScore();
	bool Start() override;
	Update_Status Update() override;
	Update_Status PostUpdate() override;
	bool CleanUp();

};


#endif
