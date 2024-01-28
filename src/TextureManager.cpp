#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

//Loading textures repeatedly in SFML causes lots of issues, therefore this class exists

sf::Texture* TextureManager::getTexture(std::string textureName) {
    auto result = textures.find(textureName);
    if(result == textures.end()) {
        // If texture does not already exist in the map, it's retrieved
        sf::Texture newTexture;
        textures.emplace(textureName, newTexture);
        textures[textureName].loadFromFile("files/images/" + textureName + ".png");
        return &textures[textureName];
    }
    else {
        //If texture already exists, returned
        return &result->second;
    }

}
