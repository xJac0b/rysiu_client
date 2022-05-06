#pragma once
#include "Player.h"
class GameState :
    public State
{
public:
    GameState(StateData& state_data, sf::String&& path);
    ~GameState();
    void set_dFPS(const float& fps);
    void set_dPOS(const sf::FloatRect& pos);
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
    
private:
    sf::Text title,d_fps,d_pos;
    Player player;
    std::vector<std::unique_ptr<Obstacle>>obstacles;
    sf::Clock dtClock;
    float dt,fps;

};