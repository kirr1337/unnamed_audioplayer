//
// Created by cheat on 27.10.2026.
//

#include "Utils.h"

#include <iomanip>
#include "sstream"

std::string Utils::formatTime(sf::Time time) {
    // Getting current amount seconds
    int totalSeconds = static_cast<int>(time.asSeconds());

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes
    << ":"
    << std::setfill('0') << std::setw(2) << seconds;

    return oss.str();
}

/**
 *
 * @param sprites
 * @param textures
 * @param sizeX
 * @param sizeY
 * @param startX
 *
 * this function need for initialization icons, obviously...
 */

void Utils::initIcons(std::vector<FallingStars> &sprites, const sf::Sprite &sprite) {
    for (size_t i = 0; i < sprites.size(); ++i) {

        sprites.at(i).sprite = sprite;
        sprites.at(i).speed = 80.0f + std::rand() % 120;
        sprites.at(i).swayAmp = 20.0f + std::rand() % 40;

        sprites.at(i).sprite.setScale(0.5f, 0.5f);
    }
}

void Utils::restartIcons(std::vector<FallingStars> &sprites, float x, float y) {
    for (size_t i = 0; i < sprites.size(); ++i) {

        x = static_cast<float>(i) * 70.0f;

        sprites.at(i).sprite.setPosition(x, y);

        if (sprites.at(i).sprite.getPosition().x > 830.0f) {
            y -= 50;
        }
    }
}

void Utils::doIconMovement(std::vector<FallingStars> &sprites, sf::Time deltaTime) {
    for (size_t i = 0; i < sprites.size(); i++) {
        sprites.at(i).sprite.move(0.0f, sprites.at(i).speed * deltaTime.asSeconds());
        sprites.at(i).sprite.rotate(sprites.at(i).swayAmp * deltaTime.asSeconds());

        if (sprites.at(i).sprite.getPosition().y > 650.0f) {
            const float newX = i * 80.0f;
            sprites.at(i).sprite.setPosition(newX, -30);
        }
    }
}
