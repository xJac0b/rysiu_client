#pragma once
#include "Timer.h"
enum class BTN_STATES { IDLE, HOVER, ACTIVE };
namespace gui
{
	float p2pX(float perc, const sf::Vector2u& v);
	float p2pY(float perc, const sf::Vector2u& v);
	const unsigned calcCharSize(const sf::Vector2u& v, const unsigned modifier = 60);
	void setVertexShape(std::vector<sf::Vertex>& varray, sf::FloatRect bounds, bool pos = 1);
	sf::FloatRect getVertexShape(std::vector<sf::Vertex>& varray);
	
	class Button
	{
	public:
		//Ctor,dtor
		Button(sf::FloatRect bounds, sf::String text, sf::Font& font, unsigned charSize,
			sf::Vector2u w_size,
			std::vector<sf::Color> colors = { sf::Color::Yellow, sf::Color::White, sf::Color::Black,
			sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
			sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent });
		~Button();
		//Accessors
		const bool&& isClicked();
		const sf::String& getText() const;
		//Functions
		void setText(sf::String str);
		void setColor(sf::Color color, sf::Color out_color);
		void update(sf::Vector2i& mousePosWin);
		void render(sf::RenderTarget& target);
	protected:
		bool clicked;

		sf::SoundBuffer buffer;
		sf::Sound sound;
		std::vector<sf::Vertex>shape;
		std::vector<sf::Vertex>outline;
		sf::Text text;
		sf::FloatRect bounds;
		BTN_STATES state;

		sf::Color textIdle;
		sf::Color textHover;
		sf::Color textActive;
		sf::Color idle;
		sf::Color hover;
		sf::Color active;
		sf::Color outIdle;
		sf::Color outHover;
		sf::Color outActive;
	};
	class ButtonImage : public Button {
	public:
		ButtonImage(sf::String path, sf::FloatRect bounds, sf::FloatRect tbounds, sf::String text, sf::Font& font, unsigned charSize,
			sf::Vector2u w_size,
			std::vector<sf::Color> colors = { sf::Color::Yellow, sf::Color::White, sf::Color::Black,
			sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
			sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent });
		void render(sf::RenderTarget& target);
	private:
		sf::Texture t;
		sf::RenderStates state;
		std::vector<sf::Vertex> image;
	};

	class ButtonImageList
	{
	public:
		ButtonImageList(std::vector<std::pair<sf::String, bool>> paths, sf::FloatRect bounds, sf::FloatRect tbounds,
			sf::Font& font, unsigned charSize, sf::Vector2u w_size,
			int active = 0,
			std::vector<sf::Color> colors = { sf::Color::Yellow, sf::Color::White, sf::Color::Black,
			sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
			sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent });
		~ButtonImageList();
		void render(sf::RenderTarget& target);
		void update(sf::Vector2i& mousePosWin);
		const int& getActive();
		const bool activeAvailable();
		const bool isClicked();

	private:
		std::vector<std::pair<std::unique_ptr<ButtonImage>, bool>>items; // bool - available or not
		int active;
		sf::RectangleShape frame, curtain;
		sf::Text curtainText;
		bool clicked;
	};
	class Input
	{
	public:
		Input(sf::FloatRect bounds, sf::Color color_field, sf::Color color_text, sf::Font& font, unsigned char_size, sf::String label = "",
			bool hide = false, unsigned size = 20, sf::String placeholder = "", bool only_numbers = false);
		~Input();

		const std::wstring get();
		void setTextPosition();
		void setPosition(sf::Vector2f pos);
		void enter(unsigned int x);
		void update(sf::Vector2i mouse_pos_win, const float& dt);
		void render(sf::RenderTarget& target);
		void setFocus(bool x = true);
		const bool& getFocus();
	protected:
		sf::FloatRect bounds;
		std::vector<sf::Vertex> field;
		std::vector<sf::Vertex>outline;
		std::vector<sf::Vertex>cursor;
		Timer timerCursor;
		bool showCursor;
		sf::Text text, label;
		unsigned size;
		sf::Color colorField;
		sf::Color colorText;
		bool focused, hide, onlyNumbers;
		sf::String svalue;
	};



}