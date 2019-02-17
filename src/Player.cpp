#include "hdr/Player.hpp"
#include <iostream>

const float jumpVertcalVelocity = -25.f;
const sf::Time defaultAnimDuration = sf::seconds(3.f);

Player::Player()
: mShape{}
, mDefaultState { PlayerAnimations::Stand }
, mCurrentState { PlayerAnimations::Stand }
, mPendingState { PlayerAnimations::Stand }
, mVelocity { 0, 0 }
, mInitialJumpVelocity { 0, jumpVertcalVelocity }
, mBottomCollided { false }
{

}

Player::Player(TextureHolder& textures)
: mShape {}
, mDefaultState { PlayerAnimations::Stand }
, mCurrentState { PlayerAnimations::Stand }
, mPendingState { PlayerAnimations::Stand }
, mVelocity { 0, 0 }
, mInitialJumpVelocity { 0, jumpVertcalVelocity }
, mBottomCollided { false }
{
	//taking texture from resources
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	sf::Texture& texture = textures.get(playerData->textureID);

	//setting current animation to default: stand position
	mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
	mShape.setTexture(texture);
	mShape.setDuration(defaultAnimDuration);
	mShape.setFirstFrame(playerData->animations.defaultTile);

	setPosition(0, 0);
}

Player::Player(TextureHolder& textures, sf::Vector2f position)
: mShape {}
, mDefaultState { PlayerAnimations::Stand }
, mCurrentState { PlayerAnimations::Stand }
, mPendingState { PlayerAnimations::Stand }
, mVelocity { 0, 0 }
, mInitialJumpVelocity { 0, jumpVertcalVelocity }
, mBottomCollided { false }
{
	//taking texture from resources
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	sf::Texture& texture = textures.get(playerData->textureID);

	//setting current animation to default: stand position
	mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
	mShape.setTexture(texture);
	mShape.setDuration(defaultAnimDuration);
	mShape.setFirstFrame(playerData->animations.defaultTile);

	setPosition(position);
}

Player::Player(TextureHolder& textures, float x, float y)
: mShape {}
, mDefaultState { PlayerAnimations::Stand }
, mCurrentState { PlayerAnimations::Stand }
, mPendingState { PlayerAnimations::Stand }
, mVelocity { 0, 0 }
, mInitialJumpVelocity { 0, jumpVertcalVelocity }
, mBottomCollided { false }
{
	//taking texture from resources
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	sf::Texture& texture = textures.get(playerData->textureID);

	//setting current animation to default: stand position
	mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
	mShape.setTexture(texture);
	mShape.setDuration(defaultAnimDuration);
	mShape.setFirstFrame(playerData->animations.defaultTile);

	setPosition(x, y);
}

void Player::update(sf::Time dt)
{
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	//if a different state than current was triggered, handles it
	if(mPendingState != mCurrentState)
	{
		//position of the textureRect on the sprite sheet depending on the triggered state
		sf::IntRect firstFrame = playerData->animations.defaultTile;
		firstFrame.top = playerData->animations.tileSize.y * static_cast<int>(mPendingState);

		switch(mPendingState)
		{
			case PlayerAnimations::Stand :
				mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
				mShape.setFirstFrame(playerData->animations.defaultTile);
				mShape.setDuration(defaultAnimDuration);
				break;
			case PlayerAnimations::Walk :
				mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Walk)]);
				mShape.setFirstFrame(firstFrame);
				mShape.setDuration(defaultAnimDuration);
				break;
			case PlayerAnimations::Run :
				mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Run)]);
				mShape.setFirstFrame(firstFrame);
				mShape.setDuration(sf::seconds(1.5f));
				break;
			case PlayerAnimations::Jump :
				mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Jump)]);
				mShape.setFirstFrame(firstFrame);
				mShape.setDuration(defaultAnimDuration);
				break;
		}

		mCurrentState = mPendingState;

	}

	//if the player is in the air, we increment the time spent in the air
	if(!mBottomCollided)
	{
		mInAirTime += dt;
	} else
	{
		//if it is not, we ensure the the InAirTime value is set to zero
		mInAirTime = sf::Time::Zero;
	}

	applyForces(dt);
	mShape.update(dt);

}

void Player::handleEvent(const sf::Event& event)
{
	//depending on triggered key, we set a specific pending state
	if(event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::Space:
				mPendingState = PlayerAnimations::Jump;
				jump();
				break;
			case sf::Keyboard::Tab:
				if(mVelocity.x != 0.f && mCurrentState != PlayerAnimations::Run)
				{
					mVelocity.x *= 1.7f;
					mPendingState = PlayerAnimations::Run;
				}
				break;
			case sf::Keyboard::Left:
				mPendingState = PlayerAnimations::Walk;
				mVelocity.x = -20.f;
				break;
			case sf::Keyboard::Right:
				mPendingState = PlayerAnimations::Walk;
				mVelocity.x = 20.f;
				break;
			default:
			//if any other key is triggered, we set the pending state to default
				mPendingState = PlayerAnimations::Stand;
				mVelocity = sf::Vector2f(0.f, 0.f);
				break;
		}
	}
	//if a specific key is released, we set the pending state to default
	if(event.type == sf::Event::KeyReleased)
	{
		if(event.key.code == sf::Keyboard::Tab && mVelocity.x != 0.f)
		{
			mPendingState = PlayerAnimations::Walk;
			mVelocity.x *= 1.f / 1.7f;
		}

		if(event.key.code == sf::Keyboard::Space
		|| event.key.code == sf::Keyboard::Left
		|| event.key.code == sf::Keyboard::Right)
		{
			mPendingState = PlayerAnimations::Stand;
			mVelocity = sf::Vector2f(0.f, 0.f);
		}
	}
}

bool Player::isBottomCollided(sf::FloatRect target)
{
	//if currently jumping, cannot collide bottom
	if(mVelocity.y < 0.f)
		return false;

	sf::FloatRect bounds { mShape.getGlobalBounds() };
	bounds.left += 15.f; //reduces width range for bottom collision
	bounds = getTransform().transformRect(bounds);

	if(bounds.intersects(target))
	{
		mBottomCollided = true;
		return mBottomCollided;
	} else
	{
		mBottomCollided = false;
		return mBottomCollided;
	}

}

void Player::jump()
{
	//setting y component of velocity to initial jump value
	//setting mBottomCollided to false because player is going to be in the air
	mVelocity.y = mInitialJumpVelocity.y;
	mBottomCollided = false;
}

void Player::applyForces(sf::Time dt)
{
	float newY { 0.f };

	//if player is in the air, weight is applied
	if(!mBottomCollided)
	{
		float t { mInAirTime.asSeconds() };
		newY = 5 * t * t + mInitialJumpVelocity.y * t;
	}
	//factor of +20% to make the jump quicker
	mVelocity.y = newY * 1.5f;

	move(mVelocity * dt.asSeconds());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mShape, states);
}