#include "Game.h"
#include "ResourceLoader.h"
#include "sprite_renderer.h"
#include "ParticleGenerator.h"
#include "BallObject.h"
#include "PostProcessor.h"

spriteRenderer* renderer;
PostProcessor* Effects;

// Collision return data
typedef std::tuple<bool, Direction, glm::vec2> Collision;

// player
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
GameObject* player;

// ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
BallObject* ball;

// Particles
ParticleGenerator* Particles;

// post-effect constants
float shakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), keys(), width(width), height(height)
{

}

Game::~Game()
{
	delete renderer;
}

void Game::Init()
{
	ResourceLoader::LoadShader("./default.vert", "./default.frag", nullptr, "sprite");
	ResourceLoader::LoadShader("./particle.vert", "./particle.frag", nullptr, "particle");
	ResourceLoader::LoadShader("./post_effects.vert", "./post_effects.frag", nullptr, "post_effects");
	
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
	Shader shader = ResourceLoader::getShader("sprite");
	shader.use().setInt("image", 0);
	shader.setMatrix4("projection", projection);
	renderer = new spriteRenderer(shader);
	shader = ResourceLoader::getShader("particle");
	shader.use().setInt("sprite", 0);
	shader.setMatrix4("projection", projection);
	ResourceLoader::loadTexture("./smily.png", true, "face");
	ResourceLoader::loadTexture("./background.jpg", false, "background");

	ResourceLoader::loadTexture("./block.png", false, "block");
	ResourceLoader::loadTexture("./block_solid.png", false, "block_solid");
	ResourceLoader::loadTexture("./paddle.png", true, "paddle");

	ResourceLoader::loadTexture("./particle.png", true, "particle");

	// load levels
	GameLevel one; one.Load("./1.lvl", width, height / 2);
	levels.push_back(one);
	level = 0;

	// player
	glm::vec2 playerPos = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceLoader::getTexture("paddle"));

	// ball
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, BALL_RADIUS * 2.0f);
	ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceLoader::getTexture("face"));

	// Particles
	Particles = new ParticleGenerator(ResourceLoader::getShader("particle"), ResourceLoader::getTexture("particle"), 500);

	// Post-Processor
	Effects = new PostProcessor(ResourceLoader::getShader("post_effects"), width, height);
}

void Game::update(float dt)
{
	ball->Move(dt, width);
	DoCollisions();
	if (ball->position.y >= height)
	{
		// reset level
		this->levels[0].Load("./1.lvl", this->width, this->height / 2);
		// reset player
		player->size = PLAYER_SIZE;
		player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
		ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
	}

	// update particles
	Particles->Update(dt, *ball, 2, glm::vec2(ball->radius / 2.0f));

	// post-effects
	if (shakeTime > 0.0f)
	{
		shakeTime -= dt;
		if (shakeTime <= 0.0f)
			Effects->shake = false;
	}
}

void Game::processInput(float dt)
{
	if (State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (keys[GLFW_KEY_A])
		{
			if (player->position.x >= 0.0f)
				player->position.x -= velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			if (player->position.x <= width - player->size.x)
				player->position.x += velocity;
		}
		if (keys[GLFW_KEY_SPACE])
			ball->onPaddle = false;
	}
}

// check collisions
bool checkCollision(GameObject& obj1, GameObject& obj2);
Collision checkCollision(BallObject& obj1, GameObject& obj2);
Direction vectorDirection(glm::vec2 closest);

bool checkCollision(GameObject& obj1, GameObject& obj2)
{
	bool collisionX = obj1.position.x + obj1.size.x >= obj2.position.x && obj2.position.x + obj2.size.x >= obj1.position.x;
	bool collisionY = obj1.position.y + obj1.size.y >= obj2.position.y && obj2.position.y + obj2.size.y >= obj1.position.y;
	return collisionX && collisionY;
}

Collision checkCollision(BallObject& one, GameObject& two)
{
	glm::vec2 center(one.position + one.radius);
	glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
	glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;
	
	if (glm::length(difference) <= one.radius)
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Game::DoCollisions()
{
	for (GameObject& box : levels[level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = checkCollision(*ball, box);
			if (std::get<0>(collision))
			{
				if (!box.isSolid)
					box.Destroyed = true;
				else {
					shakeTime = 0.05f;
					Effects->shake = true;
				}
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT)
				{
					ball->velocity.x = -ball->velocity.x;
					float penetration = ball->radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						ball->position.x += penetration;
					else
						ball->position.x -= penetration;
				}
				else
				{
					ball->velocity.y = -ball->velocity.y;
					float penetration = ball->radius - std::abs(diff_vector.y);
					if (dir == UP)
						ball->position.y -= penetration;
					else 
						ball->position.y += penetration;
				}
			}
		}
	}

	// Ball-Player collision
	Collision result = checkCollision(*ball, *player);
	if (!ball->onPaddle && std::get<0>(result))
	{
		float centerBoard = player->position.x + player->size.x / 2.0f;
		float distance = (ball->position.x + ball->radius) - centerBoard;
		float percent = distance / (player->size.x / 2.0f);
		// move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = ball->velocity;
		ball->velocity.x = INITIAL_BALL_VELOCITY.x * percent * strength;
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
		ball->velocity.y = -1.0f * abs(ball->velocity.y);
		//ball->velocity = glm::normalize(ball->velocity);
	}
}

void Game::render()
{
	if (State == GAME_ACTIVE)
	{
		Effects->BeginRender();
			Texture texture = ResourceLoader::getTexture("background");
			renderer->drawSprite(texture, glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
			// draw level
			levels[level].Draw(*renderer);
			// draw player
			player->Draw(*renderer);
			// draw particles
			Particles->Draw();
			// draw ball
			ball->Draw(*renderer);
		Effects->EndRender();
		Effects->confuse = true;
		Effects->Render(glfwGetTime());
	}
}

Direction vectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;
}