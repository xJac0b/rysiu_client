#pragma once
#include "Weapon.h"
class Obstacle :
    public Entity
{
public:
    Obstacle(sf::FloatRect&& bounds, sf::FloatRect textureBounds, sf::FloatRect hitboxBounds, sf::String texture_path);
    ~Obstacle();
    
    void update(const float& dt, const sf::Vector2i mouse_pos);
    void render(sf::RenderTarget& target);
};

