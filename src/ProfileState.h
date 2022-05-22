#pragma once
#include "State.h"
class ProfileState :
    public State
{
public:
    ProfileState(StateData& state_data, sf::String&& path,
        std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int> data = { "Anonim", "",-1,-1, "",0,0,0,0 });
    ~ProfileState();
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
private:
    sf::Text title, stats, nick, description, exp;
    std::vector<sf::Vertex>panel;
    //sf::RectangleShape panelup, paneldown;
    std::vector<gui::Button>buttons;
    sf::Texture avatar;
    sf::RectangleShape avatarSprite;
    sf::String username, desc;
    int rank, money, xp, points, wins, losses;
    std::string img;
};