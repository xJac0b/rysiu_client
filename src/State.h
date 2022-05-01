#pragma once
#include "Gui.h"
#include "Socket.h"
class State;
class StateData
{
public:
	StateData(sf::RenderWindow& window, std::stack<std::unique_ptr<State>>& states, sf::Event& sf_event, sf::Font& font)
		: window(window), states(states), sfEvent(sf_event), font(font)
	{};
	sf::RenderWindow& window;
	std::stack<std::unique_ptr<State>>&states;
	sf::Event& sfEvent;
	sf::Font& font;
	std::set<int, std::greater<int>>scores;
};
class State
{
public:
	State(StateData& state_data);
	
	
	virtual ~State();

	virtual void updateSfmlEvents();
	virtual void updateMousePosition();
	virtual void update(const float& dt) = 0;
	virtual void render() = 0;
protected:
	StateData& stateData;
	sf::Vector2i mousePosWindow;
	std::vector<sf::Vertex>background;
	sf::Texture bgTexture;
	sf::RenderStates state;
	
};

