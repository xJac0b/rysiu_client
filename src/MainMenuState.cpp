#include "pch.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateData& state_data, sf::String&& path,
	std::tuple < sf::String, sf::String, sf::String, int, int, int>& data) : State(state_data),panel(4)
{
	//Init backgorund
	this->bgTexture.loadFromFile(path);
	state.texture = &this->bgTexture;
	gui::setVertexShape(this->background, {0.f, 0.f, 
		static_cast<float>(this->stateData.window.getSize().x),
		static_cast<float>(this->stateData.window.getSize().y)}, 0);
	for (auto& i : this->background)
		i.color = sf::Color(70, 70, 70, 250);
	//Init panel
	gui::setVertexShape(this->panel, { 50.f, 185.f, 700.f, 350.f });
	for (auto& i : this->panel)
		i.color = sf::Color(30, 30, 30,200);
	//init title
	this->title.setFont(this->stateData.font);
	this->title.setString("Rysiu");
	this->title.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 20));
	this->title.setPosition(this->stateData.window.getSize().x / 2.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->title.setOutlineThickness(2.f);
	this->title.setOutlineColor(sf::Color::Black);

	//init welcome text
	this->welcome.setFont(this->stateData.font);
	this->welcome.setString("Witaj " + std::get<0>(data) +"!");
	this->welcome.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 40));
	this->welcome.setPosition(gui::p2pX(20.f, this->stateData.window.getSize()), gui::p2pY(55.f, this->stateData.window.getSize()));
	this->welcome.setOutlineThickness(2.f);
	this->welcome.setOutlineColor(sf::Color::Black);
	//buttons
	this->buttons.push_back(gui::Button(
		sf::FloatRect(28.f, 33.33f, 14.5f, 10.f),
		"PLAY",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 20),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(15.f, 80.f, 14.f, 8.f),
		"Stats",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(37.5f, 80.f, 26.f, 8.f),
		"Leaderboards",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(72.f, 80.f, 14.f, 8.f),
		"Quit",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(),50),
		this->stateData.window.getSize()));
	std::vector<std::pair<sf::String, bool>>temp{ {"Resources/Images/Skins/abc.png", true}, {"Resources/Images/Skins/abcd.png", false} };
	this->skins = std::make_unique<gui::ButtonImageList>(temp,
		sf::FloatRect(62.5f, 33.33f, 25.f, 33.33f), sf::FloatRect(0.f, 0.f, 200.f, 200.f),
		this->stateData.font,
		100, this->stateData.window.getSize());
}

MainMenuState::~MainMenuState()
{
}


void MainMenuState::update(const float& dt)
{
	this->updateMousePosition();
	this->skins->update(this->mousePosWindow);
	for (auto& i : this->buttons)
	{
		i.update(this->mousePosWindow);
	}

	if (this->buttons[0].isClicked())
	{
		this->stateData.states.push(std::make_unique<GameState>(this->stateData, "Resources/Images/background.jpg"));
	}	
	else if (this->buttons[3].isClicked())
	{
		this->stateData.states.pop();
	}

}

void MainMenuState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(&this->panel[0], 4, sf::Quads);
	this->stateData.window.draw(this->title);
	this->skins->render(this->stateData.window);
	for (auto& i : this->buttons)
	{
		i.render(this->stateData.window);
	}
	this->stateData.window.draw(this->welcome);
}
