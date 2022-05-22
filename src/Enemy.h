#pragma once
#include "Entity.h"
class Enemy :
    public Entity
{
public:
    Enemy(sf::FloatRect&& bounds, sf::FloatRect&& hitbox_bounds, sf::String texture_path, sf::String nick, sf::Font& font);
    ~Enemy();
    void update(const float& dt, const sf::Vector2i mouse_pos);
    void update(const sf::Vector2f pos, float rotation, bool right_direction);
    void render(sf::RenderTarget& target);
private:
    sf::Sprite weaponSprite;
    sf::Texture weaponTexture;
    sf::Texture texture2;
    sf::Text nick;
};

