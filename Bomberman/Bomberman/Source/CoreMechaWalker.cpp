#include "CoreMechaWalker.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "SceneLevel1.h"
#include <ctime>

CoreMechaWalker::CoreMechaWalker(int x, int y) : Entitie(x, y)
{
	position.x = x;
	position.y = y;


	// idle
	idleAnim.PushBack({ 128, 64,32,32 });
	idleAnim.loop = true;
	idleAnim.mustFlip = false;
	idleAnim.speed = 0.1f;

	// move up
	upAnim.PushBack({ 192, 64,32,32 });
	upAnim.PushBack({ 224, 64,32,32 });
	upAnim.loop = true;
	upAnim.mustFlip = false;
	upAnim.speed = 0.1f;

	// move down
	downAnim.PushBack({ 128, 64,32,32 });
	downAnim.PushBack({ 160, 64,32,32 });
	downAnim.loop = true;
	downAnim.mustFlip = false;
	downAnim.speed = 0.1f;

	// move left
	leftAnim.PushBack({ 256, 64,32,32 });
	leftAnim.PushBack({ 288, 64,32,32 });
	leftAnim.PushBack({ 320, 64,32,32 });
	leftAnim.loop = true;
	leftAnim.mustFlip = false;
	leftAnim.speed = 0.1f;

	// move right
	rightAnim.PushBack({ 256, 64,32,32 });
	rightAnim.PushBack({ 288, 64,32,32 });
	rightAnim.PushBack({ 320, 64,32,32 });
	rightAnim.loop = true;
	rightAnim.mustFlip = true;
	rightAnim.speed = 0.1f;

	// death
	deathAnim.PushBack({ 352, 64,32,32 });
	deathAnim.loop = false;
	deathAnim.mustFlip = false;
	deathAnim.speed = 0.1f;

	currentAnim = &idleAnim;
	state = IDLE;
	direction = RIGHT;
	collider = App->collisions->AddCollider({ 8, 16, 16, 16 }, Collider::Type::ENEMY, (Module*)App->enemies);
	colliderPosition.x = position.x + 8;
	colliderPosition.y = position.y + 16;
	App->sceneLevel_1->grid[(colliderPosition.x - 24) / 16][(colliderPosition.y - 32) / 16] = SceneLevel1::GridType::EMPTY;
}


void CoreMechaWalker::Update()
{
	Entitie::Update();


	switch (state)
	{
	case Entitie::IDLE:

		currentAnim = &idleAnim;
		CheckDirection();
		break;

	case Entitie::MOVE:
		if ((App->frameCounter % 2)) {

			if ((colliderPosition.x - 24) % 16 == 0 && (colliderPosition.y - 32) % 16 == 0) CheckDirection();

			if (state != IDLE)
			{
				if (upAnim.HasFinished() == true) upAnim.mustFlip = !upAnim.mustFlip;
				if (downAnim.HasFinished() == true) downAnim.mustFlip = !downAnim.mustFlip;

				App->sceneLevel_1->grid[(colliderPosition.y - 32) / 16][(colliderPosition.x - 24) / 16] = SceneLevel1::GridType::EMPTY;
				if (direction == UP) position.y--;
				else if (direction == DOWN) position.y++;
				else if (direction == LEFT) position.x--;
				else if (direction == RIGHT) position.x++;

				colliderPosition.x = position.x + 8;
				colliderPosition.y = position.y + 16;

				App->sceneLevel_1->grid[(colliderPosition.y - 32) / 16][(colliderPosition.x - 24) / 16] = SceneLevel1::GridType::MECHA_WALKER;
			}
		}

		break;
	case Entitie::DEATH:
		if (deathAnim.HasFinished() == true) SetToDelete();
		break;
	default:
		break;
	}


}


void CoreMechaWalker::CheckDirection()
{
	Direction avaibleDirections[4];
	int avaibleCount = 0;
	int randDirection = 0;
	bool canContinue = false;

	int x = (colliderPosition.x - 24) / 16;
	int y = (colliderPosition.y - 32) / 16;

	if (y != 10) {
		if (App->sceneLevel_1->grid[y + 1][x] == SceneLevel1::GridType::EMPTY)
		{
			avaibleDirections[avaibleCount] = DOWN;
			avaibleCount++;
		}
	}
	if (y != 0)
	{
		if (y != 5 && x != 6)
		{
			if (App->sceneLevel_1->grid[y - 1][x] == SceneLevel1::GridType::EMPTY)
			{
				avaibleDirections[avaibleCount] = UP;
				avaibleCount++;
			}
		}
	}
	if (x != 0)
	{
		if (App->sceneLevel_1->grid[y][x - 1] == SceneLevel1::GridType::EMPTY)
		{
			avaibleDirections[avaibleCount] = LEFT;
			avaibleCount++;
		}
	}
	if (x != 12) {
		if (App->sceneLevel_1->grid[y][x + 1] == SceneLevel1::GridType::EMPTY)
		{
			avaibleDirections[avaibleCount] = RIGHT;
			avaibleCount++;
		}
	}

	for (int i = 0; i < avaibleCount; i++) {
		if (avaibleDirections[i] == direction) canContinue = true;
	}

	if (canContinue == true)
	{
		int randnum = rand() % (100);

		if (randnum < 60) {
			canContinue = false;
		}
	}

	if (avaibleCount > 0 && canContinue == false)
	{
		randDirection = rand() % avaibleCount;
		direction = avaibleDirections[randDirection];


		switch (direction)
		{
		case Entitie::UP: currentAnim = &upAnim;
			break;
		case Entitie::DOWN: currentAnim = &downAnim;
			break;
		case Entitie::RIGHT: currentAnim = &rightAnim;
			break;
		case Entitie::LEFT: currentAnim = &leftAnim;
			break;
		case Entitie::NONE:
			break;
		default:
			break;
		}
		state = MOVE;
	}
	else state = IDLE;

}

void CoreMechaWalker::OnCollision(Collider* collider)
{
	if (collider->type == Collider::Type::BOMB) {
		state = DEATH;
		currentAnim = &deathAnim;
	}
	if (collider->type == Collider::Type::ENEMY)
	{
		/*if (direction == UP) position.y++;
		else if (direction == DOWN) position.y--;
		else if (direction == LEFT) position.x++;
		else if (direction == RIGHT) position.x--;*/
		switch (direction)
		{
		case UP:
			currentAnim = &downAnim;
			direction = DOWN;
			break;

		case DOWN:
			direction = UP;
			currentAnim = &upAnim;
			break;

		case RIGHT:
			direction = LEFT;
			currentAnim = &leftAnim;
			break;

		case LEFT:
			direction = RIGHT;
			currentAnim = &rightAnim;
			break;
		}
	}
}