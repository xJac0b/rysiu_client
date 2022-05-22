#pragma once
#include "State.h";
#include "ProfileState.h";
struct Leader
{
    Leader(sf::RectangleShape rect, sf::Text number, sf::Text nick, sf::Text points,
        sf::Texture avatar, sf::RectangleShape avatarRect, gui::Button profileButton)
        : rect(rect), number(number), nick(nick), avatar(avatar), avatarRect(avatarRect), profileButton(profileButton), points(points)
    {
    }
    sf::RectangleShape rect;
    sf::Text number, nick, points;
    sf::Texture avatar;
    sf::RectangleShape avatarRect;
    gui::Button profileButton;
};

class LeaderBoardState :
    public State
{
public:
    LeaderBoardState(StateData& state_data, sf::String&& path,
        std::vector<
        std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>> data);
    ~LeaderBoardState();
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
private:
    sf::Text title;
    std::vector<sf::Vertex>panel;
    std::vector<std::unique_ptr<Leader>>leaders;
    sf::RectangleShape paneldown;
    gui::Button quit;
    std::vector<
        std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>> data;
};