#include "pch.h"
#include "LeaderBoardState.h"

LeaderBoardState::LeaderBoardState(StateData& state_data, sf::String&& path,
	std::vector<
	std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>> data) : State(state_data), panel(4), data(data),
	quit(gui::Button(
		sf::FloatRect(72.f, 80.f, 14.f, 8.f),
		L"WyjdŸ",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()))
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
	this->title.setString("Top 5");
	this->title.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 20));
	this->title.setPosition(this->stateData.window.getSize().x / 2.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->title.setOutlineThickness(2.f);
	this->title.setOutlineColor(sf::Color::Black);


	this->paneldown.setPosition(50.f, 465.f);
	this->paneldown.setSize({ 700.f, 70.f });
	this->paneldown.setFillColor(sf::Color(20, 20, 20, 220));
	//this->avatar.loadFromMemory(std::get<4>(this->data).c_str(), std::get<4>(this->data).size());

	sf::RectangleShape shape;
	shape.setPosition( { 70, 185});
	shape.setSize({560,45});
	shape.setFillColor(sf::Color(10, 10, 10, 230));
	
	int size = data.size();
	for (int i = 0; i<size; i++)
	{
		sf::Text number, nick, points;
		sf::RectangleShape rect, avatarRect;
		sf::Texture avatar;
		rect = shape;

		number.setFont(this->stateData.font);
		number.setCharacterSize(30);
		number.setString(std::to_string(i+1));
		number.setStyle(sf::Text::Bold);
		number.setPosition(rect.getPosition().x + 25 - number.getGlobalBounds().width/2.f, rect.getPosition().y + rect.getSize().y / 2.f - number.getGlobalBounds().height);

		nick.setFont(this->stateData.font);
		nick.setCharacterSize(27);
		nick.setString(std::get<0>(data[i]));
		nick.setPosition(rect.getPosition().x + 110,
			rect.getPosition().y + rect.getSize().y / 2.f - nick.getGlobalBounds().height/1.5f);
		
		

		
		avatarRect.setPosition(rect.getPosition().x +50, rect.getPosition().y);
		avatarRect.setSize({45, rect.getSize().y });


		points.setFont(this->stateData.font);
		points.setCharacterSize(27);
		points.setString("punkty: " + std::to_string(std::get<6>(data[i])));
		points.setPosition(rect.getPosition().x + rect.getSize().x - points.getGlobalBounds().width - 5,
			rect.getPosition().y + rect.getSize().y / 2.f - points.getGlobalBounds().height/1.5f);



		gui::Button profileButton = (gui::Button(
			sf::FloatRect(((rect.getPosition().x+rect.getSize().x+5.f)/800.f)*100,
				(rect.getPosition().y/ 600.f) * 100,
				14.f, 8.f),
			L"Poka¿",
			this->stateData.font,
			gui::calcCharSize(this->stateData.window.getSize(), 50),
			this->stateData.window.getSize()));

		this->leaders.push_back(std::make_unique<Leader>(rect,number,nick, points, avatar,avatarRect,profileButton));
		this->leaders.back()->avatar.loadFromMemory(std::get<4>(data[i]).c_str(), std::get<4>(data[i]).size());
		this->leaders.back()->avatarRect.setTexture(&this->leaders.back()->avatar);
		shape.move({ 0,50 });
	}
}

LeaderBoardState::~LeaderBoardState()
{
}


void LeaderBoardState::updateSfmlEvents()
{
}



void LeaderBoardState::update(const float& dt)
{
	this->updateMousePosition();
	this->quit.update(this->mousePosWindow);
	if (this->quit.isClicked())
	{
		this->stateData.states.pop();
	}
	for (int i = 0; i<this->leaders.size(); i++)
	{
		this->leaders[i]->profileButton.update(this->mousePosWindow);
		if (this->leaders[i]->profileButton.isClicked())
			this->stateData.states.push(std::make_unique<ProfileState>(this->stateData, "Resources/Images/background.jpg", this->data[i]));
	}

}

void LeaderBoardState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(&this->panel[0], 4, sf::Quads);
	for (auto& i : this->leaders)
	{
		this->stateData.window.draw(i->rect);
		this->stateData.window.draw(i->avatarRect);
		this->stateData.window.draw(i->number);
		this->stateData.window.draw(i->nick);
		this->stateData.window.draw(i->points);
		i->profileButton.render(this->stateData.window);
	}
	this->stateData.window.draw(this->paneldown);
	this->stateData.window.draw(this->title);
	this->quit.render(this->stateData.window);
}
