#include "pch.h"
#include "GameState.h"

GameState::GameState(StateData& state_data, sf::String&& path) : State(state_data),
player(sf::FloatRect(0, 470, 128, 128), sf::FloatRect(10, 0, 110, 128), sf::String("Resources/Images/Skins/abc.png"), { 400.f,100.f },
	2.f, 1.f)
{
	//Init backgorund
	this->bgTexture.loadFromFile(path);
	state.texture = &this->bgTexture;
	gui::setVertexShape(this->background, { 0.f, 0.f,
		static_cast<float>(this->stateData.window.getSize().x),
		static_cast<float>(this->stateData.window.getSize().y) }, 0);
	for (auto& i : this->background)
		i.color = sf::Color(70, 70, 70, 250);
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(300, 400, 160, 40), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 160, 40), "Resources/Images/platform.png"));
}

GameState::~GameState()
{
}

void GameState::updateSfmlEvents()
{
}


void GameState::update(const float& dt)
{
	this->updateMousePosition();
	this->player.update(dt, this->mousePosWindow);
	for (auto& i : this->obstacles)
	{
		sf::FloatRect pbnd = this->player.getBounds(), obnd = i->getBounds();
		if (pbnd.intersects(obnd))
			std::cout << "kolizja";
	}
}

void GameState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(this->title);
	this->player.render(this->stateData.window);
	for (auto& i : this->obstacles)
		i->render(this->stateData.window);

}
