#pragma once
#include "GameState.h"
class MainMenuState :
    public State
{
public:
    MainMenuState(StateData& state_data, sf::String&& path, std::tuple < sf::String, sf::String, sf::String, int, int, int>& data);
    ~MainMenuState(); 
    void update(const float& dt);
    void render();
private:
    sf::Text title, welcome;
    std::vector<sf::Vertex>panel;
    std::vector<gui::Button>buttons;
    std::unique_ptr<gui::ButtonImageList> skins;
};