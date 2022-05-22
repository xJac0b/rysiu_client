#include "pch.h"
#include "Weapon.h"

Weapon::Weapon(sf::FloatRect bounds, sf::String texture_path1, float bulletVelocity,
	float reload_duration, float shot_duration)
	: reloadTimer(reload_duration, reload_duration), shotTimer(shot_duration, shot_duration), ammunition(30), reloading(false)
{
	this->texture1.loadFromFile(texture_path1);
	this->sprite.setTexture(this->texture1);
	this->bufferShot.loadFromFile("Resources/Sounds/shot.wav");
	this->soundShot.setBuffer(this->bufferShot);
	this->soundShot.setVolume(10.f);
	this->bufferReload.loadFromFile("Resources/Sounds/reload.wav");
	this->soundReload.setBuffer(this->bufferReload);
	this->soundReload.setVolume(10.f);
}

Weapon::~Weapon()
{
}

void Weapon::setDirection(bool right)
{
	if (right)
		this->sprite.setScale({ 1,1 });	
	else
		this->sprite.setScale({ 1,-1 });
}

void Weapon::shot(sf::Vector2i mouse_pos)
{
	this->soundShot.play();
	this->ammunition--;
	sf::Vector2f aimDir = static_cast<sf::Vector2f>(mouse_pos) - this->sprite.getPosition();
	sf::Vector2f aimDirNorm = { aimDir.x / std::sqrt(aimDir.x*aimDir.x + aimDir.y* aimDir.y),
		aimDir.y / std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y) };
	this->shotTimer.reset();
	Bullet b1;
	b1.shape.setPosition(this->sprite.getTransform().transformPoint({92,6}));
	b1.currVelocity = aimDirNorm * b1.maxSpeed;
	this->bullets.push_back(Bullet(b1));
}

void Weapon::update(const float& dt, sf::Vector2i mouse_pos, sf::Vector2f pos)
{
	this->reloadTimer.update(dt);
	if (this->reloading && this->reloadTimer.get())
		this->reloading = false;
	this->shotTimer.update(dt);
	this->sprite.setPosition(pos);
	if (this->sprite.getScale() == sf::Vector2f(1.f, -1.f))
		this->sprite.move(25.f, 0.f);
	else
		this->sprite.move(-25.f, 0.f);
	
	float rotZ = std::atan2(mouse_pos.y - pos.y, mouse_pos.x - pos.x) * (180.0 / 3.141592653589793238463);
	this->sprite.setRotation(rotZ);
	if (this->shotTimer.get() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->reloading)
	{
		if (this->ammunition > 0)
			this->shot(mouse_pos);
		else
			this->reload();
		this->shotTimer.reset();
	}
		

	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i].shape.move(bullets[i].currVelocity*dt);
		
		if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > 800.f
			|| bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > 600.f
			)
		{
			bullets.erase(bullets.begin() + i);		
		}
	}

}

void Weapon::render(sf::RenderTarget& target)
{
	for (auto& i : this->bullets)
		target.draw(i.shape);
	target.draw(this->sprite);
}

void Weapon::correctBullets(std::vector<sf::Vector2f> to_stay)
{
	int s = this->bullets.size();
	if (s == to_stay.size())
	{
		for (int i = 0; i < s; i++)
		{
			this->bullets[i].setPos(to_stay[i]);
		}
	}
}

void Weapon::reload()
{
	this->reloading = true;
	this->reloadTimer.reset();
	this->soundReload.play();
	this->ammunition = 30;
}

const std::vector<sf::Vector2f> Weapon::getBullets()
{
	std::vector<sf::Vector2f>temp;
	for (auto& i : this->bullets)
		temp.push_back(i.getPos());
	return temp;
}

const float& Weapon::getRotation()
{
	return this->sprite.getRotation();
}

