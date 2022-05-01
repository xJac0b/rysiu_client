#pragma once
#include "LoginState.h"
class App
{
public:
	App();
	~App();
	void updateDt();
	void updateSfmlEvents();
	void update();
	void render();
	void run();
private:
	std::unique_ptr<sf::RenderWindow> window;
	sf::Event event;
	sf::Clock dtClock;
	sf::Font font;
	std::stack<std::unique_ptr<State>>states;
	std::unique_ptr<StateData> stateData;
	float dt;
};

