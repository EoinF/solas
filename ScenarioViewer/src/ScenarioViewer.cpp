﻿#include "ScenarioViewer.h"
#include <filesystem>

void init();
void update();
void updateUI();
float calculateFrameRate(float timeDelta);

int WINDOW_WIDTH = 1080;
int WINDOW_HEIGHT = 720;
int TILE_SIZE = 4;

Game game(TILE_SIZE, WINDOW_WIDTH / TILE_SIZE, WINDOW_HEIGHT / TILE_SIZE);

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dynamic Lightmap Test",
						sf::Style::Default, sf::ContextSettings(0U, 0U, 8U));
sf::Clock gameClock;
sf::Font font;
sf::Text fpsLabel;
sf::Text scenarioLabel;
sf::Sprite lightmapOverlaySprite;
float totalElapsed = 0;
int framesThisSecond = 0;
int totalSeconds = 0;
int totalFrames = 0;
float inputTimeout = 0;

bool showOverlay = true;
bool isMouseClicked = false;
bool wasMouseClicked = false;

int main() {
	init();
	while (window.isOpen()) {
		update();
	}
	return 0;
}

void init() {
	std::string path = std::filesystem::current_path();
	font.loadFromFile(path.append("/assets/Arial.ttf"));

	scenarioLabel.setFont(font);
	scenarioLabel.setFillColor(sf::Color::White);
	scenarioLabel.setString("Scenario: 1");
	scenarioLabel.setPosition(10, 10);

	fpsLabel.setFont(font);
	fpsLabel.setFillColor(sf::Color::White);
	fpsLabel.setString("...");
	fpsLabel.setPosition(10, 40);

	lightmapOverlaySprite.setScale((float)TILE_SIZE, (float)TILE_SIZE);
	lightmapOverlaySprite.setTexture(game.lightmapOverlay);
	glEnable(GL_TEXTURE_2D);
	// window.setFramerateLimit(60);
}

float calculateFrameRate(float timeDelta) {
	totalElapsed += timeDelta;
	framesThisSecond++;

	if (totalElapsed > 1.0f) {
		totalSeconds++;
		totalFrames += framesThisSecond;
		fpsLabel.setString("current: " + std::to_string(framesThisSecond) +
						   " average: " + std::to_string(totalFrames / totalSeconds));
		framesThisSecond = 0;
		totalElapsed -= 1.0f;
	}
	return timeDelta;
}

void update() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
	}

	float timeDelta = gameClock.restart().asSeconds();
	calculateFrameRate(timeDelta);

	if (inputTimeout < 0) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			game.previousScenario();
			updateUI();
			inputTimeout = 0.3f;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			game.nextScenario();
			updateUI();
			inputTimeout = 0.3f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			showOverlay = !showOverlay;
			inputTimeout = 0.3f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			auto currentMode = game.getPlacementMode();
			if (currentMode == PlacementMode::LIGHT) {
				currentMode = PlacementMode::WALL;
			} else {
				currentMode = PlacementMode::LIGHT;
			}
			game.setPlacementMode(currentMode);
			inputTimeout = 0.3f;
		}
	} else {
		inputTimeout -= timeDelta;
	}

	glm::vec2 deltaPosition(0, 0);
	float deltaSpan = 0.0f;
	float moveSpeed = 600.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		moveSpeed = 100.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		deltaPosition.y -= timeDelta * moveSpeed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		deltaPosition.y += timeDelta * moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		deltaPosition.x -= timeDelta * moveSpeed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		deltaPosition.x += timeDelta * moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		deltaSpan += timeDelta * 5.0f;
	}

	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	wasMouseClicked = isMouseClicked;
	isMouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	// clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	window.pushGLStates();

	game.update(mousePosition, deltaPosition, deltaSpan, wasMouseClicked && !isMouseClicked,
				timeDelta);

	window.clear(sf::Color::Blue);
	for (auto &element : game.sprites) {
		window.draw(*element.second);
	}
	if (showOverlay) {
		window.draw(lightmapOverlaySprite);
	}
	for (auto &spritePtr : game.debugSprites) {
		window.draw(*spritePtr);
	}
	window.draw(scenarioLabel);
	window.draw(fpsLabel);
	window.popGLStates();
	window.display();
}

void updateUI() {
	totalFrames = 0;
	totalElapsed = 0;
	totalSeconds = 0;
	framesThisSecond = 0;
	scenarioLabel.setString("Scenario: " + std::to_string(game.scenarioIndex + 1));
}