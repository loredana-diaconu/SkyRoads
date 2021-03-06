#pragma once

#include <Component/SimpleScene.h>
#include "Camera.h"
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#define LIVES 3
#define Z_NEAR 0.01f
#define Z_FAR 2000.0f
#define RADIUS 0.5f
#define JUMP_LIMIT 2.f
#define PLAYER_SPEED 3.f
#define NUM_COL 3
#define NUM_GENERATED_TILES 5
#define TILE_WIDTH 2.f
#define TILE_HEIGHT 0.1f
#define TILE_LENGTH 5.f
#define TILE_DISPLACEMENT 3
#define DEFAULT_TILE_SPEED 6.f
#define MAX_TILE_SPEED 10.f
#define MIN_TILE_SPEED 2.f
#define ORANGE_SPEED 15.f
#define TILE_START_POS -20.f
#define SPHERE_RADIUS 0.25f
#define JUMP_IMPULSE 5
#define PLAYER_ACCELERATION 0.1
#define SILVER glm::vec3(0.7f, 0.7f, 0.9f)
#define BLUE glm::vec3(0.3f, 0.1f, 1.f)
#define WHITE  glm::vec3(1, 1, 1)
#define PURPLE glm::vec3(223.f / 255.f, 33.f / 255.f, 198.f / 255.f)
#define RED glm::vec3(200.f / 255.f, 30.f / 255.f, 40.f / 255.f)
#define YELLOW glm::vec3(255.f / 255.f, 211.f / 255.f, 9.f / 255.f)
#define ORANGE glm::vec3(253.f / 255.f, 115.f / 255.f, 48.f / 255.f)
#define GREEN glm::vec3(0.f, 0.8f, 0.6f)
#define ORANGE_TIME_LIMIT 7.f
#define POWERUP_TIME_LIMIT 0.5f
#define FUEL_DECR 0.02f
#define GAME_OVER_TILE_SPEED  20.f

class Player {
public:
	std::string state;
	float translateX; 
	float translateY;
	float speed;
	bool hasPoweup;
	bool dead;
	bool orange;
	int lives;
	int score;

	Player() {
		reset();
	}

	~Player() {}

	void reset() {
		state = "rolling";
		translateX = 0;
		translateY = 0;
		speed = PLAYER_SPEED;
		hasPoweup = false;
		dead = false;
		orange = false;
		lives = LIVES;
		score = 0;
	}
};

class Tile {
public:
	float translateZ;
	float speed;
	float posZ;
	bool empty;
	glm::vec3 color;

	Tile(float posZ) {
		translateZ = 0;
		speed = PLAYER_SPEED;
		this->posZ = posZ;
		empty = false;
		color = BLUE;
	}

	~Tile() {}
};

class Tema3 : public SimpleScene {
public:
	Tema3();
	~Tema3();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture1);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void LoadMeshes();
	void LoadTextures();
	void CreateShaders();
	void CreateStartingTiles();
	void InitVariables();

	void SetCameraPosition();
	void ApplyPowerup();
	void CheckPlayerPowerup();
	void CheckPlayerVerticalPos(float deltaTime);
	void GenerateTiles();
	void RenderPlayer(float deltaTime);
	void RenderBuildings(float deltaTime);
	void RenderLives();
	void RenderTiles(float deltaTime);
	void RenderFuelBar(float deltaTime);
	void RenderPlayButton();
	bool CheckCollision();
	void PrintScore();

protected:
	Laborator::Camera* camera;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	std::unordered_map<std::string, Texture2D*> mapTextures;
	float FoV = RADIANS(60);
	float width = 30.f;
	float height = 30.f;
	float velocity;
	Player player;
	std::vector<std::vector<Tile>> tiles;
	std::vector<Tile> buildings;
	int collisionI;
	int collisionJ;
	float tileSpeed;
	float powerupStartTime;
	float orangeStartTime = 1000;
	std::string cameraType;
	float fuelScale;
	bool lockSpeed;
	bool running = false;
};
