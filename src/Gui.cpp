#include "pch.h"
#include "Gui.h"

namespace gui
{
	float gui::p2pX(float perc, const sf::Vector2u& v)
	{
		return std::floor(static_cast<float>(v.x * (perc / 100.f)));
	}

	float gui::p2pY(float perc, const sf::Vector2u& v)
	{
		return std::floor(static_cast<float>(v.y * (perc / 100.f)));
	}

	const unsigned gui::calcCharSize(const sf::Vector2u& v, const unsigned modifier)
	{
		return static_cast<unsigned>((v.x + v.y) / modifier);
	}
	void setVertexShape(std::vector<sf::Vertex>& varray, sf::FloatRect bounds, bool pos)
	{
		if (pos)
		{
			varray[0].position = { bounds.left, bounds.top };
			varray[1].position = { bounds.left + bounds.width, bounds.top };
			varray[2].position = { bounds.left + bounds.width, bounds.top + bounds.height };
			varray[3].position = { bounds.left, bounds.top + bounds.height };
		}						 
		else					 
		{						 
			varray[0].texCoords = {bounds.left, bounds.top};
			varray[1].texCoords = { bounds.left + bounds.width, bounds.top };
			varray[2].texCoords = { bounds.left + bounds.width, bounds.top + bounds.height };
			varray[3].texCoords = { bounds.left, bounds.top + bounds.height };
		}
	}
	sf::FloatRect getVertexShape(std::vector<sf::Vertex>& varray)
	{
		return sf::FloatRect(varray[0].position.x, varray[0].position.y, varray[1].position.x - varray[0].position.x,
			varray[2].position.y - varray[1].position.y);
	}

	
	Button::Button(sf::FloatRect bounds, sf::String text, sf::Font& font, unsigned charSize,sf::Vector2u w_size,
		std::vector<sf::Color> colors) : textIdle(*colors.begin()),
		textHover(*(colors.begin()+1)), textActive(*(colors.begin() + 2)),
		idle(*(colors.begin() + 3)), hover(*(colors.begin() + 4)), active(*(colors.begin() + 5)),
		outIdle(*(colors.begin() + 6)), outHover(*(colors.begin() + 7)), outActive(*(colors.begin() + 8)),
		state(BTN_STATES::IDLE), clicked(false)
	{
		bounds.left = gui::p2pX(bounds.left, w_size);
		bounds.top = gui::p2pY(bounds.top, w_size);
		bounds.width = gui::p2pX(bounds.width, w_size);
		bounds.height = gui::p2pY(bounds.height, w_size);
		this->bounds = bounds;

		this->shape.resize(4, sf::Vertex());
		this->outline.resize(5, sf::Vertex());
		//Shape
		setVertexShape(this->shape, this->bounds);
		//Outline
		setVertexShape(this->outline, sf::FloatRect(this->bounds.left - 1.f, this->bounds.top - 1.f, this->bounds.width + 2.f, this->bounds.height + 2.f));
		this->outline[4].position = this->outline[0].position;
		//Text 
		this->text.setString(text);
		this->text.setFont(font);
		this->text.setCharacterSize(charSize);
		this->text.setPosition(sf::Vector2f(bounds.left + (bounds.width / 2.f) - this->text.getGlobalBounds().width / 2.f,
			bounds.top + (bounds.height / 3.7f) - this->text.getGlobalBounds().height / 2.f));
		this->text.setFillColor(this->textIdle);
		this->text.setOutlineThickness(2.f);
		this->text.setOutlineColor(sf::Color::Black);
		this->text.setFillColor(this->textIdle);
		this->setColor(this->idle, this->outIdle);
		//init sound
		this->buffer.loadFromFile("Resources/Sounds/click.wav");
		this->sound.setVolume(10.f);
	}

	Button::~Button()
	{
	}

	const bool&& Button::isClicked()
	{
		if (this->clicked)
		{
			this->clicked = false;
			return true;
		}
		return false;
	}

	const sf::String& Button::getText() const
	{
		return this->text.getString();
	}

	void Button::setText(sf::String str)
	{
		this->text.setString(str);
		this->text.setPosition(sf::Vector2f(bounds.left + (bounds.width / 2.f) - this->text.getGlobalBounds().width / 2.f,
			this->text.getPosition().y));
	}

	void Button::setColor(sf::Color color, sf::Color out_color)
	{
		for (auto& i : this->shape)
			i.color = color;
		for (auto& i : this->outline)
			i.color = out_color;
	}

	void Button::update(sf::Vector2i& mousePosWin)
	{

		if (bounds.contains(static_cast<sf::Vector2f>(mousePosWin)))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->state = BTN_STATES::ACTIVE;
				this->text.setFillColor(this->textActive);
				this->setColor(this->active, this->outActive);
			}
			else
			{
				if (this->state == BTN_STATES::ACTIVE)
				{
					this->sound.setBuffer(this->buffer);
					this->sound.play();
					do 
					{
					} while (!this->sound.getStatus() == sf::Sound::Stopped);
					this->clicked = true;
				}
					
				this->state = BTN_STATES::HOVER;
				this->text.setFillColor(this->textHover);
				this->setColor(this->hover, this->outHover);
			}
		}
		else
		{
			this->state = BTN_STATES::IDLE;
			this->text.setFillColor(this->textIdle);
			this->setColor(this->idle, this->outIdle);
		}

	}
	void Button::render(sf::RenderTarget& target)
	{
		target.draw(&this->shape[0], 4, sf::Quads);
		target.draw(&this->outline[0], 5, sf::LinesStrip);
		target.draw(this->text);
	}

	///							///
	///							///	
	///							///
	/// BUTTON IMAGE			///	BUTTON IMAGE	
	///							///
	///							///
	///							///
	ButtonImage::ButtonImage(sf::String path, sf::FloatRect bounds, sf::FloatRect tbounds,
	sf::String text, sf::Font& font, unsigned charSize,
	sf::Vector2u w_size, std::vector<sf::Color> colors) : Button(bounds, text, font, charSize, w_size, colors)
	{
		t.loadFromFile(path);
		image.resize(4, sf::Vertex());
		setVertexShape(image, this->bounds);
		state.texture = &this->t;
		setVertexShape(image, tbounds,0);
	}

	void ButtonImage::render(sf::RenderTarget& target)
	{
		target.draw(&this->image[0], 4, sf::Quads, state);
	}


	///							///
	///							///	
	///							///
	/// BUTTON IMAGE LIST		///	BUTTON IMAGE LIST	
	///							///
	///							///
	///							///

	ButtonImageList::ButtonImageList(std::vector<std::pair<sf::String, bool>> paths,
		sf::FloatRect bounds, sf::FloatRect tbounds, sf::Font& font, unsigned charSize,
		sf::Vector2u w_size, int active, std::vector<sf::Color> colors) 
		: active(active)
	{
		this->frame.setSize(sf::Vector2f(p2pX(bounds.width, w_size), p2pY(bounds.height, w_size)));
		this->frame.setPosition(p2pX(bounds.left, w_size), p2pY(bounds.top, w_size));
		this->curtain = frame;
		this->curtain.setFillColor(sf::Color(0, 0, 0, 210));
		this->frame.setOutlineColor(sf::Color::Black);
		this->frame.setOutlineThickness(3.f);
		this->frame.setFillColor(sf::Color::Transparent);
		
		this->curtainText.setString("Not available");
		this->curtainText.setCharacterSize(calcCharSize(w_size));
		this->curtainText.setFont(font);
		this->curtainText.setPosition(
			p2pX(bounds.left, w_size) + p2pX(bounds.width, w_size) / 2 - this->curtainText.getGlobalBounds().width/2,
			p2pY(bounds.top, w_size) + p2pY(bounds.height, w_size) / 2);
		
		

		for (auto& i : paths)
		{
			this->items.push_back({
				std::make_unique<ButtonImage>(i.first, bounds, tbounds,
					"", font, charSize, w_size, colors), i.second
		});
		}
	}

	ButtonImageList::~ButtonImageList()
	{
	}

	void ButtonImageList::render(sf::RenderTarget& target)
	{
		target.draw(this->frame);
		this->items[active].first->render(target);
		if (!this->items[active].second)
		{
			target.draw(this->curtain);
			target.draw(this->curtainText);
		}
			
	}

	void ButtonImageList::update(sf::Vector2i& mousePosWin)
	{
		this->items[active].first->update(mousePosWin);
		if (this->items[active].first->isClicked())
			if (++active >= this->items.size())
				active = 0;
	}
	
	///							///
	///							///	
	///							///
	///	INPUT					///	INPUT	
	///							///
	///							///
	///							///
	
	Input::Input(sf::FloatRect bounds, sf::Color color_field, sf::Color color_text, sf::Font& font, unsigned char_size, bool hide,
		unsigned size, sf::String placeholder)
		: bounds(bounds), field(4, sf::Vertex()), outline(5, sf::Vertex()), cursor(2, sf::Vertex()),
		colorField(color_field), colorText(color_text),
		size(size), focused(0), timerCursor(Timer(0.f, 40.f)), showCursor(1), hide(hide)
	{
		gui::setVertexShape(this->field, bounds);
		for (auto& i : this->field)
			i.color = this->colorField;

		setVertexShape(this->outline, sf::FloatRect(this->bounds.left - 2.f, this->bounds.top - 2.f, this->bounds.width + 3.f, this->bounds.height + 3.f));
		this->outline[4].position = this->outline[0].position;
		for (auto& i : this->outline)
			i.color = sf::Color::Black;

		this->text.setFont(font);
		this->svalue = placeholder;
		this->text.setString(this->svalue);
		this->text.setCharacterSize(char_size);
		this->text.setFillColor(this->colorText);
		this->text.setPosition(bounds.left + bounds.width / 2.f - this->text.getGlobalBounds().width / 2.f,
			bounds.top + (bounds.height / 3.7f) - this->text.getGlobalBounds().height / 2.f);

		this->cursor[0].position = { this->text.getPosition().x + this->text.getGlobalBounds().width + 4.f, this->text.getPosition().y + this->text.getGlobalBounds().height / 2.f };
		this->cursor[1].position = { this->cursor[0].position.x, this->cursor[0].position.y + char_size / 1.2f };
		this->cursor[0].color = sf::Color::Black;
		this->cursor[1].color = sf::Color::Black;

	}

	Input::~Input()
	{
	}

	const std::wstring Input::get()
	{
		return this->svalue;
	}

	void Input::setPosition()
	{
		this->text.setPosition(this->bounds.left + this->bounds.width / 2.f - this->text.getGlobalBounds().width / 2.f,
			this->text.getPosition().y);
		this->cursor[0].position = { this->text.getPosition().x + this->text.getGlobalBounds().width + 4.f, this->cursor[0].position.y };
		this->cursor[1].position = { this->cursor[0].position.x, this->cursor[0].position.y + this->text.getCharacterSize() / 1.2f };
	}

	void Input::enter(unsigned int x)
	{
		if (focused && x != 38 && x!=32 && x != 9 && x != 13)
		{
			if (x == 8)
			{
				if (this->svalue.getSize() > 1)
				{
					if (this->svalue[this->text.getString().getSize() - 1] == 10)
						this->svalue = this->svalue.substring(0, this->svalue.getSize() - 2);
					else
						this->svalue = this->svalue.substring(0, this->svalue.getSize() - 1);
				}
				else
					this->svalue = this->svalue.substring(0, this->svalue.getSize() - 1);
			}

			else if (this->svalue.getSize() < this->size)
			{
				if (this->cursor[0].position.x > this->bounds.left + 5 * (this->bounds.width / 6) && this->size > 30)
				{
					this->svalue = this->svalue + '\n' + x;
				}
				else
					this->svalue = this->svalue + x;
			}
			if (this->hide)
			{
				sf::String temp = this->svalue;
				for(auto&i : temp)
				{
					if (i != '*')
						i = '*';
				}
				this->text.setString(temp);
			}
			else
				this->text.setString(this->svalue);
			
				
			
			this->setPosition();

			this->showCursor = 1;
			this->timerCursor.reset();
		}

	}

	void Input::update(sf::Vector2i mouse_pos_win, const float& dt)
	{
		this->timerCursor.update(dt);
		if (this->timerCursor.get())
			this->showCursor = !this->showCursor;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if (this->bounds.contains(sf::Vector2f(mouse_pos_win)))
				this->focused = true;
			else
				this->focused = false;
	}

	void Input::render(sf::RenderTarget& target)
	{
		target.draw(&this->field[0], 4, sf::Quads);
		if (this->focused)
		{
			target.draw(&this->outline[0], 5, sf::LinesStrip);
			if (this->showCursor)
				target.draw(&this->cursor[0], 2, sf::Lines);
		}

		target.draw(this->text);
	}

	void Input::setFocus(bool x)
	{
		this->focused = x;
	}

	const bool& Input::getFocus()
	{
		return this->focused;
	}

}