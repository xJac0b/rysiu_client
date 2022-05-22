#pragma once
#include "State.h"
#include "MovementComponent.h"
class Entity
{
public:
	Entity(sf::FloatRect& bounds, sf::String texture_path, sf::FloatRect hitboxBounds, int hp_max = 0);
	~Entity();

	const sf::FloatRect getBounds(bool hitboxa = 0);
	const sf::Vector2f& getPosition(bool hitboxa = 0) const;

	void setPosition(sf::Vector2f pos);
	void setHp(int hp);
	virtual void update(const float& dt, const sf::Vector2i mouse_pos) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
protected:
	std::vector<sf::Vertex>sprite;
	sf::Texture texture;
	sf::RenderStates state;
	sf::RectangleShape hitbox;
	sf::Vector2f hitboxOffset;
	int hpMax, hp;
	sf::RectangleShape hpBarBack, hpBar;
};

