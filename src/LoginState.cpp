#include "pch.h"
#include "LoginState.h"

LoginState::LoginState(StateData& state_data, sf::String&& path) : State(state_data)
{
	this->bgTexture.loadFromFile(path);
	state.texture = &this->bgTexture;
	gui::setVertexShape(this->background, { 0.f, 0.f,
		static_cast<float>(this->stateData.window.getSize().x),
		static_cast<float>(this->stateData.window.getSize().y) }, 0);
	for (auto& i : this->background)
		i.color = sf::Color(70, 70, 70, 250);


	this->title.setFont(this->stateData.font);
	this->title.setString(L"Logowanie");
	this->title.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 20));
	this->title.setPosition(this->stateData.window.getSize().x / 2.f - this->title.getGlobalBounds().width / 2.f,
		gui::p2pY(10.f, this->stateData.window.getSize()));
	this->title.setOutlineThickness(2.f);
	this->title.setOutlineColor(sf::Color::Black);

	this->errorText.setFont(this->stateData.font);
	this->errorText.setString("");
	this->errorText.setCharacterSize(gui::calcCharSize(this->stateData.window.getSize(), 50));
	this->errorText.setOutlineThickness(2.f);
	this->errorText.setOutlineColor(sf::Color::Black);
	this->errorText.setFillColor(sf::Color::Red);

	this->userInput = std::make_unique<gui::Input>(
		sf::FloatRect(gui::p2pX(30.f, this->stateData.window.getSize()), gui::p2pY(40.f, this->stateData.window.getSize()),
			gui::p2pX(40.f, this->stateData.window.getSize()), gui::p2pY(10.f, this->stateData.window.getSize())),
		sf::Color(100, 100, 100, 190), sf::Color::Black, this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize()), L"U¿ytkownik", 0, 10
		);
	this->passInput = std::make_unique<gui::Input>(
		sf::FloatRect(gui::p2pX(30.f, this->stateData.window.getSize()), gui::p2pY(55.f, this->stateData.window.getSize()),
			gui::p2pX(40.f, this->stateData.window.getSize()), gui::p2pY(10.f, this->stateData.window.getSize())),
		sf::Color(100, 100, 100, 190), sf::Color::Black, this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize()), L"Has³o", 1
		);
	this->submitButton = std::make_unique<gui::Button>(
		sf::FloatRect(40.f, 75.f, 20.f, 8.f),
		"Zaloguj", this->stateData.font, gui::calcCharSize(this->stateData.window.getSize(), 40), this->stateData.window.getSize()
		);
}

LoginState::~LoginState()
{
}

void LoginState::updateSfmlEvents()
{
	if (this->stateData.sfEvent.type == sf::Event::TextEntered)
	{
		if (this->stateData.sfEvent.text.unicode == 9)
		{
			if (this->userInput->getFocus())
			{
				this->userInput->setFocus(0);
				this->passInput->setFocus();
			}
			else if (this->passInput->getFocus())
			{
				this->userInput->setFocus();
				this->passInput->setFocus(0);
			}
			else
				this->userInput->setFocus();
		}
		else if (this->stateData.sfEvent.text.unicode == 13 && this->userInput->getFocus())
		{
			this->userInput->setFocus(0);
			this->passInput->setFocus();
		}
		this->userInput->enter(this->stateData.sfEvent.text.unicode);
		this->passInput->enter(this->stateData.sfEvent.text.unicode);
	}
}

void LoginState::update(const float& dt)
{
	this->updateMousePosition();
	this->userInput->update(this->mousePosWindow, dt);
	this->passInput->update(this->mousePosWindow, dt);
	this->submitButton->update(this->mousePosWindow);
	if (this->submitButton->isClicked())
	{
		if (this->passInput->get().size() > 5 && this->userInput->get().size() > 0)
		{
			this->errorText.setString("");
			std::wcout << this->userInput->get() << " " << this->passInput->get() << '\n';
			short stat = Socket::GetInstance(307)->LogIn(this->userInput->get(), this->passInput->get(), this->stateData.playerId, this->data);
			switch (stat)
			{
			case 0:
				this->errorText.setString(L"SprawdŸ po³¹czenie z internetem!");
				this->errorText.setPosition(this->stateData.window.getSize().x / 2.f - this->errorText.getGlobalBounds().width / 2.f,
					gui::p2pY(90.f, this->stateData.window.getSize()));
				break;
			case 1:
				this->errorText.setString(L"Niepoprawny u¿ytkownik lub has³o!");
				this->errorText.setPosition(this->stateData.window.getSize().x / 2.f - this->errorText.getGlobalBounds().width / 2.f,
					gui::p2pY(90.f, this->stateData.window.getSize()));
				break;
			case 2:
				this->errorText.setString(L"B³¹d serwera! Spróbuj póŸniej!");
				this->errorText.setPosition(this->stateData.window.getSize().x / 2.f - this->errorText.getGlobalBounds().width / 2.f,
					gui::p2pY(90.f, this->stateData.window.getSize()));
				break;
			case 7:
				//SUCCESS
				this->stateData.states.push(std::make_unique<MainMenuState>(this->stateData, "Resources/Images/background.jpg", this->data));
				break;
			default:
				this->errorText.setString("Undefined Error");
				this->errorText.setPosition(this->stateData.window.getSize().x / 2.f - this->errorText.getGlobalBounds().width / 2.f,
					gui::p2pY(90.f, this->stateData.window.getSize()));
				break;
			}
		}
		else if (this->userInput->get().size() == 0)
		{
			this->errorText.setString(L"Minimalna d³ugoœæ loginu to 1 znak");
			this->errorText.setPosition(this->stateData.window.getSize().x / 2.f - this->errorText.getGlobalBounds().width / 2.f,
				gui::p2pY(90.f, this->stateData.window.getSize()));
		}
		else if (this->passInput->get().size() < 6)
		{
			this->errorText.setString(L"Minimalna d³ugoœæ has³a to 6 znaków");
			this->errorText.setPosition(this->stateData.window.getSize().x / 2.f - this->errorText.getGlobalBounds().width / 2.f,
				gui::p2pY(90.f, this->stateData.window.getSize()));
		}

	}
}

void LoginState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, this->state);
	this->stateData.window.draw(this->title);
	this->userInput->render(this->stateData.window);
	this->passInput->render(this->stateData.window);
	this->submitButton->render(this->stateData.window);

	this->stateData.window.draw(this->errorText);
}