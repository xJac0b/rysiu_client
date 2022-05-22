#pragma once
#include "GameState.h"
class LobbyState :
    public State
{
public:
    LobbyState(StateData& state_data, sf::String&& path, int game_token,
        std::string avatar_texture, sf::String nick, sf::Vector2i rank, sf::String host_name = "", std::string host_avatar="");
    ~LobbyState();
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
    void listen();
private:
    sf::Text title, subtitle, guestReadyText;
    std::vector<sf::Vertex>panel;
    std::vector<gui::Button>buttons;
    std::unique_ptr<gui::ButtonImageList> skins;
    sf::Texture t1, t2;
    sf::RectangleShape paneldown;
    std::vector<std::tuple<sf::RectangleShape, sf::RectangleShape, sf::Text>>users;
    std::string secondTexture;
    sf::Uint32 token, comtype;
    sf::String username, username2;
    sf::Vector2i rank;
    std::unique_ptr<std::thread> threadd;
    bool terminateThread = false, host, guestReady;
    std::tuple<sf::Vector2f, int, bool, int, float, sf::String> dataHost, dataGuest;
};