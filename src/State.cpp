#include "pch.h"
#include "State.h"

State::State(StateData& state_data) : stateData(state_data)
{
	this->background.resize(4, sf::Vertex());
	gui::setVertexShape(this->background, { 0.f, 0.f, static_cast<float>(this->stateData.window.getSize().x),
		static_cast<float>(this->stateData.window.getSize().y) });
}

State::~State()
{
}

void State::updateSfmlEvents()
{
}

void State::updateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(this->stateData.window);
}