#include "ScenarioViewer.h"

using namespace std;

void init(string path);
void update();
void updateUI();
float calculateFrameRate(float timeDelta);

int WINDOW_WIDTH = 1080;
int WINDOW_HEIGHT = 720;
int TILE_SIZE = 8;

Game game(TILE_SIZE, WINDOW_WIDTH / TILE_SIZE, WINDOW_HEIGHT / TILE_SIZE);

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dynamic Lightmap Test", 7U, sf::ContextSettings(0U, 0U, 8));
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

// Hacky solution to get the current path
// but it's fine because this app isn't meant to be distributed anyway
string get_current_path(string executablePath)
{
    while (executablePath[executablePath.length() - 1] != '/' && executablePath[executablePath.length() - 1] != '\\')
    {
        executablePath = executablePath.substr(0, executablePath.length() - 2);
    }
    return executablePath;
}

int main(int argc, char **argv)
{
    init(get_current_path(argv[0]));
    while (window.isOpen())
    {
        update();
    }
    return 0;
}

void init(string path)
{
    window.setTitle(path);

    font.loadFromFile(path.append("assets/Arial.ttf"));
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
    //window.setFramerateLimit(60);
}

float calculateFrameRate(float timeDelta)
{
    totalElapsed += timeDelta;
    framesThisSecond++;

    if (totalElapsed > 1.0f)
    {
        totalSeconds++;
        totalFrames += framesThisSecond;
        fpsLabel.setString("current: " + to_string(framesThisSecond) + " average: " + to_string(totalFrames / totalSeconds));
        framesThisSecond = 0;
        totalElapsed -= 1.0f;
    }
    return timeDelta;
}

void update()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    float timeDelta = gameClock.restart().asSeconds();
    calculateFrameRate(timeDelta);

    if (inputTimeout < 0)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            game.previousScenario();
            updateUI();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            game.nextScenario();
            updateUI();
        }
    }
    else
    {
        inputTimeout -= timeDelta;
    }

    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    game.update(mousePosition, timeDelta);

    window.clear(sf::Color::Blue);
    for (auto element : game.sprites)
    {
        window.draw(*(element.second));
    }
    for (auto &spritePtr : game.debugSprites)
    {
        window.draw(*spritePtr);
    }
    window.draw(lightmapOverlaySprite);
    window.draw(scenarioLabel);
    window.draw(fpsLabel);
    window.display();
}

void updateUI()
{
    totalFrames = 0;
    totalElapsed = 0;
    totalSeconds = 0;
    framesThisSecond = 0;
    inputTimeout = 0.3f;
    scenarioLabel.setString("Scenario: " + to_string(game.scenarioIndex + 1));
}