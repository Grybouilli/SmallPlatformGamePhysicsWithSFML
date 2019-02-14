#ifndef GUARD_APPLICATION_HPP
#define GUARD_APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "hdr/TextureHolder.hpp"
#include "hdr/Player.hpp"

using CollisionBoxes = std::vector<sf::RectangleShape>;

class Application
{
public:
	Application();
	void run();

private:
	void update(sf::Time dt);
	void render();
	void processEvents();
	void loadTextures();

private:
	sf::RenderWindow	mWindow;
	const sf::Time		mTimePerFrame;

	TextureHolder		mTextures;
	Player				mPlayer;
	CollisionBoxes		mBoxes;

};

#endif