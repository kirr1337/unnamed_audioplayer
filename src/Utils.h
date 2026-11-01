//
// Created by cheat on 27.10.2026.
//

#ifndef SFML_TEST_UTILS_H
#define SFML_TEST_UTILS_H

#include <vector>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include "sol/sol.hpp"


namespace sf {
    class Texture;
    class Sprite;
}

struct FallingStars {
    sf::Sprite sprite;
    float speed{};
    float swayAmp{};
};

class Utils {
public:

    static std::string formatTime(sf::Time time);
    static void initIcons(std::vector<FallingStars>& sprites, const sf::Sprite& sprite, sol::state& lua);
    static void restartIcons(std::vector<FallingStars>& sprites, float x, float y);
    static void doIconMovement(std::vector<FallingStars>& sprites, sf::Time deltaTime);
};


#endif //SFML_TEST_UTILS_H