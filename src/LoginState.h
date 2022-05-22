#pragma once
#include "MainMenuState.h"
class LoginState :
    public State
{
public:
    LoginState(StateData& state_data, sf::String&& path);
    ~LoginState();
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
private:
    sf::Text title, errorText;
    std::tuple < sf::String, sf::String, int, int, std::string, int, int,int, int>data;
    std::unique_ptr<gui::Input> userInput;
    std::unique_ptr<gui::Input> passInput;
    std::unique_ptr<gui::Button> submitButton;
};

