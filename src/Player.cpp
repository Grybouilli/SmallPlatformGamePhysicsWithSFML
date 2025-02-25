#include "hdr/Player.hpp"
#include <iostream>
#include "hdr/collidesBottom.hpp"

const float jumpVertcalVelocity = -20.f;
const float horizontalWalkSpeed = 40.f;
const float runFactor = 1.7f;
const sf::Time defaultAnimDuration = sf::seconds(2.f);

Player::Player()
: mShape{}
, mDefaultState { PlayerAnimations::Stand }
, mCurrentState { PlayerAnimations::Stand }
, mPendingState { PlayerAnimations::Stand }
, mVelocity { 0, 0 }
, mInitialJumpVelocity { 0, jumpVertcalVelocity }
, mBottomCollided { false }
, mIsJumping { false }
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
, mIsJumping { false }
{
	//taking texture from resources
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	sf::Texture& texture = textures.get(playerData->textureID);

	//setting current animation to default: stand position
	mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
	mShape.setTexture(texture);
	mShape.setDuration(defaultAnimDuration);
	mShape.setFirstFrame(playerData->animations.defaultTileOR);

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
, mIsJumping { false }
{
	//taking texture from resources
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	sf::Texture& texture = textures.get(playerData->textureID);

	//setting current animation to default: stand position
	mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
	mShape.setTexture(texture);
	mShape.setDuration(defaultAnimDuration);
	mShape.setFirstFrame(playerData->animations.defaultTileOR);

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
, mIsJumping { false }
{
	//taking texture from resources
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	sf::Texture& texture = textures.get(playerData->textureID);

	//setting current animation to default: stand position
	mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
	mShape.setTexture(texture);
	mShape.setDuration(defaultAnimDuration);
	mShape.setFirstFrame(playerData->animations.defaultTileOR);

	setPosition(x, y);
}

void Player::update(sf::Time dt)
{
	auto playerData = &Table[static_cast<int>(EntityType::Player)];
	if(mPendingState == PlayerAnimations::Jump && mBottomCollided)
	{
		mPendingState = PlayerAnimations::Stand;
		mVelocity.x = 0.f;
	}
	//if a different state than current was triggered, handles it
	sf::IntRect firstFrame {};
	//position of the textureRect on the sprite sheet depending on the horizontal orientation
	if(mVelocity.x < 0.f)
	{
		firstFrame = playerData->animations.defaultTileOL;
	} else
	{
		firstFrame = playerData->animations.defaultTileOR;
	}

	firstFrame.top = playerData->animations.tileSize.y * static_cast<int>(mPendingState);

	if(mShape.getFirstFrame() != firstFrame)
	{
		mShape.setFirstFrame(firstFrame);
	}

	if(mPendingState != mCurrentState)
	{
		//position of the textureRect on the sprite sheet depending on the triggered state
		switch(mPendingState)
		{
			case PlayerAnimations::Stand :
				mShape.setFramesNumber(playerData->animations.framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)]);
				mShape.setFirstFrame(playerData->animations.defaultTileOR);
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
				mShape.setDuration(sf::seconds(1.f));
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
			//checking for current state to avoid infinit horizontal velocity
				if(mCurrentState != PlayerAnimations::Run)
				{
					mVelocity.x *= runFactor;
					mPendingState = PlayerAnimations::Run;
				}
				break;
			case sf::Keyboard::Left:
				mVelocity.x = -horizontalWalkSpeed;
				checkForRunState(); //keeps player running if tab was pressed before left key
				break;
			case sf::Keyboard::Right:
				mVelocity.x = horizontalWalkSpeed;
				checkForRunState();  //keeps player running if tab was pressed before right key
				break;
			default:
			//if any other key is triggered, we set the pending state to default
				mPendingState = PlayerAnimations::Stand;
				mVelocity = sf::Vector2f(0.f, 0.f);
				break;
		}
	}

	//handles release of keys
	if(event.type == sf::Event::KeyReleased)
	{
		//x velocity
		// if still walking
		if(event.key.code == sf::Keyboard::Tab && mVelocity.x != 0.f)
		{
			mPendingState = PlayerAnimations::Walk;
			mVelocity.x = horizontalWalkSpeed;
		}
		else if(event.key.code == sf::Keyboard::Left
		|| event.key.code == sf::Keyboard::Right
		|| event.key.code == sf::Keyboard::Tab)
		{
			//not walking anymore, not doing anything
			mPendingState = PlayerAnimations::Stand;
			mVelocity.x = 0.f;
		}
	}
}

bool Player::isBottomCollided(sf::FloatRect target)
{
	//if currently jumping, cannot collide bottom
	if(mVelocity.y < 0.f)
	{
		mBottomCollided = false;
		return false;
	}
	sf::FloatRect bounds { mShape.getGlobalBounds() };
	bounds.left += 15.f; //reduces width range for bottom collision
	bounds.width -= 15.f;
	bounds = getTransform().transformRect(bounds);

	if(collidesBottom(target, bounds))
	{
		mIsJumping = false;
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
	mIsJumping = true;
}

void Player::checkForRunState()
{
	//checking if running key is being pressed
	mPendingState = mCurrentState == PlayerAnimations::Run ?
	mCurrentState : PlayerAnimations::Walk;
	if(mCurrentState == PlayerAnimations::Run)
	{
		mVelocity.x *= runFactor;
	}
}

void Player::applyForces(sf::Time dt)
{
	float newY { 0.f };

	//if player is in the air, weight is applied and has jumped
	if(!mBottomCollided && mIsJumping)
	{
		float t { mInAirTime.asSeconds() };
		newY = 5 * t * t + mInitialJumpVelocity.y * t; //mInitialJumVelocity.y < 0
	} else if(!mBottomCollided && !mIsJumping)
	{
		//if player is just falling (no jump triggered)
		float t { mInAirTime.asSeconds() };
		newY = 5 * t * t - mInitialJumpVelocity.y;
	}
	//growing the y velocity to make the jump quicker
	mVelocity.y = newY * 2.5f;

	move(mVelocity * dt.asSeconds());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mShape, states);
}