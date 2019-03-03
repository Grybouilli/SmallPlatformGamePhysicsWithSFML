#ifndef GUARD_PLAYER_HPP
#define GUARD_PLAYER_HPP

#include "hdr/TextureHolder.hpp"
#include "hdr/Animation.hpp"
#include "DataTables.hpp"
#include <SFML/Graphics.hpp>

class Player : public sf::Drawable, public sf::Transformable
{
public:
	Player();
	Player(TextureHolder& textures);
	Player(TextureHolder& textures, sf::Vector2f position);
	Player(TextureHolder& textures, float x, float y);

	void 				update(sf::Time dt);
	void				handleEvent(const sf::Event& event);

	bool				isBottomCollided(sf::FloatRect target);

private:
	void 				jump();
	void				checkForRunState();
	void				applyForces(sf::Time dt);
	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Animation			mShape;

	PlayerAnimations	mDefaultState;
	PlayerAnimations	mCurrentState;
	PlayerAnimations	mPendingState;

	sf::Vector2f		mVelocity;
	sf::Vector2f		mInitialJumpVelocity;

	sf::Time			mInAirTime;

	bool				mBottomCollided;
	bool				mIsJumping;
};

#endif