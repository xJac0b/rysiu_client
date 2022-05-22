#include "pch.h"
#include "LobbyState.h"

LobbyState::LobbyState(StateData& state_data, sf::String&& path, int game_token,
	std::string avatar_texture, sf::String nick, sf::Vector2i rank, sf::String host_name, 
	std::string host_avatar) : State(state_data), panel(4),
	token(game_token), username(nick), rank(rank), guestReady(false)
{
	this->threadd = std::make_unique<std::thread>(&LobbyState::listen, this);
	if (host_name == "")
		this->host = true;
	else 
		this->host = false;
	
	if (this->host)
		t1.loadFromMemory(avatar_texture.c_str(), avatar_texture.size());
	else
	{
		t1.loadFromMemory(host_avatar.c_str(), host_avatar.size());
		t2.loadFromMemory(avatar_texture.c_str(), avatar_texture.size());
	}	
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
	std::cout << game_token;
	this->title.setString("GAME TOKEN: "+ std::to_string(game_token));
	this->title.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 30));
	this->title.setPosition(this->stateData.window.getSize().x / 2.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->title.setOutlineThickness(2.f);
	this->title.setOutlineColor(sf::Color::Black);

	//init welcome text
	this->subtitle.setFont(this->stateData.font);
	this->subtitle.setString(L"Udostępnij znajomemu powyższy kod, by mógł dołączyć do poczekalni!");
	this->subtitle.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 80));
	this->subtitle.setPosition(this->stateData.window.getSize().x / 2.f - this->subtitle.getGlobalBounds().width / 2.f,
		gui::p2pY(21.5f, this->stateData.window.getSize()));
	this->subtitle.setOutlineThickness(2.f);
	this->subtitle.setOutlineColor(sf::Color::Black);
	//buttons
	this->buttons.push_back(gui::Button(
		sf::FloatRect(14.f, 79.f, this->host ? 14.f : 23.5f , 10.f),
		this->host ? "Start" : "Niegotowy",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 45),
		this->stateData.window.getSize()));
	this->buttons.push_back(gui::Button(
		sf::FloatRect(72.f, 80.f, 14.f, 10.f),
		L"Wyjdź",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 50),
		this->stateData.window.getSize()));
	std::vector<std::pair<sf::String, bool>>temp{ {"Resources/Images/Skins/skin0.png", true}, {"Resources/Images/Skins/skin1.png", 
		true} };
	this->skins = std::make_unique<gui::ButtonImageList>(temp,
		sf::FloatRect(62.5f, 33.33f, 25.f, 33.33f), sf::FloatRect(0.f, 0.f, 200.f, 200.f),
		this->stateData.font,
		100, this->stateData.window.getSize());

	sf::RectangleShape r;
	r.setSize({ 300, 60 });
	r.setPosition({ 100,230 });
	r.setFillColor(sf::Color(100,100,100,100));

	sf::RectangleShape a;
	a.setTexture(&t1);
	a.setPosition(r.getPosition());
	a.setSize({ 70, r.getSize().y });
	sf::Text t;
	t.setCharacterSize(20);
	t.setFont(this->stateData.font);
	if(this->host)
		t.setString(nick + " [lvl "+ std::to_string(rank.x) + "]");
	else
		t.setString(host_name + " [lvl " + std::to_string(rank.x) + "]");
	t.setPosition(r.getPosition().x + 80, r.getPosition().y + r.getSize().y/2.f - t.getGlobalBounds().height/2);
	
	this->users.push_back(std::make_tuple(r,a,t));
	
	r.setPosition({ 100,300 });
	sf::RectangleShape a1;
	if(!this->host)
	a1.setTexture(&t2);
	a1.setSize({ 70, r.getSize().y });
	a1.setPosition(r.getPosition());
	if (this->host)
		t.setString("");
	else
		t.setString(username + " [lvl " + std::to_string(rank.y) + "]");
	t.setPosition(r.getPosition().x + 80, r.getPosition().y + r.getSize().y / 2.f - t.getGlobalBounds().height / 2);
	this->users.push_back(std::make_tuple(r, a1, t));

	guestReadyText.setFont(this->stateData.font);
	guestReadyText.setCharacterSize(20);
	guestReadyText.setString("R");
	guestReadyText.setPosition(r.getPosition().x + 250, r.getPosition().y + r.getSize().y / 2.f - guestReadyText.getGlobalBounds().height / 2);
	
	
	this->paneldown.setPosition(50.f, 465.f);
	this->paneldown.setSize({ 700.f, 70.f });
	this->paneldown.setFillColor(sf::Color(20, 20, 20, 220));
}

LobbyState::~LobbyState()
{
}
void LobbyState::updateSfmlEvents()
{
	if (this->stateData.sfEvent.type == sf::Event::Closed)
	{
		this->terminateThread = true;
		this->threadd->join();
		this->stateData.window.close();
	}
		
}


void LobbyState::update(const float& dt)
{
	if (this->terminateThread)
	{
		this->stateData.states.pop();
		this->comtype = 0;
	}
	else
	{
		this->updateMousePosition();
		this->skins->update(this->mousePosWindow);
		if (this->skins->isClicked() && this->skins->activeAvailable())
			Socket::GetInstance(307)->SetSkin(this->token, this->skins->getActive());
		
		for (auto& i : this->buttons)
		{
			i.update(this->mousePosWindow);
		}

		if (this->buttons[0].isClicked())
		{
			if (this->host)
			{
				Socket::GetInstance(307)->RunGame(this->token);
			}
			else
			{
				Socket::GetInstance(307)->ToggleReady(this->token);
			}

		}
		else if (this->buttons[1].isClicked())
		{
			Socket::GetInstance(307)->LeaveGame(this->token);
			this->terminateThread = true;
			this->threadd->join();
			this->stateData.states.pop();
		}
		switch (comtype)
		{
		case 1:
			this->terminateThread = true;
			this->threadd->join();
			this->stateData.states.pop();
			break;
		case 2:
		{
			std::get<2>(this->users[1]).setString(this->username2 + (this->username2 == "" ? "" : " [lvl " + std::to_string(rank.y) + "]"));
			std::get<2>(this->users[1]).setPosition(std::get<0>(this->users[1]).getPosition().x + 80,
				std::get<0>(this->users[1]).getPosition().y + std::get<0>(this->users[1]).getSize().y / 2.f
				- std::get<2>(this->users[1]).getGlobalBounds().height / 2);
			t2.loadFromMemory(this->secondTexture.c_str(), this->secondTexture.size());
			std::get<1>(this->users[1]).setTexture(&t2);
			this->guestReady = false;
			this->comtype = 0;
		}
			break;
		case 3:
			if (!this->host)
			{
				if (this->guestReady)
					this->buttons[0].setText("Gotowy");
				else
					this->buttons[0].setText("Niegotowy");
			}
			break;
		case 4:
			this->comtype = 0;
			this->terminateThread = true;
			this->threadd->join();
			std::get<5>(this->dataHost) = std::get<2>(this->users[0]).getString();
			std::get<5>(this->dataGuest) = std::get<2>(this->users[1]).getString();
			if (this->host)
			{	
				this->stateData.states.push(std::make_unique<GameState>(this->stateData, "Resources/Images/background.jpg",
					this->dataHost, this->dataGuest, this->token, this->t1, this->t2, 
					std::get<2>(this->users[0]).getString().toWideString().substr(
						0, std::get<2>(this->users[0]).getString().toWideString().rfind('[')),
						std::get<2>(this->users[1]).getString().toWideString().substr(
							0, std::get<2>(this->users[1]).getString().toWideString().rfind('['))));
			}
			else
			{
				this->stateData.states.push(std::make_unique<GameState>(this->stateData, "Resources/Images/background.jpg",
					this->dataGuest, this->dataHost, this->token, this->t2, this->t1,
					std::get<2>(this->users[1]).getString().toWideString().substr(
						0, std::get<2>(this->users[1]).getString().toWideString().rfind('[')),
					std::get<2>(this->users[0]).getString().toWideString().substr(
						0, std::get<2>(this->users[0]).getString().toWideString().rfind('['))));
			}
			break;
		}
	}
}

void LobbyState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	this->stateData.window.draw(&this->panel[0], 4, sf::Quads);
	this->stateData.window.draw(this->paneldown);
	this->stateData.window.draw(this->title);
	this->stateData.window.draw(this->subtitle);

	for (auto& i : this->users)
	{
		this->stateData.window.draw(std::get<0>(i));
		if (std::get<2>(i).getString() != "")	
			this->stateData.window.draw(std::get<1>(i));
			
		this->stateData.window.draw(std::get<2>(i));
	}
	if (this->guestReady)
		this->stateData.window.draw(this->guestReadyText);

	this->skins->render(this->stateData.window);
	for (auto& i : this->buttons)
	{
		i.render(this->stateData.window);
	}
	
}

void LobbyState::listen()
{
	Socket::GetInstance(307)->Listen(this->comtype, this->username2, this->rank, this->terminateThread, this->guestReady,
		this->dataHost, this->dataGuest, this->secondTexture);
}
