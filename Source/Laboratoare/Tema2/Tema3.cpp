#include "Tema3.h"
#include "Object2d.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <Core/Engine.h>

using namespace std;

Tema3::Tema3() {}

Tema3::~Tema3() {}

void Tema3::LoadTextures() {
	const string textureLoc = "Source/Laboratoare/Tema2/Textures/";

	Texture2D* grid = new Texture2D();
	grid->Load2D((textureLoc + "grid.jpg").c_str(), GL_REPEAT);
	mapTextures["grid"] = grid;

	Texture2D* building = new Texture2D();
	building->Load2D((textureLoc + "building1.jpg").c_str(), GL_REPEAT);
	mapTextures["building"] = building;

	Texture2D* background = new Texture2D();
	background->Load2D((textureLoc + "1.jpg").c_str(), GL_CLAMP_TO_BORDER);
	mapTextures["background"] = background;

	Texture2D* red = new Texture2D();
	red->Load2D((textureLoc + "red_persian.jpg").c_str(), GL_REPEAT);
	mapTextures["red"] = red;

	Texture2D* green = new Texture2D();
	green->Load2D((textureLoc + "green4.jpg").c_str(), GL_REPEAT);
	mapTextures["green"] = green;

	Texture2D* yellow = new Texture2D();
	yellow->Load2D((textureLoc + "yellow1.jpg").c_str(), GL_REPEAT);
	mapTextures["yellow"] = yellow;

	Texture2D* pink = new Texture2D();
	pink->Load2D((textureLoc + "80s.gif").c_str(), GL_REPEAT);
	mapTextures["pink_grid"] = pink;

	Texture2D* orange = new Texture2D();
	orange->Load2D((textureLoc + "orange_zebra_print.jpg").c_str(), GL_REPEAT);
	mapTextures["orange"] = orange;

	Texture2D* disco_ball = new Texture2D();
	disco_ball->Load2D((textureLoc + "disco.png").c_str(), GL_REPEAT);
	mapTextures["disco_ball"] = disco_ball;
}

void Tema3::LoadMeshes() {
	Mesh* heart = new Mesh("heart");
	heart->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "heart.obj");
	meshes[heart->GetMeshID()] = heart;

	Mesh* box = new Mesh("box");
	box->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[box->GetMeshID()] = box;

	Mesh* sphere = new Mesh("sphere");
	sphere->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[sphere->GetMeshID()] = sphere;

	Mesh* rectangle = Object2D::CreateRectangle("rectangle", 0.25f, 0.07f);
	AddMeshToList(rectangle);

	Mesh* playButton = Object2D::CreateTriangle("playButton", 0.25f);
	AddMeshToList(playButton);
}

void Tema3::CreateShaders() {
	Shader* playerShader = new Shader("playerShader");
	playerShader->AddShader("Source/Laboratoare/Tema2/Shaders/PlayerVS.glsl", GL_VERTEX_SHADER);
	playerShader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	playerShader->CreateAndLink();
	shaders[playerShader->GetName()] = playerShader;

	Shader* heartShader = new Shader("heartShader");
	heartShader->AddShader("Source/Laboratoare/Tema2/Shaders/HeartVS.glsl", GL_VERTEX_SHADER);
	heartShader->AddShader("Source/Laboratoare/Tema2/Shaders/NormalFragmentShader.glsl", GL_FRAGMENT_SHADER);
	heartShader->CreateAndLink();
	shaders[heartShader->GetName()] = heartShader;

	Shader* tileShader = new Shader("tileShader");
	tileShader->AddShader("Source/Laboratoare/Tema2/Shaders/TileVS.glsl", GL_VERTEX_SHADER);
	tileShader->AddShader("Source/Laboratoare/Tema2/Shaders/TileFS.glsl", GL_FRAGMENT_SHADER);
	tileShader->CreateAndLink();
	shaders[tileShader->GetName()] = tileShader;
	
	Shader* fuelShader = new Shader("fuelShader");
	fuelShader->AddShader("Source/Laboratoare/Tema2/Shaders/FuelVS.glsl", GL_VERTEX_SHADER);
	fuelShader->AddShader("Source/Laboratoare/Tema2/Shaders/NormalFragmentShader.glsl", GL_FRAGMENT_SHADER);
	fuelShader->CreateAndLink();
	shaders[fuelShader->GetName()] = fuelShader;
}

// Creates 5 starting tiles.
void Tema3::CreateStartingTiles() {
	for (int i = 0; i < NUM_COL; i++) {
		vector<Tile> v;
		tiles.push_back(v);
	}
	for (int i = 0; i < NUM_COL; i++) {
		for (int j = 0; j < NUM_GENERATED_TILES; j++) {
			float pos = TILE_LENGTH / 2 - TILE_LENGTH * j;
			Tile t(pos);
			if (i == 0 || i == 2) {
				t.empty = true;
			}
			tiles[i].push_back(t);
		}
	}

	for (int i = 0; i < 10; i++) {
		float pos = TILE_LENGTH / 2 - TILE_LENGTH * i;
		Tile t(pos);
		buildings.push_back(t);
	}
}

void Tema3::InitVariables() {
	velocity = 0;
	fuelScale = 1;
	lockSpeed = false;
	tileSpeed = DEFAULT_TILE_SPEED;
	cameraType = "third person";

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 5.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
}

void Tema3::Init() {
	InitVariables();
	LoadTextures();
	LoadMeshes();
	CreateShaders();
	CreateStartingTiles();
	srand(time(NULL));
}

void Tema3::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

// Draws remaining lives.
void Tema3::RenderLives() {
	for (int i = 0; i < player.lives; i++) {
		if (cameraType == "third person") {
			modelMatrix = glm::mat3(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(3.7 + (0.5 * i), 3.35, -1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.015f, 0.015f, 0.015f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));
			RenderMesh(meshes["heart"], shaders["heartShader"], modelMatrix, RED, mapTextures["disco_ball"]);
		}
	}
}

// Draws play button.
void Tema3::RenderPlayButton() {
	modelMatrix = glm::mat3(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	RenderMesh(meshes["playButton"], shaders["fuelShader"], modelMatrix, GREEN, mapTextures["disco_ball"]);

	modelMatrix = glm::mat3(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.015, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.25, 1.25, 1.25));
	RenderMesh(meshes["playButton"], shaders["fuelShader"], modelMatrix, WHITE, mapTextures["disco_ball"]);
}

// Draws fuel bar.
void Tema3::RenderFuelBar(float deltaTime) {
	glm::vec3 color = GREEN;

	if (fuelScale > 0) {
		fuelScale -= FUEL_DECR * deltaTime;
	}
	if (fuelScale < 0.6) {
		color = ORANGE;
	}
	if (fuelScale < 0.3) {
		color = RED;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.9, 0.8, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(fuelScale, 1, 1));
	RenderMesh(meshes["rectangle"], shaders["fuelShader"], modelMatrix, color, mapTextures["disco_ball"]);

	modelMatrix = glm::mat3(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.9 - 0.005, 0.8 - 0.01, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.05, 1.25, 1.25));
	RenderMesh(meshes["rectangle"], shaders["fuelShader"], modelMatrix, WHITE, mapTextures["disco_ball"]);
}

// Checks if player is jumping or falling and modifies the Y coordinate accordingly.
void Tema3::CheckPlayerVerticalPos(float deltaTime) {
	if (player.state == "jumping") {
		player.translateY += deltaTime * (velocity + JUMP_IMPULSE);
		velocity -= 0.15f;
		if (player.translateY <= 0 && CheckCollision()) {
			player.state = "rolling";
			player.translateY = 0;
			velocity = 0;
		}
	} else {
		if (!CheckCollision()) {
			player.state = "falling";
		}
	}
	if (player.state == "falling") {
		velocity -= 0.15f;
		player.translateY += deltaTime * (velocity);
	}
}

// Checks if player has any powerup.
void Tema3::CheckPlayerPowerup() {
	float currTime = Engine::GetElapsedTime();
	float limit;
	if (!player.orange) {
		limit = POWERUP_TIME_LIMIT;
	} else {
		limit = ORANGE_TIME_LIMIT;
	}
	if (currTime - powerupStartTime >= limit) {
		player.hasPoweup = false;
	}

	currTime = Engine::GetElapsedTime();
	if (currTime - orangeStartTime >= ORANGE_TIME_LIMIT) {
		if (lockSpeed == true) {
			tileSpeed = DEFAULT_TILE_SPEED;
		}
		lockSpeed = false;
		player.orange = false;
	}
}

// Draws the ball.
void Tema3::RenderPlayer(float deltaTime) {
	CheckPlayerVerticalPos(deltaTime);
	CheckPlayerPowerup();

	modelMatrix = glm::mat3(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, SPHERE_RADIUS, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(player.translateX, player.translateY, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	if (player.translateY >= -3) {
		RenderMesh(meshes["sphere"], shaders["playerShader"], modelMatrix, SILVER, mapTextures["disco_ball"]);
	}
}

// Apply upgrades and downgrades if player is on a special tile.
void Tema3::ApplyPowerup() {
	if (tiles[collisionI][collisionJ].color != BLUE && tiles[collisionI][collisionJ].color != PURPLE) {
		player.hasPoweup = true;
		powerupStartTime = Engine::GetElapsedTime();
		if (tiles[collisionI][collisionJ].color == RED) {
			fuelScale = 0;
			player.lives = 0;
			player.dead = true;
		}
		if (tiles[collisionI][collisionJ].color == YELLOW) {
			if (fuelScale > 0.25) {
				fuelScale -= 0.25;
			} else {
				fuelScale = 0;
			}
		}
		if (tiles[collisionI][collisionJ].color == ORANGE) {
			lockSpeed = true;
			tileSpeed = ORANGE_SPEED;
			player.orange = true;
			orangeStartTime = Engine::GetElapsedTime();
		}
		if (tiles[collisionI][collisionJ].color == GREEN) {
			if (fuelScale < 0.85) {
				fuelScale += 0.15;
			} else {
				fuelScale = 1;
			}
		}
	}
	if (tiles[collisionI][collisionJ].color != PURPLE) {
		PrintScore();
	}
	tiles[collisionI][collisionJ].color = PURPLE;
}

void Tema3::RenderBuildings(float deltaTime) {
	for (int i = 0; i < buildings.size(); i++) {
		glm::mat4 modelMatrix = glm::mat3(1);
		buildings[i].translateZ += deltaTime * tileSpeed;

		modelMatrix = glm::translate(modelMatrix, glm::vec3(-8.5, 0, buildings[i].posZ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, buildings[i].translateZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 5, 2));
		RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, RED, mapTextures["building"]);
	
		modelMatrix = glm::mat3(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8.5, 0, buildings[i].posZ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, buildings[i].translateZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 5, 2));
		RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, RED, mapTextures["building"]);

		if (buildings[i].posZ + buildings[i].translateZ >= TILE_LENGTH) {
			buildings.erase(buildings.begin() + i);
			i--;
		}
	}

}

// Draws tiles.
void Tema3::RenderTiles(float deltaTime) {
	for (int i = 0; i < NUM_COL; i++) {
		for (int j = 0; j < tiles[0].size(); j++) {
			glm::mat4 modelMatrix = glm::mat3(1);
			tiles[i][j].translateZ += deltaTime * tileSpeed;
			int x = TILE_DISPLACEMENT * (i - 1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(x, - TILE_HEIGHT / 2, tiles[i][j].posZ));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, tiles[i][j].translateZ));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(TILE_WIDTH, TILE_HEIGHT, TILE_LENGTH));
			if (CheckCollision()) {
				ApplyPowerup();
			}
			if (!tiles[i][j].empty) {
				if (tiles[i][j].color == BLUE) {
					RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, tiles[i][j].color, mapTextures["grid"]);
				}
				if (tiles[i][j].color == PURPLE) {
					RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, tiles[i][j].color, mapTextures["pink_grid"]);
				}
				if (tiles[i][j].color == RED) {
					RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, tiles[i][j].color, mapTextures["red"]);
				}
				if (tiles[i][j].color == GREEN) {
					RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, tiles[i][j].color, mapTextures["green"]);
				}
				if (tiles[i][j].color == YELLOW) {
					RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, tiles[i][j].color, mapTextures["yellow"]);
				}
				if (tiles[i][j].color == ORANGE) {
					RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, tiles[i][j].color, mapTextures["orange"]);
				}
				
			}
			if (tiles[0][j].posZ + tiles[0][j].translateZ >= TILE_LENGTH) {
				tiles[0].erase(tiles[0].begin() + j);
				tiles[1].erase(tiles[1].begin() + j);
				tiles[2].erase(tiles[2].begin() + j);
				j--;
			}
		}
	}
}

// Generates tiles.
void Tema3::GenerateTiles() {
	if (tiles[0].size() < NUM_GENERATED_TILES) {
		for (int i = 0; i < NUM_COL; i++) {
			for (int j = 0; j < NUM_GENERATED_TILES; j++) {
				// chance to be empty or not
				int chance = rand() % 100 + 1;
				float pos = TILE_START_POS - TILE_LENGTH * j;
				Tile t(pos);
				if (chance <= 20) {
					t.empty = true;
				} else {
					// random color
					chance = rand() % 1000 + 1;
					if (chance <= 5) {
						t.color = RED;
					} else {
						if (chance <= 100) {
							t.color = YELLOW;
						} else {
							if (chance <= 110) {
								t.color = ORANGE;
							}
							else {
								if (chance <= 210) {
									t.color = GREEN;
								}
							}
						}
					}
				}
				tiles[i].push_back(t);
			}
		}
	}


	if (buildings.size() <= 10) {
		for (int i = 0; i < 10; i++) {
			float pos = TILE_START_POS - TILE_LENGTH * i;
			Tile t(pos);
			buildings.push_back(t);
		}
	}
}

// Checks if there are any collisions between the player and the tiles.
bool Tema3::CheckCollision() {
	float sphereX = player.translateX;
	float sphereY = player.translateY;
	float sphereZ = 0;

	for (int i = 0; i < NUM_COL; i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			float displacement = TILE_DISPLACEMENT * (i - 1);
			float minX = displacement - TILE_WIDTH / 2;
			float minY = -TILE_HEIGHT;
			float minZ = tiles[i][j].posZ + tiles[i][j].translateZ - TILE_LENGTH / 2;

			float maxX = minX + TILE_WIDTH;
			float maxY = minY + TILE_HEIGHT;
			float maxZ = minZ + TILE_LENGTH;

			float x = fmax(minX, fmin(sphereX, maxX));
			float y = fmax(minY, fmin(sphereY, maxY));
			float z = fmax(minZ, fmin(sphereZ, maxZ));

			float distance = sqrt((x - sphereX) * (x - sphereX) +
				(y - sphereY) * (y - sphereY) +
				(z - sphereZ) * (z - sphereZ));

			if (distance < SPHERE_RADIUS && !tiles[i][j].empty) {
				collisionI = i;
				collisionJ = j;
				return true;
			}
		}
	}
	return false;
}

// Prints score.
void Tema3::PrintScore() {
	player.score++;
	cout << "SCORE:   " << player.score << endl;
}
 
void Tema3::SetCameraPosition() {
	float posX = player.translateX;
	float posY = player.translateY + SPHERE_RADIUS * 3;
	float posZ = -SPHERE_RADIUS;
	if (cameraType == "first person") {
		camera->Set(glm::vec3(posX, posY, posZ), glm::vec3(0, 1, -200), glm::vec3(0, 1, 0));
	} else {
		camera->Set(glm::vec3(0, 2, 5.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	}
}

void Tema3::Update(float deltaTimeSeconds) {
	if (running) {

		glm::mat4 modelMatrix = glm::mat3(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -50, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1000, 1000, 1000));
		RenderMesh(meshes["box"], shaders["tileShader"], modelMatrix, RED, mapTextures["background"]);

		if (!player.dead) {
			GenerateTiles();
		} else {
			tileSpeed = GAME_OVER_TILE_SPEED;
		}

		RenderPlayer(deltaTimeSeconds);
		RenderTiles(deltaTimeSeconds);
		RenderBuildings(deltaTimeSeconds);
		RenderFuelBar(deltaTimeSeconds);
		RenderLives();

		if (player.lives == 0) {
			player.dead = true;
		}

		if (fuelScale <= 0.01) {
			player.lives--;
			if (player.lives == 0) {
				player.dead = true;
				fuelScale = 0;
			} else {
				fuelScale = 1;
			}
		}

		if (player.translateY <= -2) {
			player.dead = true;
			fuelScale = 0;
		}

		if (tiles[0].empty()) {
			running = false;
		}
	} else {
		RenderPlayButton();
	}
}

// Renders an object using the specified shader and the specified position.
void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture) {
	if (!mesh || !shader || !shader->program)
		return;

	//shader->Use();
	glUseProgram(shader->program);

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);
	if (player.hasPoweup) {
		int power = glGetUniformLocation(shader->program, "power");
		glUniform1i(power, 1);
	} else {
		int power = glGetUniformLocation(shader->program, "power");
		glUniform1i(power, 0);
	}
	float time = glGetUniformLocation(shader->program, "time");
	glUniform1f(time, (GLfloat)Engine::GetElapsedTime());

	//TODO : activate texture location 0
	glActiveTexture(GL_TEXTURE0);
	//TODO : Bind the texture1 ID
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	//TODO : Send texture uniform value
	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	//mesh->Render();
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::OnInputUpdate(float deltaTime, int mods) {
	if (window->KeyHold(GLFW_KEY_W)) {
		// Increase speed
		if (tileSpeed < MAX_TILE_SPEED && !lockSpeed) {
			tileSpeed += 0.1;
		}
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		// Move left
		player.translateX -= deltaTime * player.speed;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		// Decrease speed
		if (tileSpeed > MIN_TILE_SPEED && !lockSpeed) {
			tileSpeed -= 0.1;
		}
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		// Move right
		player.translateX += deltaTime * player.speed;
	}
	
}

void Tema3::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_SPACE && player.state == "rolling") {
		// Jump
		player.state = "jumping";
	}
	if (key == GLFW_KEY_C) {
		// Change camera type
		if (cameraType == "first person") {
			cameraType = "third person";
		} else {
			cameraType = "first person";
		}
	}
	if (key == GLFW_KEY_ENTER) {
		// Play game
		if (!running) {
			Init();
			player.reset();
		}
		running = true;
	}
}

void Tema3::FrameEnd() {}

void Tema3::OnKeyRelease(int key, int mods) {}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema3::OnWindowResize(int width, int height) {}
