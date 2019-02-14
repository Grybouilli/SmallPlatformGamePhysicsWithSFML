#ifndef GUARD_TEXTUREHOLDER_HPP
#define GUARD_TEXTUREHOLDER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

namespace Texture {
	enum class ID {
		Player,
		Background
	};
}

class TextureHolder {
public:
	TextureHolder();

	void load(Texture::ID id, const std::string& fileName);

	sf::Texture& get(Texture::ID id);
	const sf::Texture& get(Texture::ID id) const;

private:
	std::map<Texture::ID, std::unique_ptr<sf::Texture>>		mTextures;

};

#endif