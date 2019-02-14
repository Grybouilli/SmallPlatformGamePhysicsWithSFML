#include "hdr/TextureHolder.hpp"
#include <stdexcept>
#include <memory>

TextureHolder::TextureHolder()
: mTextures{}
{
}

void TextureHolder::load(Texture::ID id, const std::string& fileName)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if(!texture->loadFromFile(fileName))
	{
		throw std::runtime_error("Could not load texture: " + fileName);
	}

	auto inserted = mTextures.insert(std::make_pair(id, std::move(texture)));

	assert(inserted.second);

}

sf::Texture& TextureHolder::get(Texture::ID id) {
	auto found = mTextures.find(id);

	assert(found != mTextures.end());

	return *found->second;
}

const sf::Texture& TextureHolder::get(Texture::ID id) const {
	auto found = mTextures.find(id);

	assert(found != mTextures.end());

	return *found->second;
}