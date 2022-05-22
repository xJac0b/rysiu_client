#pragma once
#include "Player.h"
struct UserData
{
    sf::Vector2f pos;
    int skin, //0 - cat, 1 - dog
        hp,// hp 0-100
        rank;
    float weapon_rotation;
    bool right_direction;
    sf::String nick;
};


class GameState :
    public State
{
public:
    GameState(StateData& state_data, sf::String&& path, std::tuple<sf::Vector2f, int, bool, int, float, sf::String> player,
        std::tuple<sf::Vector2f, int, bool, int, float, sf::String> enemy, sf::Uint32 token,
        sf::Texture t1, sf::Texture t2, sf::String player_nick, sf::String enemy_nick);
    ~GameState();
    void updateSfmlEvents();
    void update(const float& dt);
    void render();
private:
    sf::Text title, stats;
    Player player;
    Enemy enemy;
    sf::Uint32 token;
    std::vector<std::unique_ptr<Obstacle>>obstacles;
    sf::Clock timer;
    UserData playerData, enemyData;
    std::vector<sf::Vector2f> enemyBulletsPos;
    std::vector<Bullet> enemyBullets;
    std::vector<sf::Vector2f>playerBulletsPos;
    bool finish;
    sf::RectangleShape panel;
    gui::Button quit;
    sf::Texture t1, t2;
    sf::RectangleShape s1, s2;
};