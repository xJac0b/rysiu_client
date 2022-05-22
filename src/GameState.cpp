#include "pch.h"
#include "GameState.h"

GameState::GameState(StateData& state_data, sf::String&& path, std::tuple<sf::Vector2f, int, bool, int, float, sf::String> player,
	std::tuple<sf::Vector2f, int, bool, int, float, sf::String> enemy, sf::Uint32 token,
	sf::Texture t1, sf::Texture t2, sf::String player_nick, sf::String enemy_nick) : State(state_data), token(token), finish(false), t1(t1), t2(t2),
player(sf::FloatRect(std::get<0>(player).x, std::get<0>(player).y, 100, 100),
	sf::FloatRect(8, 99, 85, 1), sf::String("Resources/Images/Skins/skin" + std::to_string(std::get<3>(player)) + ".png"), {510.f,30.f},
	800, 580, player_nick, this->stateData.font), 
	enemy(sf::FloatRect(std::get<0>(enemy).x, std::get<0>(enemy).y, 100, 100),
		sf::FloatRect(8, 99, 85, 1), sf::String("Resources/Images/Skins/skin" + std::to_string(std::get<3>(enemy)) + ".png"),
		enemy_nick, this->stateData.font),
	quit(gui::Button(
		sf::FloatRect(40.f, 75.f, 14.5f, 7.5f),
		"QUIT",
		this->stateData.font,
		gui::calcCharSize(this->stateData.window.getSize(), 40),
		this->stateData.window.getSize()))
{
	//Init backgorund
	this->bgTexture.loadFromFile(path);
	state.texture = &this->bgTexture;
	gui::setVertexShape(this->background, { 0.f, 0.f,
		static_cast<float>(this->stateData.window.getSize().x),
		static_cast<float>(this->stateData.window.getSize().y) }, 0);
	for (auto& i : this->background)
		i.color = sf::Color(70, 70, 70, 250);
	this->obstacles.push_back(
	std::make_unique<Obstacle>(sf::FloatRect(335, 100, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(335, 200, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(205, 200, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(465, 200, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));

	
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(335, 300, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(335, 400, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(335, 500, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(0, 100, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(670, 100, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(0, 300, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(670, 300, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(102.5, 450, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));
	this->obstacles.push_back(
		std::make_unique<Obstacle>(sf::FloatRect(567.5, 450, 130, 30), sf::FloatRect(0, 0, 160, 40), sf::FloatRect(0, 0, 130, 1), "Resources/Images/platform.png"));

	
	this->stateData.window.setFramerateLimit(60);
	
	this->playerData.nick = std::get<5>(player);
	this->playerData.pos = { std::get<0>(player).x, std::get<0>(player).y };
	this->playerData.hp = std::get<1>(player);
	this->playerData.right_direction = std::get<2>(player);
	this->playerData.skin = std::get<3>(player);
	this->playerData.weapon_rotation = std::get<4>(player);

	this->enemyData.nick = std::get<5>(enemy);
	this->enemyData.pos = { std::get<0>(enemy).x, std::get<0>(enemy).y };
	this->enemyData.hp = std::get<1>(enemy);
	this->enemyData.right_direction = std::get<2>(enemy);
	this->enemyData.skin = std::get<3>(enemy);
	this->enemyData.weapon_rotation = std::get<4>(enemy);

	this->panel.setFillColor(sf::Color(30, 30, 30, 200));
	this->panel.setSize({ 700,400 });
	this->panel.setPosition({ 50,100 });

	this->title.setFont(this->stateData.font);
	this->title.setCharacterSize(50);
	this->stats.setFont(this->stateData.font);
	this->stats.setCharacterSize(25);

	this->s1.setTexture(&this->t1);
	this->s1.setSize({ 50,50 });
	this->s2.setTexture(&this->t2);
	this->s2.setSize({ 50,50 });
}

GameState::~GameState()
{
}

void GameState::updateSfmlEvents()
{
}


void GameState::update(const float& dt)
{
	this->updateMousePosition();
	if (this->finish)
	{
		this->quit.update(this->mousePosWindow);
		if(this->quit.isClicked())
			this->stateData.states.pop();
	}
	else
	{
		this->playerData.pos.x = this->player.getPosition().x;
		this->playerData.pos.y = this->player.getPosition().y;
		this->playerData.right_direction = this->player.rightDir();
		this->playerData.weapon_rotation = this->player.getWeaponRotation();
		Socket::GetInstance(307)->SendGameData(
			this->token,
			this->playerData.pos.x,
			this->playerData.pos.y,
			this->playerData.hp,
			this->playerData.right_direction,
			this->playerData.weapon_rotation,
			this->enemyData.pos.x,
			this->enemyData.pos.y,
			this->enemyData.hp,
			this->enemyData.right_direction,
			this->enemyData.weapon_rotation,
			this->player.getBullets(),
			this->enemyBulletsPos,
			this->playerBulletsPos,
			this->finish
		);
		if (this->finish)
		{
			if (this->playerData.hp == -10 && this->enemyData.hp == -10)
			{
				this->title.setString(L"Zerwano po³¹czenie!");
				this->title.setPosition({ 400 - this->title.getGlobalBounds().width / 2.f, 150 });
			}
			else
			{
				this->title.setString("Wygrywa " +(this->playerData.hp <= 0 ? this->enemyData.nick : this->playerData.nick));
				this->stats.setString(
					(this->playerData.hp <= 0 ? (this->enemyData.nick + L" " + std::to_wstring(this->enemyData.hp) + L"/100hp\n\n\n")
						: (this->playerData.nick + L" " + std::to_wstring(this->playerData.hp) + L"/100hp\n\n\n"))
					+
					(this->playerData.hp > 0 ? (this->enemyData.nick + L" " + std::to_wstring(this->enemyData.hp) + L"/100hp\n")
						: (this->playerData.nick + L" " + std::to_wstring(this->playerData.hp) + L"/100hp\n"))
				);
				this->title.setPosition({ 400 - this->title.getGlobalBounds().width / 2.f, 150 });
				this->stats.setPosition({ 400 - this->stats.getGlobalBounds().width / 2.f, 300 });
				if (this->playerData.hp > 0)
				{
					this->s1.setPosition({ this->stats.getPosition().x - 70, this->stats.getPosition().y - 10 });
					this->s2.setPosition({ this->s1.getPosition().x, this->stats.getPosition().y + 100 });

				}		
				else
				{
					this->s2.setPosition({ this->stats.getPosition().x - 70, this->stats.getPosition().y - 10});
					this->s1.setPosition({ this->s2.getPosition().x, this->stats.getPosition().y + 100 });
				}	
				
			}
		}
		this->player.correctBullets(this->playerBulletsPos);
		sf::FloatRect pbnd1 = this->player.getBounds(true);
		sf::FloatRect pbndd1 = this->player.getBounds();
		this->player.update(dt, this->mousePosWindow);
		this->enemy.update(this->enemyData.pos, this->enemyData.weapon_rotation, this->enemyData.right_direction);
		this->player.setHp(this->playerData.hp);
		this->enemy.setHp(this->enemyData.hp);
		this->enemyBullets.clear();
		for (auto& i : this->enemyBulletsPos)
			this->enemyBullets.push_back(Bullet(4.f, i));
		bool collide = false;
		for (auto& i : this->obstacles)
		{
			sf::FloatRect pbnd2 = this->player.getBounds(true), obnd = i->getBounds(true);
			if (obnd.top > pbnd1.top && obnd.top < pbnd2.top)
			{
				float minx = std::min(pbnd1.left, pbnd2.left),
					maxx = std::max(pbnd1.left + pbnd1.width, pbnd2.left + pbnd2.width),
					miny = std::min(pbnd1.top, pbnd2.top),
					maxy = std::max(pbnd1.top + pbnd1.height, pbnd2.top + pbnd2.height);
				if (sf::FloatRect(minx, miny, maxx - minx, maxy - miny).intersects(obnd))
				{
					this->player.setPosition({ pbndd1.left, obnd.top - this->player.getBounds().height });
					collide = true;
					break;
				}

			}
			if (pbnd2.intersects(obnd) && this->player.falling())
				collide = true;
		}
		this->player.onPlatform(collide);
	}
}

void GameState::render()
{
	this->stateData.window.draw(&this->background[0], 4, sf::Quads, state);
	for (auto& i : this->obstacles)
		i->render(this->stateData.window);
    this->player.render(this->stateData.window);
	this->enemy.render(this->stateData.window);
	for (auto& i : this->enemyBullets)
		this->stateData.window.draw(i.shape);
	if (this->finish)
	{
		this->stateData.window.draw(this->panel);
		this->stateData.window.draw(this->title);
		this->stateData.window.draw(this->stats);
		if (this->title.getString() != L"Zerwano po³¹czenie!")
		{
			this->stateData.window.draw(this->s1);
			this->stateData.window.draw(this->s2);
		}		
		this->quit.render(this->stateData.window);
	}
		
}