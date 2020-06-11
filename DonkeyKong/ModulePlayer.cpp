#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleHammer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollisions.h"

#include "SDL_scancode.h"

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{


	// left idle
	leftidleAnim.PushBack({ 0, 0 , 50, 26 });
	leftidleAnim.PushBack({ 0, 37, 50, 26 });
	leftidleAnim.speed = 0.1f;

	//right idle
	rightidleAnim.PushBack({ 0, 64, 50, 26 });
	rightidleAnim.PushBack({ 0, 100, 50, 26 });
	rightidleAnim.speed = 0.1f;

	// left animation
	leftAnim.PushBack({ 203, 1, 50, 26 });
	leftAnim.PushBack({ 203, 63, 50, 26 });
	leftAnim.PushBack({ 203, 36, 50, 26 });
	leftAnim.PushBack({ 203, 94, 50, 26 });
	leftAnim.speed = 0.2f;

	//right animation
	rightAnim.PushBack({ 203, 122, 50, 26 });
	rightAnim.PushBack({ 203, 189, 50, 26 });
	rightAnim.PushBack({ 203, 161, 50, 26 });
	rightAnim.PushBack({ 203, 227, 50, 26 });
	rightAnim.speed = 0.2f;

	//up animation
	upladderAnim1.PushBack({ 261, 94, 50, 26 });
	upladderAnim.PushBack({ 261, 122, 50, 26 });
	upladderAnim.PushBack({ 261, 1, 50, 26 });
	upladderAnim2.PushBack({ 261, 35, 50, 26 });
	upladderAnim3.PushBack({ 261, 63, 50, 26 });
	upladderAnim.speed = 0.12f;

	//down animation
	downladderAnim3.PushBack({ 261, 63, 50, 26 });
	downladderAnim2.PushBack({ 261, 35, 50, 26 });
	downladderAnim.PushBack({ 261, 1, 50, 26 });
	downladderAnim.PushBack({ 261, 122, 50, 26 });
	downladderAnim1.PushBack({ 261, 94, 50, 26 });
	downladderAnim.speed = 0.12f;

	//dead animation
	deadAnim1.PushBack({ 149, 41, 50, 26 });
	deadAnim4.PushBack({ 149, 71, 50, 26 });
	deadAnim2.PushBack({ 89, 68, 50, 26 });
	deadAnim3.PushBack({ 89, 101, 50, 26 });

	//dead angel animation
	angelAnim.PushBack({ 59, 67, 30, 26 });

}

ModulePlayer::~ModulePlayer() {

}


bool ModulePlayer::Start()
{
	carrotDeletex = 0;
	carrotDeletey = 0;

	LOG("Loading player textures");
	playertexture = App->textures->Load("Assets/Mario/mariosprites.png");

	currentAnimation = &rightidleAnim; //mario empieza mirando a la derecha

	position.x = { 43 };
	position.y = { 222 };

	carrotcount = 8;
	if (resetVidas == true) {
		livecount = 3;
		resetVidas = false;
	}

	player = { 56, 2, 12, 16 };

	destroyed = false;

	collider = App->collision->AddCollider({ position.x + 18, position.y + 10, 12, 16 }, Collider::Type::PLAYER, this);

	walkingFx = App->audio->LoadFx("Assets/Music/15 SFX (Walking).wav");
	silenceFx = App->audio->LoadFx("Assets/Music/silence.wav");
	deadFx = App->audio->LoadFx("Assets/Music/20 SFX (Miss).wav");
	bonusFx = App->audio->LoadFx("Assets/Music/19 SFX (Bonus).wav");

	return true;
}

Update_Status ModulePlayer::Update()
{

	if (App->input->keys[SDL_SCANCODE_F2] == Key_State::KEY_DOWN) {
		godmode = !godmode;

	}
	if (App->player->activelevel == 2) {
		if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl2, (Module*)App->winning, 0);

		}
		if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl2, (Module*)App->gameover, 0);

		}
		if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl2, (Module*)App->lvl3, 0);

		}
	}
	if (App->player->activelevel == 3) {
		if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl3, (Module*)App->winning, 0);

		}
		if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl3, (Module*)App->gameover, 0);

		}
		if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl3, (Module*)App->lvl4, 0);

		}
	}
	if (App->player->activelevel == 4) {
		if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl4, (Module*)App->winning, 0);

		}
		if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN) {
			App->fade->FadeToBlack((Module*)App->lvl4, (Module*)App->gameover, 0);

		}
	}

	if (godmode == true) {

		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT)
		{
			position.x -= speedx;
			currentAnimation = &leftAnim;
		}

		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT)
		{
			position.x += speedx;
			currentAnimation = &rightAnim;

		}

		// If last movement was left, set the current animation back to left idle
		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_UP)
		{
			currentAnimation = &leftidleAnim;
		}
		// If last movement was right, set the current animation back to left idle
		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_UP)
			currentAnimation = &rightidleAnim;

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			position.y += 2;
		}

		if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{
			position.y -= 2;
		}

		if (position.x < 0) {position.x +=2;}
		if (position.x > 216) { position.x -= 2; }
		if (position.y < 0) { position.y += 2; }
		if (position.y > 232) { position.y -= 2; }

	}


	if (destroyed) {

		if (livecount == 0) {

			resetVidas = true;
			collider->pendingToDelete = true;
			App->hammer->hammerCollider->pendingToDelete = true;
			if (App->player->activelevel == 2) {
				App->fade->FadeToBlack((Module*)App->lvl2, (Module*)App->gameover, 30);
			}
			if (App->player->activelevel == 3) {
				App->fade->FadeToBlack((Module*)App->lvl3, (Module*)App->gameover, 30);
			}
			if (App->player->activelevel == 4) {
				App->fade->FadeToBlack((Module*)App->lvl4, (Module*)App->gameover, 30);
			}
			
			destroyed = false;
		}


		else {

			collider->pendingToDelete = true;
			App->hammer->hammerCollider->pendingToDelete = true;

			if (currentAnimation != &angelAnim && currentAnimation != &deadAnim2 && currentAnimation != &deadAnim3 && currentAnimation != &deadAnim4)
			{
				currentAnimation = &deadAnim1;
			}
			if (j <= 3)
			{
				if (currentAnimation == &deadAnim1 && i % 7 == 0)
				{
					currentAnimation = &deadAnim2;
				}
				if (currentAnimation == &deadAnim2 && i % 14 == 0)
				{
					currentAnimation = &deadAnim3;
				}
				if (currentAnimation == &deadAnim3 && i % 21 == 0)
				{
					currentAnimation = &deadAnim4;
				}
				if (currentAnimation == &deadAnim4 && i % 28 == 0)
				{
					currentAnimation = &deadAnim1;
					j++;
				}
			}
			else if (currentAnimation == &deadAnim1 && i % 40 == 0)
			{
				currentAnimation = &angelAnim;
			}
			else if(currentAnimation == &angelAnim && i % 200 == 0)
			{
				if (App->player->activelevel == 2) {
					App->fade->FadeToBlack((Module*)App->lvl2, (Module*)App->pierdevida, 30);
				}
				if (App->player->activelevel == 3) {
					App->fade->FadeToBlack((Module*)App->lvl3, (Module*)App->pierdevida, 30);
				}
				if (App->player->activelevel == 4) {
					App->fade->FadeToBlack((Module*)App->lvl4, (Module*)App->pierdevida, 30);
				}
				destroyed = false;
				i = 0;
				j = 0;
			}
			i++;
		}

	}


	if (!destroyed) {

		/*if (ladderOn == false && groundOn == false && isJumping == false) {
			position.y + 1;
		}
		Con esto o lo de abajo deber�a caerse al salir de un collider del ground*/

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN) {
			isJumping = true;
			jumpTimer = 1;
			startingy = position.y;
			jumpingspeedy = 6;
		}

		if (isJumping == true) {

			jumpingspeedy -= gravity;
			position.y -= jumpingspeedy;

			if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT) {
				position.x -= jumpingspeedx;
			}
			    
			if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT) {
				position.x += jumpingspeedx;
			}

			if (position.y == startingy) {
				isJumping = false;
			}

		}


		if (groundOn == true) {

			if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT)
			{
				position.x -= speedx;
				currentAnimation = &leftAnim;
				if (position.x % 15 == 0)
				{
					App->audio->PlayFx(walkingFx);
				}
			}

			if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT)
			{
				position.x += speedx;
				currentAnimation = &rightAnim;
				if (position.x % 15 == 0)
				{
					App->audio->PlayFx(walkingFx);
				}
			}

			// If last movement was left, set the current animation back to left idle
			if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_UP)
			{
				currentAnimation = &leftidleAnim;
			}
			// If last movement was right, set the current animation back to left idle
			if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_UP)
				currentAnimation = &rightidleAnim;


			if (ladderOn == true) {

				if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
				{
					position.y += speedy;
					if (position.y == 223 || position.y == 183 || position.y == 143 || position.y == 103 || position.y == 63)
					{
						currentAnimation = &downladderAnim1;
					}
					else if (((position.y > 182) && (position.y <= 190)) || ((position.y > 142) && (position.y <= 150)) || ((position.y > 102) && (position.y <= 110)) || ((position.y > 62) && (position.y <= 70)))
					{
						currentAnimation = &downladderAnim3;
					}
					else if (((position.y > 190) && (position.y < 198)) || ((position.y > 150) && (position.y < 158)) || ((position.y > 110) && (position.y < 118)) || ((position.y > 70) && (position.y < 78)))
					{
						currentAnimation = &downladderAnim2;
					}
					else
					{
						currentAnimation = &downladderAnim;
					}
					if (position.y % 15 == 0)
					{
						App->audio->PlayFx(walkingFx);
					}
				}

				if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
				{
					position.y -= speedy;
					if (position.y == 222 || position.y == 182 || position.y == 142 || position.y == 102 || position.y == 62)
					{
						currentAnimation = &upladderAnim1;
					}
					else if (((position.y > 182) && (position.y <= 190)) || ((position.y > 142) && (position.y <= 150)) || ((position.y > 102) && (position.y <= 110)) || ((position.y > 62) && (position.y <= 70)))
					{
						currentAnimation = &upladderAnim3;
					}
					else if (((position.y > 190) && (position.y < 198)) || ((position.y > 150) && (position.y < 158)) || ((position.y > 110) && (position.y < 118)) || ((position.y > 70) && (position.y < 78)))
					{
						currentAnimation = &upladderAnim2;
					}
					else
					{
						currentAnimation = &upladderAnim;
					}
					if (position.y % 15 == 0)
					{
						App->audio->PlayFx(walkingFx);
					}
				}

				if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_UP || App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_UP)
				{
					currentAnimation = &upladderAnim1;
				}
			}

		}

		else {

			if (ladderOn == true) {

				if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
				{
					position.y += speedy;

				}

				if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
				{
					position.y -= speedy;
				}
			}

			/*if (ladderOn == false && isJumping == false) {
				position.y + 1;
			} 
			Con esto o lo de arriba deber�a caerse al salir de un collider del ground*/
		}
	}

	collider->SetPos(position.x + 18, position.y + 10);

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
	
}

Update_Status ModulePlayer::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(playertexture, position.x, position.y, &rect);

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{	
	if (godmode == false) {
		// LADDER
		if (c2->type == Collider::Type::LADDER && ((position.x + 18) > (c2->rect.x - 8) && (position.x + 18) < (c2->rect.x)))
		{
			if (((position.y + 10) + player.h) < (c2->rect.y + 3)) {
				position.y += 1;
			}

			if (((position.y + 10) + player.h) > c2->rect.y + 42)
			{
				position.y -= 1;
			}

			if (((position.y + 10) < (c2->rect.y + 26)) && ((position.y + 10) > (c2->rect.y - 13)) && ((position.x + 18) < (c2->rect.x - 6))) {
				position.x += 1;
			}

			if (((position.y + 10) < (c2->rect.y + 26)) && ((position.y + 10) > (c2->rect.y - 13)) && ((position.x + 18) > (c2->rect.x - 4))) {
				position.x -= 1;
			}

			ladderOn = true;
		}
		else {
			ladderOn = false;
		}


		// GROUND

		if (c2->type == Collider::Type::GROUND || c2->type == Collider::Type::LADDER)
		{
			groundOn = true;
		}
		else {
			groundOn = false;
		}



		// ENEMY
		if (c1 == collider && c2->type == Collider::ENEMY && destroyed == false)
		{
			App->audio->PlayFx(deadFx);
			livecount--;

			destroyed = true;
		}

		// WALL
		if (c1 == collider && destroyed == false && c2->type == Collider::WALL) {
			if ((position.x + 18) < (c2->GetRect().x + c2->GetRect().w) && (position.x + 18) > c2->GetRect().x) { position.x += 2; }
			if (((position.x + 18) + player.w) > c2->GetRect().x && ((position.x + 18) + player.w) < (c2->GetRect().x + c2->GetRect().w)) { position.x -= 2; }
		}

		// CARROT
		if (c2->type == Collider::Type::CARROT)
		{

			carrotcount--;
			App->audio->PlayFx(bonusFx);
			n++;

			if (carrotcount == 0) {
				if (App->player->activelevel == 2) {
					App->fade->FadeToBlack((Module*)App->lvl2, (Module*)App->siguientenivel, 0);
				}
				if (App->player->activelevel == 3) {
					App->fade->FadeToBlack((Module*)App->lvl3, (Module*)App->siguientenivel, 0);
				}
				if (App->player->activelevel == 4) {
					App->fade->FadeToBlack((Module*)App->lvl4, (Module*)App->winning, 0);
				}
	

	
				resetVidas = true;
			}

			colliderDelete = c2;

			carrotDeletex = c2->GetRect().x;
			carrotDeletey = c2->GetRect().y;

			groundOn = true;

		}

	}

	// OBJECT
	if (c2->type == Collider::Type::OBJECT)
	{

		groundOn = true;

	}

	// LEFTFLOOR
	/*if (c2->type == Collider::Type::OBJECT)
	{
		if ((position.y + player.h) < (c2->GetRect().y + 2) && (position.y + player.h) > c2->GetRect().y -2) { position.y --; }
		
		groundOn = true;

	}*/

} 

bool ModulePlayer::CleanUp()
{


	return true;
}


