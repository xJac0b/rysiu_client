#pragma once
#include "Player.h"
class GameState :
    public State
{
public:
    GameState(StateData& state_data, sf::String&& path);
    ~GameState();
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
private:
    sf::Text title;
    Player player;
    std::vector<std::unique_ptr<Obstacle>>obstacles;
};