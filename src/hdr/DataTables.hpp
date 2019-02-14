#ifndef GUARD_DATATABLES_HPP
#define GUARD_DATATABLES_HPP

#include <vector>
#include "hdr/TextureHolder.hpp"

enum class EntityType
{
	Player = 0,
	EntityCount
};

enum class PlayerAnimations
{
	Stand = 0,
	Walk,
	Run,
	Jump,
	AnimationCount
};

struct AnimationData
{
	size_t				animationsCount;
	std::vector<int>	framesPerAnimation;

	sf::Vector2i		tileSize;
	sf::IntRect			defaultTile;
};

struct EntityData
{
	EntityType			type;
	Texture::ID			textureID;
	AnimationData		animations;
};

std::vector<EntityData> initializeDataTables();
#endif