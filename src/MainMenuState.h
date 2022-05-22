#pragma once
#include "LobbyState.h"
#include "LeaderBoardState.h"
class MainMenuState :
    public State
{
public:
    MainMenuState(StateData& state_data, sf::String&& path,
        std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int> data = { "Anonim", "",-1,-1, "", 0, 0, 0, 0});
    ~MainMenuState(); 
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
private:
    sf::Text title, welcome;
    std::vector<sf::Vertex>panel;
    sf::RectangleShape panelup, paneldown;
    std::vector<gui::Button>buttons;
    std::unique_ptr<gui::ButtonImageList> skins;
    std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int> data;
    sf::Texture avatar;
    std::unique_ptr<gui::Input> gameTokenInput;
    std::string hostavatar;
};