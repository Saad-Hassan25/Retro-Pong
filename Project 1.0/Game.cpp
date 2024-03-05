

#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>
#include<SFML/Audio.hpp>
using namespace sf;
using namespace std;


float clamp(float n, float lower, float upper)
{
	n = (n > lower) * n + !(n > lower) * lower;
	return (n < upper) * n + !(n < upper) * upper;
}

bool collide(const RectangleShape& Rectangle, const CircleShape& Circle)
{
	float pointX;
	float pointY;
	float distanceX;
	float distanceY;
	float distance;

	pointX = clamp(Circle.getPosition().x, Rectangle.getPosition().x, Rectangle.getPosition().x + Rectangle.getPosition().x);
	pointY = clamp(Circle.getPosition().y, Rectangle.getPosition().y, Rectangle.getPosition().y + Rectangle.getPosition().y);
	distanceX = Circle.getPosition().x - pointX;
	distanceY = Circle.getPosition().y - pointY;
	distance = (distanceX * distanceX) + (distanceY * distanceY);

	if (distance < Circle.getRadius() * Circle.getRadius() && distanceX != Circle.getPosition().x && distanceY != Circle.getPosition().y)
	{
		return 3;
	}
	else if (distance < Circle.getRadius() * Circle.getRadius() && distanceX == Circle.getPosition().x)
	{
		return 2;
	}
	else if (distance < Circle.getRadius() * Circle.getRadius() && distanceY == Circle.getPosition().y)
	{
		return 1;
	}
}

bool check(int PosX)
{
	return (PosX >= 50 && PosX <= 495);
}



class Ball
{
private:
	float radius;
	sf::CircleShape Circle;

public:
	int x;
	int y;
	float speedX;
	float speedY;
	Ball() { ; };
	Ball(sf::RenderWindow& Window)
	{
		radius = 8;
		x = 50;
		y = Window.getSize().y / 3;
		speedX = 5;
		speedY = 5;
	}
	void showBall(sf::RenderWindow& Window)
	{
		Circle.setRadius(radius);
		Circle.setFillColor(sf::Color::Yellow);
		Circle.setPosition(x, y);
		Window.draw(Circle);
	}
	void start(sf::RenderWindow& Window, sf::CircleShape Circle)
	{
		x = x + speedX;
		y = y + speedY;
		if (y <= 50)
		{
			y = 50;
			speedX = 5;
			speedY = speedY * -1;
		}
		if (x < 50)
		{
			x = 50;
			speedX *= -1;
		}
		if (x > 480)
		{
			x = 480;
			speedX *= -1;
		}
		if (y >= 490)
		{
			y = 490;
			speedY = 5;
			speedY *= -1;
		}
		Window.clear();
	}
	sf::CircleShape getBall()
	{
		return Circle;
	}
};

void bot(sf::CircleShape Circle, Ball& ball, sf::RectangleShape& Rectangle, sf::RenderWindow& window)
{
	
if (check(Circle.getPosition().x))
	{
		Rectangle.setPosition(Circle.getPosition().x, window.getSize().y / 2 - 250.0f);
		window.draw(Rectangle);
		if (collide(Rectangle, Circle) == 1 || collide(Rectangle, Circle) == 2 || collide(Rectangle, Circle) == 3)
		{
			if (ball.speedY > 0)
			{
				ball.y == ball.y + 30.0f;
				ball.speedY *= 1;
			}
			else
			{
				ball.y -= 30.0f;
				ball.speedY *= -1;
			}
		}

	}
}



class Table
{
	ConvexShape shape;
public:
	Table() {};
	void drawMainTable(RenderWindow*& window)
	{
		Texture* texture = new Texture;
		texture->loadFromFile("bg1.jpg");///////////////////////////////////////////////table pic
		shape.setPointCount(4);
		shape.setPoint(0, sf::Vector2f(100, 50));
		shape.setPoint(1, sf::Vector2f(500, 50));
		shape.setPoint(2, sf::Vector2f(550, 500));
		shape.setPoint(3, sf::Vector2f(50, 500));
		shape.setOutlineThickness(10);
		shape.setOutlineColor(Color::White);
		shape.setTexture(texture, false);
		window->draw(shape);
	}
	ConvexShape gets()
	{
		return this->shape;
	}
};




class Paddle
{
public:
	sf::RectangleShape* Rectangle;
	int x;
	int y;
	int speed;

	Paddle() { ; }
	Paddle(sf::RenderWindow& window)
	{
		x = window.getPosition().x;
		y = window.getPosition().y;
		speed = 30;
	}
	void draw(sf::RenderWindow& window)
	{
		Rectangle = new sf::RectangleShape(sf::Vector2f(90, 25));
		Rectangle->setPosition(this->x, this->y);
		window.draw(*Rectangle);
	}
	sf::RectangleShape getRect()
	{
		return *this->Rectangle;
	}
};

class engine :public Ball, Table, Paddle
{
	Ball* B;
	Table* t;
	RenderWindow* win;
	Paddle* d, * l;
	Event event;
	VideoMode* vid;
	int score1, score2;
	Text s1, s2;
	Sprite s;
	Font f;
	bool range = true;
	bool play = false;
	bool pvp = false;
	bool pvc = false;
	bool openmenu = false;
	bool menu = false;
	Clock te;
	bool playsel = false, playpre = false;
	bool compsel = false, comppre = false;
	bool playbot = false;
public:
	engine() {}
	void initengine()
	{

		this->vid = new VideoMode(600, 600);
		this->win = new RenderWindow(*vid, "pong", sf::Style::Close);
		this->win->setFramerateLimit(80);
		win->clear();
		this->B = new Ball(*this->win);
		this->t = new Table;
		d = new Paddle(*this->win);
		l = new Paddle(*this->win);
		d->x = this->win->getSize().x / 2 - 50.f;
		d->y = this->win->getSize().y / 2 + 170.f;
		l->x = this->win->getSize().x / 2 - 50.f;
		l->y = this->win->getSize().y / 2 - 245.f;
		this->d->draw(*this->win);
		this->l->draw(*this->win);
		this->t->drawMainTable(this->win);
		sf::Texture* p1, * p2;
		p1 = new Texture;
		p2 = new Texture;
		p1->loadFromFile("p1.jpg");
		d->Rectangle->setTexture(p1, false);
		p2->loadFromFile("p2.jpg");
		l->Rectangle->setTexture(p2, true);
		this->score1 = score2 = 0;
		Texture* bg;
		bg = new Texture;
		bg->loadFromFile("t1.jpg");
		s.setTexture(*bg);
		f.loadFromFile("font.ttf");
		s1.setFont(f);
		s1.setPosition(win->getSize().x - 100.f, win->getSize().y / 2 + 210.f);
		s1.setCharacterSize(50);
		s1.setFillColor(Color::White);
		s1.setString(std::to_string(score1));
		s2.setFont(f);
		s2.setPosition(win->getSize().x - 550.f, win->getSize().y / 2 - 300.f);
		s2.setFillColor(sf::Color::White);
		s2.setString(to_string(score2));
		s2.setCharacterSize(50);
	}
	void pollevent()
	{
		while (this->win->pollEvent(this->event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				win->close();
			}
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				/*std::cout << "mouse x: " << e.mouseButton.x << std::endl;
				std::cout << "mouse y: " << e.mouseButton.y << std::endl;*/
				if (playsel)
				{
					play = true;
					playbot = false;
				}
				if (compsel)
				{
					play = true;
					playbot = true;
				}
			}
		}
		if (event.type == sf::Event::MouseMoved)
		{
			int x = (int)event.mouseMove.x;
			int y = (int)event.mouseMove.y;
			if (x >= 250 && x <= 341 && y >= 258 && y <= 288)playsel = true;
			else playsel = false;
			if (x >= 201 && x <= 405 && y >= 322 && y <= 369)compsel = true;
			else compsel = false;
		}
		if (!play)
		{
			Clock timer;
			if (!openmenu)
			{
				sf::Texture* le; le = new Texture;
				le->loadFromFile("start.png");
				sf::Sprite load; load.setTexture(*le);
				win->draw(load);
				if (te.getElapsedTime().asSeconds() >= 4)
				{
					menu = true;
				}
			}
			if (menu)
			{
				sf::Texture* le;
				le = new Texture;
				le->loadFromFile("menu.png");
				sf::Sprite load; 
				load.setTexture(*le);
				win->draw(load);
				Text play, vc, exit;
				f.loadFromFile("font1.ttf");
				if (!playsel)
				{
					play.setCharacterSize(40);
					play.setPosition(win->getSize().x / 2 - 70.f, win->getSize().y / 2 - 50.f);
					play.setFillColor(Color::White);
					play.setFont(f);
					play.setString("|PLAY|");
					win->draw(play);
				}
				if (!compsel)
				{
					vc.setCharacterSize(40);
					vc.setPosition(win->getSize().x / 2 - 100.f, win->getSize().y / 2 + 20.f);
					vc.setFillColor(Color::White);
					vc.setFont(f);
					vc.setString("|vs-Comp|");
					win->draw(vc);
				}

				if (playsel && !compsel)
				{
					play.setCharacterSize(60);
					play.setPosition(win->getSize().x / 2 - 80.f, win->getSize().y / 2 - 60.f);
					play.setFillColor(sf::Color::Green);
					play.setFont(f);
					play.setString("PLAY");
					win->draw(play);
				}
				if (compsel)
				{
					vc.setCharacterSize(60);
					vc.setPosition(win->getSize().x / 2 - 130.f, win->getSize().y / 2);
					vc.setFillColor(Color::Red);
					vc.setFont(f);
					vc.setString("vs-Comp");
					win->draw(vc);
				}
			}
		}
		if (play)
		{
			if (playbot)bot(B->getBall(), *B, *l->Rectangle, *win);
			this->B->start(*this->win, B->getBall());
			this->B->showBall(*this->win);
			win->draw(s);
			if (B->getBall().getPosition().y == 50)
			{
				score1++;
				s1.setString(std::to_string(score1));
				win->draw(s1);
			}
			if (B->getBall().getPosition().y == 490)
			{
				score2++;
				s2.setString(std::to_string(score2));
				win->draw(s2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (d->Rectangle->getPosition().x < 50)range = false;
				if (!range)
				{
					d->Rectangle->setPosition(d->Rectangle->getPosition().x + 10.f, d->Rectangle->getPosition().y);
					range = true;
				}
				else if (range)
					d->Rectangle->move(-10.f, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (d->Rectangle->getPosition().x > 425)range = false;
				if (!range)
				{
					d->Rectangle->setPosition(d->Rectangle->getPosition().x - 10.f, d->Rectangle->getPosition().y);
					range = true;
				}
				else if (range)
					d->Rectangle->move(10.f, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				if (l->Rectangle->getPosition().x < 80)range = false;
				if (!range)
				{
					l->Rectangle->setPosition(l->Rectangle->getPosition().x + 10.f, l->Rectangle->getPosition().y);
					range = true;
				}
				else if (range)
				{
					l->Rectangle->move(-10.f, 0);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				if (l->Rectangle->getPosition().x > 425)range = false;
				if (!range)
				{
					l->Rectangle->setPosition(l->Rectangle->getPosition().x - 10.f, l->Rectangle->getPosition().y);
					range = true;
				}
				else if (range) {
					l->Rectangle->move(10.f, 0);
				}
			}
			if (collide(*d->Rectangle, B->getBall()) == 1 || collide(*d->Rectangle, B->getBall()) == 2 || collide(*d->Rectangle, B->getBall()) == 3)
			{
				this->B->y -= 20.f;
				this->B->speedY *= -1;
			}
			if (!playbot)
			{
				if (collide(*l->Rectangle, B->getBall()) == 1 || collide(*l->Rectangle, B->getBall()) == 2 || collide(*l->Rectangle, B->getBall()) == 3)
				{
					if (B->speedY > 0)
					{
						B->y += 30.f;
						B->speedY *= 1;
					}
					else
					{
						B->y -= 30.f;
						B->speedY *= -1;
					}
				}
			}
			win->draw(t->gets());
			win->draw(B->getBall());
			win->draw(d->getRect());
			win->draw(l->getRect());
			win->draw(s1);
			win->draw(s2);
		}
	}
	void display()
	{
		this->win->display();
	}
	bool mainloop()
	{
		//win->draw(s);
		return this->win->isOpen();
	}
};
int main()
{

	srand(time(0));
	engine main;
	main.initengine();

	while (main.mainloop())
	{
		main.pollevent();
		main.display();
	}
}
