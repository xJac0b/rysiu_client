#include "pch.h"
#include "ProfileState.h"

sf::String textWrap(sf::String str, int location) {
	int length = 0;
	std::wstringstream wss;
	for (auto& i : str)
	{
		length++;
		wss << (char)i;
		if (i == ' ' && length > 26)
		{
			wss << "\n";
			length = 0;
		}
			
	}
	return wss.str();
}

ProfileState::ProfileState(StateData& state_data, sf::String&& path,
	std::tuple < sf::String, sf::String, int, int, std::string, int,int,int,int> data) : State(state_data), panel(4)
{	
	//Init backgorund
	this->bgTexture.loadFromFile(path);
	state.texture = &this->bgTexture;
	gui::setVertexShape(this->background, { 0.f, 0.f,
		static_cast<float>(this->stateData.window.getSize().x),
		static_cast<float>(this->stateData.window.getSize().y) }, 0);
	for (auto& i : this->background)
		i.color = sf::Color(70, 70, 70, 250);
	//Init panel
	gui::setVertexShape(this->panel, { 50.f, 185.f, 700.f, 350.f });
	for (auto& i : this->panel)
		i.color = sf::Color(30, 30, 30, 200);
	//init title
	this->title.setFont(this->stateData.font);
	this->title.setString("Profil");
	this->title.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 20));
	this->title.setPosition(this->stateData.window.getSize().x / 2.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->title.setOutlineThickness(2.f);
	this->title.setOutlineColor(sf::Color::Black);

	//buttons
	this->buttons.push_back(gui::Button(
		sf::FloatRect(72.f, 80.f, 14.f, 8.f),
		L"WyjdŸ",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()));

	/*this->panelup.setPosition(50.f, 185.f);
	this->panelup.setSize({ 700.f, 70.f });
	this->panelup.setFillColor(sf::Color(20, 20, 20, 220));
	this->paneldown.setPosition(50.f, 465.f);
	this->paneldown.setSize({ 700.f, 70.f });
	this->paneldown.setFillColor(sf::Color(20, 20, 20, 220));*/
	//this->avatar.loadFromMemory(std::get<4>(this->data).c_str(), std::get<4>(this->data).size());
	username = std::get<0>(data);
	desc = std::get<1>(data);
	rank = std::get<2>(data);
	money = std::get<3>(data);
	img = std::get<4>(data);
	xp = std::get<5>(data);
	points = std::get<6>(data);
	wins = std::get<7>(data);
	losses = std::get<8>(data);

	avatar.loadFromMemory(img.c_str(), img.size());
	this->avatarSprite.setPosition({ 90,200 });
	this->avatarSprite.setSize({ 200,200 });
	this->avatarSprite.setTexture(&avatar);
	
	this->nick.setFont(this->stateData.font);
	this->nick.setString(username);
	this->nick.setCharacterSize(40);
	this->nick.setPosition({ 300,200 });
	this->nick.setStyle(sf::Text::Bold);

	this->description.setFont(this->stateData.font);
	this->description.setString(textWrap(desc, 15));
	this->description.setCharacterSize(20);
	this->description.setPosition({ 300,300 });
	this->description.setStyle(sf::Text::Italic);
	this->stats.setFont(this->stateData.font);
	this->stats.setString(
		"Punkty: " + std::to_string(this->points) +
		"\nWygrane: " + std::to_string(this->wins) +
		"\nPrzegrane: " + std::to_string(this->losses)
	);
	this->stats.setCharacterSize(20);
	this->stats.setPosition({ 90,440 });
	int xpmax[] = {50,200,500,1000,2000,5000,10000,99999};

	this->exp.setFont(this->stateData.font);
	this->exp.setString("lvl " + std::to_string(this->rank) + "\n" + "exp: " + std::to_string(this->xp) + "/" + std::to_string(xpmax[this->rank-1]));
	this->exp.setCharacterSize(20);
	this->exp.setPosition({ 290,440 });

}

ProfileState::~ProfileState()
{
}


void ProfileState::updateSfmlEvents()
{

}



void ProfileState::update(const float& dt)
{
	this->updateMousePosition();
	for (auto& i : this->buttons)
	{
		i.update(this->mousePosWindow);
	}
	if (this->buttons[0].isClicked())
	{
		this->stateData.states.pop();
	}

}

void ProfileState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(&this->panel[0], 4, sf::Quads);
	/*this->stateData.window.draw(this->panelup);
	this->stateData.window.draw(this->paneldown);*/
	this->stateData.window.draw(this->title);
	for (auto& i : this->buttons)
	{
		i.render(this->stateData.window);
	}
	this->stateData.window.draw(this->avatarSprite);
	this->stateData.window.draw(this->nick);
	this->stateData.window.draw(this->exp);
	this->stateData.window.draw(this->description);
	this->stateData.window.draw(this->stats);
}
