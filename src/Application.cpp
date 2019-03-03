#include "hdr/Application.hpp"

Application::Application()
: mWindow{ sf::VideoMode(2000, 1200), "Animating Stuff", sf::Style::Close | sf::Style::Titlebar }
, mTimePerFrame{ sf::seconds(1.f / 60.f) }
, mTextures {}
, mPlayer {}
, mBoxes {5}
{
	loadTextures();
	mPlayer = Player(mTextures);
	mPlayer.scale(5.f, 5.f);
	mPlayer.setPosition(0.f, 0.f);
	mBoxes[0].setSize(sf::Vector2f(2000.f, 10.f));
	mBoxes[0].setPosition(0.f, 1000.f);
	mBoxes[1].setSize(sf::Vector2f(100.f, 80.f));
	mBoxes[1].setPosition(500.f, 920.f);
	mBoxes[2].setSize(sf::Vector2f(100.f, 80.f));
	mBoxes[2].setPosition(mBoxes[1].getPosition().x + 100.f, mBoxes[1].getPosition().y - 80.f);
	mBoxes[3].setSize(sf::Vector2f(100.f, 80.f));
	mBoxes[3].setPosition(mBoxes[2].getPosition().x + 100.f, mBoxes[2].getPosition().y - 80.f);
	mBoxes[4].setSize(sf::Vector2f(100.f, 80.f));
	mBoxes[4].setPosition(mBoxes[3].getPosition().x + 100.f, mBoxes[3].getPosition().y - 80.f);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time elapsed = sf::Time::Zero;

	while(mWindow.isOpen())
	{
		elapsed = clock.restart();
		do
		{
			elapsed -= mTimePerFrame;
			update(mTimePerFrame);
			processEvents();
		} while (elapsed > mTimePerFrame);

		render();

	}
}

void Application::update(sf::Time dt)
{
	bool hasCollided = false;
	size_t i {0};
	while(!hasCollided && i != mBoxes.size())
	{
		hasCollided = mPlayer.isBottomCollided(mBoxes[i].getGlobalBounds());
		++i;
	}
	mPlayer.update(dt);
}

void Application::render()
{
	mWindow.clear();

	for (auto box : mBoxes)
	{
		mWindow.draw(box);
	}
	mWindow.draw(mPlayer);

	mWindow.display();
}

void Application::processEvents()
{
	sf::Event event;
	while(mWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			mWindow.close();
		}

		mPlayer.handleEvent(event);
	}
}

void Application::loadTextures()
{
	mTextures.load(Texture::ID::Player, "/home/grybouilli/projects/animation_for_platform_game/medias/textures/fullset_player.png");
}