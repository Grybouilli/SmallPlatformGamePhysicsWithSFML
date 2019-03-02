#include "hdr/DataTables.hpp"

std::vector<EntityData> initializeDataTables()
{
	std::vector<EntityData> table { static_cast<int>(EntityType::EntityCount) };

	table[static_cast<int>(EntityType::Player)].type = EntityType::Player;
	table[static_cast<int>(EntityType::Player)].textureID = Texture::ID::Player;

	auto animationsData = &table[static_cast<int>(EntityType::Player)].animations;

	//hard-coded animations settings
	animationsData->animationsCount = static_cast<size_t>(PlayerAnimations::AnimationCount);
	animationsData->framesPerAnimation = std::vector<int>{ static_cast<size_t>(PlayerAnimations::AnimationCount) };

	animationsData->framesPerAnimation[static_cast<int>(PlayerAnimations::Stand)] = 3;
	animationsData->framesPerAnimation[static_cast<int>(PlayerAnimations::Walk)] = 4;
	animationsData->framesPerAnimation[static_cast<int>(PlayerAnimations::Run)] = 4;
	animationsData->framesPerAnimation[static_cast<int>(PlayerAnimations::Jump)] = 1;

	animationsData->tileSize = sf::Vector2i(32, 32);
	animationsData->defaultTileOR = sf::IntRect(0, 0, 32, 32); //OR = oriented to the right
	animationsData->defaultTileOL = sf::IntRect(128, 0, 32, 32); //OL = oriented to the left

	return table;

}