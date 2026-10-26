#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui-SFML.h"
#include "imgui.h"
#include "portable-file-dialogs.h"

int main() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoResize;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello!", sf::Style::Close); // Window
    sf::Font font;
    sf::Text text("next song", font, 32);
    sf::Music music;
    std::string currentMusicPath;
    sf::Texture vynyl_texture;

    vynyl_texture.loadFromFile("resources/icons/vynyl.png");
    vynyl_texture.setSmooth(true);
    sf::Sprite vynyl_sprite(vynyl_texture);

    window.setFramerateLimit(60);

    font.loadFromFile("resources/fonts/Samson.ttf");

    text.setColor(sf::Color::Black);
    text.setPosition(320, 250);

    vynyl_sprite.setPosition(700, 500);
    vynyl_sprite.setScale(0.07f, 0.07f);


    if (!ImGui::SFML::Init(window)) {
        std::printf("Error!\nCannot init window");
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
                std::cout << "Bye";
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        ImGui::SetNextWindowPos(ImVec2(30, 360));
        ImGui::SetNextWindowSize(ImVec2(222, 200));
        ImGui::Begin("Test", nullptr, window_flags);

        if (ImGui::Button("<<", ImVec2(50, 50))) {
            std::cout << "<<\n";
        }

        ImGui::SameLine();
        ImGui::SetCursorPosY(70);
        ImGui::SetCursorPosX(40);

        if (ImGui::Button("Play", ImVec2(150, 50))) {
            if (!music.openFromFile(currentMusicPath))
                text.setString("Please, select song");

           // music.setVolume(10);

            music.play();
            std::cout << "the music has been played!\n";
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

           // text.setString(currentMusicPath);
        }

        // Ending the ImGui context
        ImGui::End();

        // Every frame, spinning the vynyl picture
        if (music.getStatus() == sf::Music::Playing) {
            vynyl_sprite.rotate(90.0f * dt.asSeconds());
        }

        window.clear(sf::Color::White);

        // Drawing
        window.draw(text);
        window.draw(vynyl_sprite);

        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown the ImGui context
    ImGui::SFML::Shutdown();
    return 0;
}
