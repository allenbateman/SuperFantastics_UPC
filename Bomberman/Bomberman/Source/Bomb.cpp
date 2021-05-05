#include "Bomb.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "SceneLevel1.h"
#include "ModuleCollisions.h"

Bomb::Bomb(int x, int y):Entitie(x, y)
{
	int diffX = (x - 24) % 16;
	int diffY = (y - 32) % 16;

	if (diffX > 8)
	{
		position.x += 16;
	}

	position.x -= diffX;

	if (diffY > 8)
	{
		position.y += 16;
	}

	position.y -= diffY;

	idleAnim.PushBack({ 0,256,16,16 });
	idleAnim.PushBack({ 16,256,16,16 });
	idleAnim.PushBack({ 32,256,16,16 });
	idleAnim.loop = true;
	idleAnim.pingpong = true;
	idleAnim.speed = 0.1f;

	// Animacion centro explosion (SOLO IDA (como se hace ida y vuelta???))
	centerAnim.PushBack({ 0,288,16,16 });
	centerAnim.PushBack({ 16,288,16,16 });
	centerAnim.PushBack({ 32,288,16,16 });
	centerAnim.PushBack({ 48,288,16,16 });
	centerAnim.loop = true;
	centerAnim.pingpong = true;
	centerAnim.speed = 0.1f;

	// Horizontal side explosion
	horSideAnim.PushBack({ 0,304,16,16 });
	horSideAnim.PushBack({ 16,304,16,16 });
	horSideAnim.PushBack({ 32,304,16,16 });
	horSideAnim.PushBack({ 48,304,16,16 });
	horSideAnim.loop = true;
	horSideAnim.pingpong = true;
	horSideAnim.speed = 0.1f;

	// Left extreme explosion
	leftExtrAnim.PushBack({ 0,320,16,16 });
	leftExtrAnim.PushBack({ 16,320,16,16 });
	leftExtrAnim.PushBack({ 32,320,16,16 });
	leftExtrAnim.PushBack({ 48,320,16,16 });
	leftExtrAnim.loop = true;
	leftExtrAnim.pingpong = true;
	leftExtrAnim.speed = 0.1f;

	// Right extreme explosion
	rightExtrAnim.PushBack({ 0,336,15,351 });
	rightExtrAnim.PushBack({ 16,336,31,351 });
	rightExtrAnim.PushBack({ 32,336,47,351 });
	rightExtrAnim.PushBack({ 48,336,63,351 });
	rightExtrAnim.loop = true;
	rightExtrAnim.pingpong = true;
	rightExtrAnim.speed = 0.1f;

	// Vertical side explosion
	vertSideAnim.PushBack({ 0,352,15,367 });
	vertSideAnim.PushBack({ 16,352,31,367 });
	vertSideAnim.PushBack({ 32,352,47,367 });
	vertSideAnim.PushBack({ 48,352,63,367 });
	vertSideAnim.loop = true;
	vertSideAnim.pingpong = true;
	vertSideAnim.speed = 0.1f;

	// Up extreme explosion
	upExtrAnim.PushBack({ 0,368,15,383 });
	upExtrAnim.PushBack({ 16,368,31,383 });
	upExtrAnim.PushBack({ 32,368,47,383 });
	upExtrAnim.PushBack({ 48,368,63,383 });
	upExtrAnim.loop = true;
	upExtrAnim.pingpong = true;
	upExtrAnim.speed = 0.1f;

	// Down extreme explosion
	downExtrAnim.PushBack({ 0,384,15,400 });
	downExtrAnim.PushBack({ 16,384,31,400 });
	downExtrAnim.PushBack({ 32,384,47,400 });
	downExtrAnim.PushBack({ 48,384,63,400 });
	downExtrAnim.loop = true;
	downExtrAnim.pingpong = true;
	downExtrAnim.speed = 0.1f;

	state = IDLE;

	currentAnim = &idleAnim;
	frameSpawn = App->frameCounter;
}

void Bomb::Update()
{
	Entitie::Update();

	switch (state) {


	case IDLE:

		if (App->frameCounter >= frameSpawn + bombTimer) {
			// UP DIRECTION
			for (int i = 0; i < App->player->rangeExplosion; i++) {
				if (App->sceneLevel_1->grid[(position.y - 32) / 16 - i - 1][(position.x - 24) / 16] == SceneLevel1::EMPTY) {
					App->collisions->AddCollider({ position.x, position.y - (i + 1) * 16 }, Collider::BOMB);
					upCount++;
				}
				else {
					break;
				}
			}

			// DOWN DIRECTION
			for (int i = 0; i < App->player->rangeExplosion; i++) {
				if (App->sceneLevel_1->grid[(position.y - 32) / 16 + i + 1][(position.x - 24) / 16] == SceneLevel1::EMPTY) {
					App->collisions->AddCollider({ position.x, position.y + (i + 1) * 16 }, Collider::BOMB);
					downCount++;
				}
				else {
					break;
				}
			}

			// RIGHT DIRECTION
			for (int i = 0; i < App->player->rangeExplosion; i++) {
				if (App->sceneLevel_1->grid[(position.y - 32) / 16][(position.x - 24) / 16 + i + 1] == SceneLevel1::EMPTY) {
					App->collisions->AddCollider({ position.x + (i + 1) * 16, position.y}, Collider::BOMB);
					rightCount++;
				}
				else {
					break;
				}
			}

			// LEFT DIRECTION
			for (int i = 0; i < App->player->rangeExplosion; i++) {
				if (App->sceneLevel_1->grid[(position.y - 32) / 16][(position.x - 24) / 16 - i - 1] == SceneLevel1::EMPTY) {
					App->collisions->AddCollider({ position.x - (i + 1) * 16, position.y }, Collider::BOMB);
					leftCount++;
				}
				else {
					break;
				}
			}

			// TODO: START SOUND
			frameExplode = App->frameCounter;
			state = EXPLOSION;
			currentAnim = nullptr;
		}
		break;

	case EXPLOSION:
		SDL_Rect rect;

		centerAnim.Update();
		vertSideAnim.Update();
		upExtrAnim.Update();
		downExtrAnim.Update();
		horSideAnim.Update();
		leftExtrAnim.Update();
		rightExtrAnim.Update();

		rect = centerAnim.GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);

		// UP DIRECTION
		if (upCount > 0) {
			rect = vertSideAnim.GetCurrentFrame();

			for (int i = 1; i < upCount; i++) {
				App->render->Blit(texture, position.x, position.y - (i * 16), &rect);
			}

			rect = upExtrAnim.GetCurrentFrame();

			App->render->Blit(texture, position.x, position.y - (upCount * 16), &rect);
		}

		// LEFT DIRECTION
		if (leftCount > 0) {
			rect = horSideAnim.GetCurrentFrame();

			for (int i = 1; i < leftCount; i++) {
				App->render->Blit(texture, position.x - (i * 16), position.y, &rect);
			}

			rect = leftExtrAnim.GetCurrentFrame();

			App->render->Blit(texture, position.x - (leftCount * 16), position.y, &rect);
		}

		// RIGHT DIRECTION
		if (rightCount > 0) {

			rect = horSideAnim.GetCurrentFrame();

			for (int i = 1; i < rightCount; i++) {
				App->render->Blit(texture, position.x + (i * 16), position.y, &rect);
			}

			rect = rightExtrAnim.GetCurrentFrame();

			App->render->Blit(texture, position.x + (rightCount * 16), position.y, &rect);
		}

		// DOWN DIRECTION
		if (downCount > 0) {

			rect = vertSideAnim.GetCurrentFrame();

			for (int i = 1; i < downCount; i++) {
				App->render->Blit(texture, position.x, position.y + (i * 16), &rect);
			}

			rect = downExtrAnim.GetCurrentFrame();

			App->render->Blit(texture, position.x, position.y + (downCount * 16), &rect);
		}

		if (App->frameCounter >= frameExplode + explosionTimer) {
			// Remove all explosion colliders (No se como se hace no lo habia mirado)
			SetToDelete();
		}
		break;
	}
}

void Bomb::OnCollision(Collider* collider)
{

}
