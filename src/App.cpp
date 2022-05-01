#include "pch.h"
#include "App.h"
App::App()
{
	//Init window
    this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800,600), "Rysiu", sf::Style::Titlebar | sf::Style::Close);
    //Init font
    this->font.loadFromFile("Fonts/font1.ttf");
    //Init State Data
    this->stateData = std::make_unique<StateData>(*this->window, this->states, this->event, this->font);
    //Push state  
    //std::vector<std::pair<sf::String, bool>>temp{ {"Resources/Images/Skins/abc.png", true}, {"Resources/Images/Skins/abcd.png", false} };
    //this->states.push(std::make_unique<MainMenuState>(*this->stateData,"Resources/Images/background.jpg", ));
    //this->states.push(std::make_unique<LoginState>(*this->stateData, "Resources/Images/background.jpg"));
    this->states.push(std::make_unique<GameState>(*this->stateData, "Resources/Images/background.jpg"));
}

App::~App()
{
}

void App::updateDt()
{
    this->dt = this->dtClock.restart().asSeconds(); //Delta time
    //std::cout << "FPS: " << 1.0f / dt << "\n";
    //system("cls");
}

void App::updateSfmlEvents()
{
    while (window->pollEvent(this->event))
    {
        if(!this->states.empty())
            this->states.top()->updateSfmlEvents();
        if (event.type == sf::Event::Closed)
            window->close();
        
    }
}

void App::update()
{
    this->updateDt();
    this->updateSfmlEvents();
    
    if (!this->states.empty())
    {
        if (this->window->hasFocus())
        {
            this->states.top()->update(this->dt); // update things in state
        }
    }
    else
        this->window->close();
}

void App::render()
{
        window->clear();

        if (!this->states.empty())
            this->states.top()->render(); // render things in state

        window->display();
}

void App::run()
{
    while (this->window->isOpen())
    {
        this->update();
        this->render();
    }
}
