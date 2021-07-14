#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <direct.h>
#include <filesystem>
#include <vector>
#include <string>
#include "Bird.h"
#include "Pipe.h"
#include "Threshold.h"
#include "Gateway.h"
#include "Boundary.h"
#include "Collider.h"
#include "Particle.h"
#include "Consts.h"

enum class GameState { Menu = 0, Playing, Paused };

const char* highScoreFilePath = "../savedata/HighScore.txt";

//void* operator new(size_t size)
//{
//	printf("allocating %d bytes", size);
//	return malloc(size);
//}

//void operator delete[](void* pointer)
//{
//	printf("deallocating");
//	return free(pointer);
//}

void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
	float aspectRatio = (float)window.getSize().x / (float)window.getSize().y;
	view.setSize(DEFAULT_WINDOW_HEIGHT * aspectRatio, DEFAULT_WINDOW_HEIGHT);
}

void InitSideBars(sf::RectangleShape& leftSideBar, sf::RectangleShape& rightSideBar);

void InitTexts(
	sf::Text& welcomeText, sf::Text& pressEnterText, sf::Text& scoreText, sf::Text& highScoreText, unsigned int& highScore, 
	sf::Text& gamePausedText, sf::Text& pressEscText, const sf::Font& font
);

void ResetGame(Bird& bird, std::vector<Gateway>& gateways, float& gatewaySpeed, unsigned int& score, unsigned int& highScore, sf::Text& highScoreText, std::vector<Particle*> activeParticleSets)
{
	if (score > highScore)
	{
		std::fstream highScoreFile;
		highScoreFile.open(highScoreFilePath, std::ios::out);
		highScoreFile << score;
		highScoreFile.close();
		highScore = score;
		highScoreText.setString("(best " + std::to_string(highScore) + ")");
		highScoreText.setOrigin(highScoreText.getLocalBounds().width / 2, highScoreText.getLocalBounds().height / 2);
	}

	//for (unsigned int i = 0; i < activeParticleSets.size(); i++)
	//{
	//	delete[] activeParticleSets[i];
	//	activeParticleSets.erase(activeParticleSets.begin() + i);
	//}

	score = 0;
	gatewaySpeed = 300.0f;
	bird.Reset();
	for (unsigned int i = 0; i < gateways.size(); i++)
		gateways[i].Reset();
}

int main()
{
	GameState gameState = GameState::Menu;

	sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "flap", sf::Style::Close | sf::Style::Resize /*| sf::Style::Fullscreen*/);

	//float viewHeight = DEFAULT_WINDOW_HEIGHT;
	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));

	sf::Sprite background;
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("../assets/citySkyline.jpg");
	background.setTexture(backgroundTexture);
	background.setColor(sf::Color(210, 162, 255, 255));
	sf::Vector2u backgroundTextureSize = backgroundTexture.getSize();

	Boundary topBoundary(sf::Vector2f(DEFAULT_WINDOW_WIDTH, 30), sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, -30));
	Boundary bottomBoundary(sf::Vector2f(DEFAULT_WINDOW_WIDTH, 30), sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT + 30));

	sf::RectangleShape leftSideBar;
	sf::RectangleShape rightSideBar;
	InitSideBars(leftSideBar, rightSideBar);

	sf::Texture* birdTexture = new sf::Texture;
	birdTexture->loadFromFile("../assets/bird.png");
	Bird bird(birdTexture, sf::Vector2u(1, 2), 150.0f);

	sf::Texture* pipeTexture = new sf::Texture;
	pipeTexture->loadFromFile("../assets/pipe.png");

	srand((unsigned)time(0)); // use time to generate random seed so it is different every time

	std::vector<Gateway> gateways;
	gateways.reserve(3); // minimise dynamic memory copying + allocations by reserving 3 slots

	// initialise pipe starting locations
	Pipe topPipe1(pipeTexture, sf::Vector2f(150, 800), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150)/ 2, 20));
	Pipe bottomPipe1(pipeTexture, sf::Vector2f(150, 800), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2, 1180));
	Threshold threshold1(sf::Vector2f(150, DEFAULT_WINDOW_HEIGHT), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2, 600));
	gateways.emplace_back(topPipe1, bottomPipe1, threshold1);

	int gateway2OffsetY = rand() % 400 - 200;
	Pipe topPipe2(pipeTexture, sf::Vector2f(150, 800), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2 + (DEFAULT_WINDOW_WIDTH + 2 * 150) / 3 - 50, 20 + gateway2OffsetY));
	Pipe bottomPipe2(pipeTexture, sf::Vector2f(150, 800), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2 + (DEFAULT_WINDOW_WIDTH + 2 * 150) / 3 - 50, 1180 + gateway2OffsetY));
	Threshold threshold2(sf::Vector2f(150, DEFAULT_WINDOW_HEIGHT), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2 + (DEFAULT_WINDOW_WIDTH + 2 * 150) / 3 - 50, 600));
	gateways.emplace_back(topPipe2, bottomPipe2, threshold2);
	
	int gateway3OffsetY = rand() % 400 - 200;
	Pipe topPipe3(pipeTexture, sf::Vector2f(150, 800), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2 + 2 * (DEFAULT_WINDOW_WIDTH + 2 * 150) / 3 - 100, 20 + gateway3OffsetY));
	Pipe bottomPipe3(pipeTexture, sf::Vector2f(150, 800), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2 + 2 * (DEFAULT_WINDOW_WIDTH + 2 * 150) / 3 - 100, 1180 + gateway3OffsetY));
	Threshold threshold3(sf::Vector2f(150, DEFAULT_WINDOW_HEIGHT), sf::Vector2f((DEFAULT_WINDOW_WIDTH + 2 * 150) / 2 + 2 * (DEFAULT_WINDOW_WIDTH + 2 * 150) / 3 - 100, 600));
	gateways.emplace_back(topPipe3, bottomPipe3, threshold3);

	const unsigned int particleSetLength = 15;
	std::vector<Particle*> activeParticleSets;
	activeParticleSets.reserve(5); // save 5 slots for particle sets

	sf::Texture particleTexture;
	particleTexture.loadFromFile("../assets/particle.png");

	std::fstream highScoreFile;
	unsigned int highScore;
	highScoreFile.open(highScoreFilePath, std::ios::in);
	if (!highScoreFile)
	{
		_mkdir("../savedata");
		std::ofstream newHighScoreFile { highScoreFilePath };
		newHighScoreFile.close();
		highScore = 0;
	}
	else
	{
		highScoreFile >> highScore;
		highScoreFile.close();
	}

	sf::Font font;
	font.loadFromFile("../assets/PressStart2P.ttf");

	// initialise texts
	// menu texts
	sf::Text welcomeText;
	sf::Text pressEnterText;
	// in game texts
	sf::Text scoreText;
	sf::Text highScoreText;
	// pause menu texts
	sf::Text gamePausedText;
	sf::Text pressEscText;

	InitTexts(welcomeText, pressEnterText, scoreText, highScoreText, highScore, gamePausedText, pressEscText, font);

	sf::Clock clock;
	float deltaTime = 0.0f;

	sf::Clock timer;
	float elapsedTime = 0.0f;

	float timeSinceLastEsc = 0.0f;

	int newYOffset = 0;
	float gatewaySpeed = 300.0f;

	sf::Vector2f intitialParticleVelocity;

	unsigned int score = 0;

	bool jumped = false;

	bool resetGame;

	// game loop
	while (window.isOpen())
	{
		resetGame = false;

		elapsedTime = timer.getElapsedTime().asSeconds();
		deltaTime = clock.restart().asSeconds();
		// artificially set delta time if game lags to avoid clipping through objects
		if (deltaTime > 1.0f / 20.0f)
			deltaTime = 1.0f / 20.0f;

		timeSinceLastEsc += deltaTime;

		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				ResizeView(window, view);
			default:
				break;
			}
		}

		view.setCenter(sf::Vector2f(bird.GetPosition().x * 2, DEFAULT_WINDOW_HEIGHT / 2));

		window.clear();
		window.draw(background);

		// gamestate checks
		if (gameState == GameState::Menu)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				gameState = GameState::Playing;
				continue;
			}

			window.draw(welcomeText);
			window.draw(pressEnterText);
		}

		else if (gameState == GameState::Playing)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && timeSinceLastEsc > 0.2f)
			{
				gameState = GameState::Paused;
				timeSinceLastEsc = 0.0f;
				continue;
			}
			
			// collision checks
			Collider birdCollider = bird.GetCollider();
			for (unsigned int i = 0; i < gateways.size(); i++)
			{
				Collider topPipeCollider = gateways[i].topPipe.GetCollider();
				Collider bottomPipeCollider = gateways[i].bottomPipe.GetCollider();
				Collider thresholdCollider = gateways[i].threshold.GetCollider();

				if (birdCollider.CheckCollision(topPipeCollider) || birdCollider.CheckCollision(bottomPipeCollider))
				{
					deltaTime = 0.0f;
					ResetGame(bird, gateways, gatewaySpeed, score, highScore, highScoreText, activeParticleSets);
					resetGame = true;
				}

				if (birdCollider.CheckCollision(thresholdCollider) && gateways[i].threshold.IsEnabled())
				{
					score += 1;
					gateways[i].threshold.ToggleOff(); // so further collisions with the same threshold are disabled
				}
			}
			Collider topBoundaryCollider = topBoundary.GetCollider();
			Collider bottomBoundaryCollider = bottomBoundary.GetCollider();
			if (birdCollider.CheckCollision(topBoundaryCollider) || birdCollider.CheckCollision(bottomBoundaryCollider))
			{
				deltaTime = 0.0f;
				ResetGame(bird, gateways, gatewaySpeed, score, highScore, highScoreText, activeParticleSets);
				resetGame = true;
			}

			// update particle positions + opacities
			for (Particle* particleSet : activeParticleSets)
			{
				for (unsigned int i = 0; i < particleSetLength; i++)
					particleSet[i].Update(deltaTime);
			}

			// update bird position and animation
			bird.Update(deltaTime, jumped);
			if (jumped)
			{
				jumped = false;

				// make new particle set
				Particle* particleSet = new Particle[particleSetLength];
				for (unsigned int i = 0; i < particleSetLength; i++)
				{
					intitialParticleVelocity = sf::Vector2f(-rand() % 100 / 200.0f, rand() % 100 / 200.0f);
					float rotationPerSecond = rand() % 1440 - 720;
					particleSet[i] = Particle(
						&particleTexture, sf::Vector2f(10.0f, 10.0f), 
						sf::Vector2f(bird.GetPosition().x - (bird.GetSize().x / 2.0f) + 30.0f, bird.GetPosition().y), 
						intitialParticleVelocity, rotationPerSecond
					);
				}
				activeParticleSets.push_back(particleSet);
			}

			// delete invisible particles
			for (unsigned int i = 0; i < activeParticleSets.size(); i++)
			{
				if (activeParticleSets[i][0].GetOpacity() == 0.0f)
				{
					delete[] activeParticleSets[i];
					activeParticleSets.erase(activeParticleSets.begin() + i);
				}
			}

			// move gateways each frame (use deltaTime to normalise movement across different frame rates)
			newYOffset = rand() % 400 - 200;
			bool offScreen;
			for (unsigned int i = 0; i < gateways.size(); i++)
			{
				offScreen = false;
				gateways[i].MoveX(gatewaySpeed, newYOffset, deltaTime, &offScreen);
				// close gates by 20px every time they go off screen to make it more difficult over time
				if (offScreen)
				{
					gateways[i].CloseBy(40);
					if (gatewaySpeed < 900.0f) gatewaySpeed += 10.0f;
					offScreen = false;
				}
			}

			for (Particle* particleSet : activeParticleSets)
				for (int i = 0; i < particleSetLength; i++)
					particleSet[i].Draw(window);
			bird.Draw(window);
			for (Gateway gateway : gateways)
				gateway.Draw(window);
			scoreText.setString(std::to_string(score));
			scoreText.setOrigin(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2);
			window.draw(scoreText);
			window.draw(highScoreText);
		}

		else if (gameState == GameState::Paused)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && timeSinceLastEsc > 0.2f)
			{
				gameState = GameState::Playing;
				timeSinceLastEsc = 0.0f;
				continue;
			}
			bird.Draw(window);
			for (Gateway gateway : gateways)
				gateway.Draw(window);
			window.draw(gamePausedText);
			window.draw(pressEscText);
		}

		window.draw(leftSideBar);
		window.draw(rightSideBar);
		window.setView(view);
		window.display();
	}
}

void InitSideBars(sf::RectangleShape& leftSideBar, sf::RectangleShape& rightSideBar)
{
	leftSideBar.setSize(sf::Vector2f(1000, DEFAULT_WINDOW_HEIGHT));
	leftSideBar.setOrigin(leftSideBar.getSize() / 2.0f);
	leftSideBar.setPosition(sf::Vector2f(-500, DEFAULT_WINDOW_HEIGHT / 2));
	leftSideBar.setFillColor(sf::Color(0, 0, 0, 255));
	
	rightSideBar.setSize(sf::Vector2f(1000, DEFAULT_WINDOW_HEIGHT));
	rightSideBar.setOrigin(rightSideBar.getSize() / 2.0f);
	rightSideBar.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH + 500, DEFAULT_WINDOW_HEIGHT / 2));
	rightSideBar.setFillColor(sf::Color(0, 0, 0, 255));
}

void InitTexts(
	sf::Text& welcomeText, sf::Text& pressEnterText, sf::Text& scoreText, sf::Text& highScoreText, unsigned int& highScore,
	sf::Text& gamePausedText, sf::Text& pressEscText, const sf::Font& font)
{
	welcomeText.setFont(font);
	welcomeText.setCharacterSize(30);
	welcomeText.setString("welcome to flap");
	welcomeText.setOrigin(welcomeText.getLocalBounds().width / 2, welcomeText.getLocalBounds().height / 2);
	welcomeText.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, 100));

	pressEnterText.setFont(font);
	pressEnterText.setCharacterSize(24);
	pressEnterText.setString("press enter to begin");
	pressEnterText.setOrigin(pressEnterText.getLocalBounds().width / 2, pressEnterText.getLocalBounds().height / 2);
	pressEnterText.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT / 2 - 100));

	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, 50));

	highScoreText.setFont(font);
	highScoreText.setCharacterSize(24);
	highScoreText.setString("(best " + std::to_string(highScore) + ")");
	highScoreText.setOrigin(highScoreText.getLocalBounds().width / 2, highScoreText.getLocalBounds().height / 2);
	highScoreText.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, 110));

	gamePausedText.setFont(font);
	gamePausedText.setCharacterSize(30);
	gamePausedText.setString("game paused");
	gamePausedText.setOrigin(gamePausedText.getLocalBounds().width / 2, gamePausedText.getLocalBounds().height / 2);
	gamePausedText.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, 100));

	pressEscText.setFont(font);
	pressEscText.setCharacterSize(24);
	pressEscText.setString("press esc to resume");
	pressEscText.setOrigin(pressEscText.getLocalBounds().width / 2, pressEscText.getLocalBounds().height / 2);
	pressEscText.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT / 2 - 100));
}
