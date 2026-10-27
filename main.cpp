#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui-SFML.h"
#include "imgui.h"
#include "portable-file-dialogs.h"


int main() {
    constexpr int screen_Width = 800;
    constexpr int screen_Height = 600;

    int x = -70, y = 0;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoResize;

    sf::RenderWindow window(sf::VideoMode(screen_Width, screen_Height), "Unnamed_AudioPlayer", sf::Style::Close); // Window
    sf::Font font;
    sf::Text text("next song", font, 32);
    sf::Music music;

    sf::Texture vynyl_texture;
    vynyl_texture.loadFromFile("resources/icons/vynyl.png");
    vynyl_texture.setSmooth(true);
    sf::Sprite vynyl_sprite(vynyl_texture);

    sf::Texture star_Texture;
    star_Texture.loadFromFile("resources/icons/star.png");
    star_Texture.setSmooth(true);

    std::string currentMusicPath;

    window.setFramerateLimit(60);

    font.loadFromFile("resources/fonts/Samson.ttf");

    text.setColor(sf::Color::Black);
    text.setPosition(screen_Width / 2 - 50, screen_Height / 2 - 50);

    vynyl_sprite.setPosition(700, 500);
    vynyl_sprite.setScale(0.07f, 0.07f);

    std::vector<sf::Sprite> star_sprites(10);

    for (int i = 0; i < star_sprites.size(); i++) {
        star_sprites[i].setTexture(star_Texture);
        star_sprites[i].setScale(0.5f, 0.5f);

        y = i * 80.f;

        star_sprites[i].setPosition(x, y);
    }


    if (!ImGui::SFML::Init(window)) {
        std::printf( "Error!\nCannot init window");
        return -1; // If cannot init window
    }

    sf::FloatRect bounds = vynyl_sprite.getLocalBounds();
    vynyl_sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    sf::Clock deltaClock;

    // Main loop
    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        sf::Event event;

        // Check events
        while (window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
                std::cout << "The window closed";
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        ImGui::SetNextWindowPos(ImVec2(30, 360));
        ImGui::SetNextWindowSize(ImVec2(222, 200));
        ImGui::Begin("Controller", nullptr, window_flags);

        if (ImGui::Button("<<", ImVec2(50, 50))) {
            std::cout << "<<\n";
        }

        ImGui::SameLine();
        ImGui::SetCursorPosY(70);
        ImGui::SetCursorPosX(40);

        if (ImGui::Button("Play", ImVec2(150, 50))) {
            if (!music.openFromFile(currentMusicPath)) {
                text.setColor(sf::Color::Red);
                text.setString("Please, select song");
            }
            else {
                for (int i = 0; i < star_sprites.size(); i++) {
                    star_sprites[i].setTexture(star_Texture);
                    star_sprites[i].setScale(0.5f, 0.5f);

                    star_sprites[i].rotate(45);
                    y = i * 80.f;

                    star_sprites[i].setPosition(x, y);
                }

                text.setColor(sf::Color::Black);

                // music.setVolume(10);

                music.play();
                std::cout << "the music has been played!\n";
            }
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(165);

        if (ImGui::Button(">>", ImVec2(50, 50))) {
            std::cout << ">>\n";
        }

        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(15, 130));

        // loading custom music from disk
        if (ImGui::Button("load music", ImVec2(100, 50))) {
            // calling the OS explorer
            auto selection = pfd::open_file("Select song", ".",{
            "Audio files", "*.wav", "*.ogg", "All files", "*"
            }).result();

            // if selection not empty, then current music path = what the user has chosen
            if (!selection.empty()) {
                currentMusicPath = selection[0];
            }

            std::filesystem::path p(currentMusicPath);
            text.setString(sf::String(p.filename().string()));
        }

        // Ending the ImGui context
        ImGui::End();

        // Every frame, we're spinning the vynyl picture
        if (music.getStatus() == sf::Music::Playing) {
            vynyl_sprite.rotate(90.0f * dt.asSeconds());
            for (int i = 0; i < star_sprites.size(); i++) {
                star_sprites[i].move(100 * dt.asSeconds(), 0);
                star_sprites[i].rotate((std::rand() % 100 - 50) * dt.asSeconds());

                if (star_sprites[i].getPosition().x > 830.0f) {
                    float newY = i * 80.f;
                    star_sprites[i].setPosition(x, newY);
                }
            }
        }
        if (music.getStatus() != sf::Music::Playing) {
            for (auto& stars : star_sprites) {
                stars.move(0, 100 * dt.asSeconds());
            }
        }

        window.clear(sf::Color::White);

        // Drawing
        window.draw(text);
        window.draw(vynyl_sprite);
        for (auto& star : star_sprites) {
            window.draw(star);
        }

        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown the ImGui context
    ImGui::SFML::Shutdown();
    return 0;
}
