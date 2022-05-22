#include "pch.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateData& state_data, sf::String&& path,
	std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int> data) : State(state_data),panel(4), data(data)
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
	std::wcout << std::endl << std::get<0>(data).toWideString() << std::endl;
	this->welcome.setString("Witaj " + std::get<0>(data) +"!");
	this->welcome.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 40));
	this->welcome.setPosition(400 - this->welcome.getGlobalBounds().width/2, gui::p2pY(55.f, this->stateData.window.getSize()));
	this->welcome.setOutlineThickness(2.f);
	this->welcome.setOutlineColor(sf::Color::Black);
	//buttons
	this->buttons.push_back(gui::Button(
		sf::FloatRect(10.f, 33.33f, 25.0f, 10.f),
		L"Utwórz grê",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 40),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(76.f, 33.33f, 15.5f, 10.f),
		L"Do³¹cz",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 40),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(15.f, 80.f, 14.f, 8.f),
		"Profil",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(37.5f, 80.f, 18.f, 8.f),
		"Ranking",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(72.f, 80.f, 14.f, 8.f),
		L"WyjdŸ",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(),50),
		this->stateData.window.getSize()));

	this->gameTokenInput = std::make_unique<gui::Input>(
		sf::FloatRect(gui::p2pX(52.f, this->stateData.window.getSize()), gui::p2pY(35.f, this->stateData.window.getSize()),
			gui::p2pX(20.f, this->stateData.window.getSize()), gui::p2pY(7.f, this->stateData.window.getSize())),
		sf::Color(150, 150, 150, 190), sf::Color::White, this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize()), "token gry", false, 6,"",true
		);
	this->panelup.setPosition(50.f, 185.f);
	this->panelup.setSize({ 700.f, 70.f });
	this->panelup.setFillColor(sf::Color(20, 20, 20, 220));
	this->paneldown.setPosition(50.f, 465.f);
	this->paneldown.setSize({ 700.f, 70.f });
	this->paneldown.setFillColor(sf::Color(20, 20, 20, 220));
	//this->avatar.loadFromMemory(std::get<4>(this->data).c_str(), std::get<4>(this->data).size());
}

MainMenuState::~MainMenuState()
{
}


void MainMenuState::updateSfmlEvents()
{
	if (this->stateData.sfEvent.type == sf::Event::TextEntered)
		this->gameTokenInput->enter(this->stateData.sfEvent.text.unicode);
}



void MainMenuState::update(const float& dt)
{
	this->updateMousePosition();
	this->gameTokenInput->update(this->mousePosWindow, dt);
	for (auto& i : this->buttons)
	{
		i.update(this->mousePosWindow);
	}

	if (this->buttons[0].isClicked())
	{
		sf::Uint32 game_token = 0;
		if (!Socket::GetInstance(307)->HostGame(game_token, std::get<0>(data), std::get<2>(data), std::get<4>(this->data),
			this->stateData.playerId))
			std::cout << "problem" << std::endl;
		else
		{
			std::cout << "SUCCES HOST TOKEN:" << game_token << std::endl;
			this->stateData.states.push(std::make_unique<LobbyState>(this->stateData, "Resources/Images/background.jpg", game_token,
				std::get<4>(this->data), std::get<0>(data), sf::Vector2i(std::get<2>(data), 0)));
		}
	}
	else if (this->buttons[1].isClicked())
	{
		sf::String token = this->gameTokenInput->get(), hostName;
		if (token.getSize() == 6)
		{
			sf::Vector2i ranks;
			ranks.y = std::get<2>(data);
			std::cout << "joining";
			if (!Socket::GetInstance(307)->JoinGame(std::stoi(token.toAnsiString()), std::get<0>(data), hostName,
				ranks, std::get<4>(this->data), this->hostavatar, this->stateData.playerId))
				std::cout << "problem joining" << std::endl;
			else
			{
				std::cout << "SUCCES JOIN TOKEN: " << std::string(token) << std::endl;
				this->stateData.states.push(std::make_unique<LobbyState>(this->stateData, "Resources/Images/background.jpg", 
					std::stoi(token.toAnsiString()), std::get<4>(this->data), std::get<0>(data), ranks, hostName, this->hostavatar));
			}
		}
	}
	else if (this->buttons[2].isClicked())
	{
		if (!Socket::GetInstance(307)->getUserDataById(this->stateData.playerId, this->data))
		{
			std::cout << "[error] get user data by id [error]";
		}
		else
			this->stateData.states.push(std::make_unique<ProfileState>(this->stateData, "Resources/Images/background.jpg", this->data));
	}
	
	else if (this->buttons[3].isClicked())
	{
		std::vector<
			std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>> dataLeaders;
		if (!Socket::GetInstance(307)->getLeaderBoard(dataLeaders))
		{
			std::cout << "[error] get leaderboard [error]";
		}
		else
			this->stateData.states.push(std::make_unique<LeaderBoardState>(this->stateData, "Resources/Images/background.jpg", dataLeaders));
	}
	else if (this->buttons[4].isClicked())
	{
		this->stateData.states.pop();
	}

}

void MainMenuState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(&this->panel[0], 4, sf::Quads);
	this->stateData.window.draw(this->panelup);
	this->stateData.window.draw(this->paneldown);
	this->stateData.window.draw(this->title);
	for (auto& i : this->buttons)
	{
		i.render(this->stateData.window);
	}
	this->stateData.window.draw(this->welcome);
	this->gameTokenInput->render(this->stateData.window);
}
