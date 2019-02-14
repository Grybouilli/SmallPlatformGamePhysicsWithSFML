#include "hdr/Animation.hpp"

Animation::Animation()
: mElapsedTime { sf::Time::Zero }
, mDuration { sf::Time::Zero }
{
}

Animation::Animation(sf::Texture& spriteSheet, sf::IntRect firstFrame, int framesNumber, sf::Time animDuration)
: mShape {}
, mFirstFrame { firstFrame }
, mFrameSize { firstFrame.width, firstFrame.height }
, mFramesNumber { framesNumber }
, mCurrentFrame { 1 }
, mElapsedTime { sf::Time::Zero }
, mDuration { animDuration }
{
	mShape.setTexture(spriteSheet);
	mShape.setTextureRect(firstFrame);
}


void Animation::update(sf::Time dt)
{
	if(mFramesNumber <= 1)
	{
		//unanimated or uninitialized sprite
		return;
	}

	mElapsedTime += dt;

	while(mElapsedTime > mDuration)
	{
		if(mCurrentFrame + 1 <= mFramesNumber)
		{
			++mCurrentFrame;
			mShape.setTextureRect(sf::IntRect(
				mFrameSize.x * (mCurrentFrame - 1), mFirstFrame.top, mFrameSize.x, mFrameSize.y));
		} else
		{
			mCurrentFrame = 1;
			mShape.setTextureRect(sf::IntRect(
				mFirstFrame.left, mFirstFrame.top, mFrameSize.x, mFrameSize.y));
		}

		mElapsedTime -= mDuration;
	}

}

void Animation::setFramesNumber(int nb)
{
	assert(nb > 0);

	mCurrentFrame = 1;
	mFramesNumber = nb;
}

void Animation::setTexture(sf::Texture& texture)
{
	mShape.setTexture(texture);
}

void Animation::setDuration(sf::Time duration)
{
	mElapsedTime = sf::Time::Zero;
	mDuration = duration;
}

void Animation::setFirstFrame(sf::IntRect firstFrame)
{
	mFirstFrame = firstFrame;
	mFrameSize = sf::Vector2i(firstFrame.width, firstFrame.height);
	mShape.setTextureRect(firstFrame);
}

int Animation::getFramesNumber() const
{
	return mFramesNumber;
}

sf::Time Animation::getDuration() const
{
	return mDuration;
}

sf::IntRect Animation::getFirstFrame() const
{
	return mFirstFrame;
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return sf::FloatRect{ getPosition().x, getPosition().y,
	 mShape.getTextureRect().width, mShape.getTextureRect().height};
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect{ 0.f, 0.f,
	 mShape.getTextureRect().width, mShape.getTextureRect().height};
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mShape, states);
}
