#include "pch.h"
#include "GameState.h"
#include "Gui.h"6

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
	this->d_fps.setString(std::to_string(60.0f));
	this->d_fps.setFont(this->stateData.font);
	this->d_fps.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 40));
	this->d_fps.setPosition(this->stateData.window.getSize().x / 2.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->d_fps.setOutlineThickness(4.f);
	this->d_fps.setOutlineColor(sf::Color::Black);
	this->d_fps.setFillColor(sf::Color::White);

	this->d_pos.setString(std::to_string(60.0f));
	this->d_pos.setFont(this->stateData.font);
	this->d_pos.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 40));
	this->d_pos.setPosition(this->stateData.window.getSize().x / 5.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->d_pos.setOutlineThickness(4.f);
	this->d_pos.setOutlineColor(sf::Color::Black);
	this->d_pos.setFillColor(sf::Color::White);


}

GameState::~GameState()
{
}

void GameState::updateSfmlEvents()
{
}

void GameState::set_dFPS(const float& fps) {
	this->d_fps.setString("fps:"+std::to_string(fps));
}
void GameState::set_dPOS(const sf::FloatRect& pos) {
	this->d_fps.setString("pos:" + std::to_string(pos.left) + " " + std::to_string(pos.top));
}

void GameState::update(const float& dt)
{
	this->dt = this->dtClock.restart().asSeconds(); //Delta time
	//this->set_dFPS(std::floor(1.0f / dt));
		
	
	this->player.update(dt, this->mousePosWindow);
	this->set_dPOS(this->player.getBounds());

	for (auto& i : this->obstacles)
	{
		sf::FloatRect pbnd = this->player.getBounds(), obnd = i->getBounds();
		if (pbnd.intersects(obnd)) {
			if (pbnd.left < obnd.left + obnd.width&& 1==2) {
				this->player.setVelocityX(64.0f);

			}
			if (pbnd.top + pbnd.height >= obnd.top && !(pbnd.top + pbnd.height > obnd.top) + obnd.height && pbnd.left + pbnd.width >= obnd.left && pbnd.left <= obnd.left + obnd.width) {
				this->player.setOnPlatform(true);
				this->player.setJumping(false);

				this->player.setVelocityY(-0.2f);
				
			}
			if (pbnd.top < obnd.top + obnd.height && pbnd.top+pbnd.height > obnd.top+obnd.height && obnd.top && pbnd.left + pbnd.width >obnd.left && pbnd.left < obnd.left + obnd.width) {
				this->player.setVelocityY(256.0f);
			}
		}
			//std::cout << "kolizja";
	}
	this->updateMousePosition();
}

void GameState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(this->title);
	this->stateData.window.draw(this->d_fps);
	this->player.render(this->stateData.window);
	for (auto& i : this->obstacles)
		i->render(this->stateData.window);

}
