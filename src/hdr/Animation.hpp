#ifndef GUARD_ANIMATION_HPP
#define GUARD_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include "hdr/DataTables.hpp"

namespace
{
	const std::vector<EntityData> Table = initializeDataTables();
}

class Animation: public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	Animation(sf::Texture& spriteSheet, sf::IntRect firstFrame, int framesNumber, sf::Time animDuration);

	void				update(sf::Time dt);

	void				setFramesNumber(int nb);
	void				setTexture(sf::Texture& texture);
	void				setDuration(sf::Time duration);
	void				setFirstFrame(sf::IntRect firstFrame);

	int					getFramesNumber() const;
	sf::Time			getDuration() const;
	sf::IntRect			getFirstFrame() const;
	sf::FloatRect		getGlobalBounds() const;
	sf::FloatRect		getLocalBounds() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite			mShape;

	sf::IntRect			mFirstFrame;
	sf::Vector2i		mFrameSize;
	int					mFramesNumber;
	int					mCurrentFrame;

	sf::Time			mElapsedTime;
	sf::Time			mDuration;
};

#endif