#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui-SFML.h"
#include "../portable-file-dialogs.h"
#include "Utils.h"
#include "imgui.h"

int main() {
    constexpr int screen_Width = 800;
    constexpr int screen_Height = 600;

    static int seekCooldown = 0.0f;

    bool isLooped = false;

    int x = -70;

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

    sf::Sprite star_sprite(star_Texture);

    std::string currentMusicPath;
    std::string isLoopedStr;

    window.setFramerateLimit(60);

    font.loadFromFile("resources/fonts/Samson.ttf");

    text.setColor(sf::Color::Black);
    text.setPosition(screen_Width / 2 - 200, screen_Height / 2 - 50);

    vynyl_sprite.setPosition(700, 500);
    vynyl_sprite.setScale(0.07f, 0.07f);

    std::vector<FallingStars> star_sprites(15);

    Utils::initIcons(star_sprites, star_sprite);

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

        float currentSec = music.getPlayingOffset().asSeconds();
        float totalSec = music.getDuration().asSeconds();

        std::string timeLabel = Utils::formatTime(sf::seconds(currentSec)) + " / " + Utils::formatTime(sf::seconds(totalSec));

        // Check events
        while (window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
                std::cout << "The window closed";
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        ImGui::SetNextWindowPos(ImVec2(15, 390));
        ImGui::SetNextWindowSize(ImVec2(450, 200));
        ImGui::Begin("Controller", nullptr, window_flags);

        if (ImGui::Button("<<", ImVec2(50, 50))) {
            sf::Time currentTime = music.getPlayingOffset();
            music.setPlayingOffset(currentTime - sf::seconds(10));
            if (currentTime <= sf::seconds(10)) {
                music.setPlayingOffset(sf::seconds(0));
            }
            std::cout << "<<\n";
        }

        ImGui::SameLine();
        ImGui::SetCursorPosY(70);
        ImGui::SetCursorPosX(40);

        if (ImGui::Button("Play", ImVec2(150, 50))) {
            Utils::restartIcons(star_sprites, x, -30);

            text.setColor(sf::Color::Black);

            // music.setVolume(10);

            music.play();
            std::cout << "the music has been played!\n";
            std::cout << totalSec << "\n" << currentSec;
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(160);

        if (ImGui::Button(">>", ImVec2(50, 50))) {
            sf::Time currentTime = music.getPlayingOffset();
            music.setPlayingOffset(currentTime + sf::seconds(10));
            std::cout << ">>\n";
        }

        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(15, 130));

        // loading custom music from disk
        if (ImGui::Button("load music", ImVec2(100, 50))) {
            // calling the OS explorer
            auto selection = pfd::open_file("Select song", ".",{
            "Audio files", "*.mp3;" "*.wav;" "*.ogg", "All files", "*"
            }).result();

            // if selection not empty, then current music path = what the user has chosen
            if (!selection.empty()) {
                currentMusicPath = selection[0];
                music.openFromFile(currentMusicPath);
            }
            if (selection.empty()) {
                text.setString("Please..\nSelect the song");
            }
            else {
                std::filesystem::path p(currentMusicPath);
                text.setString(sf::String(p.filename().string()));
            }
        }

        ImGui::SetCursorPos(ImVec2(140, 130));

        if (ImGui::Button("pause", ImVec2(50, 50))) {
            music.pause();
        }

        ImGui::SetCursorPos(ImVec2(380, 8));

        if (ImGui::Button("loop", ImVec2(50, 50))) {
            std::cout << "\nLooped\n";
            isLooped = !isLooped;
            music.setLoop(isLooped);
            std::cout << isLooped;
        }

        if (seekCooldown > 0.0f) {
            seekCooldown -= dt.asSeconds();
        }

        ImGui::SetCursorPos(ImVec2(230, 110));
        ImGui::SetNextItemWidth(210.0f);

        if (ImGui::SliderFloat("##progress", &currentSec, 0.0f, totalSec, "%.1f")) {
            currentSec = std::clamp(currentSec, 0.0f, totalSec);

            music.setPlayingOffset(sf::seconds(currentSec));

            seekCooldown = 0.2f;
        }

        if (currentSec > totalSec) {
            music.setPlayingOffset(sf::seconds(0));
        }

        if (!ImGui::IsItemActive() && seekCooldown <= 0.0f) {
            currentSec = music.getPlayingOffset().asSeconds();
        }

        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(230, 140));
        ImGui::Text("total: %s", timeLabel.c_str());

        ImGui::SetCursorPos(ImVec2(230, 30));
        ImGui::Text("Looped = %s", isLooped ? "true" : "false");

        // Ending the ImGui context
        ImGui::End();

        // Every frame, we're spinning the vynyl picture and moving stars
        if (music.getStatus() == sf::Music::Playing) {
            vynyl_sprite.rotate(90.0f * dt.asSeconds());
            Utils::doIconMovement(star_sprites, dt);
        }

        if (music.getStatus() != sf::Music::Playing) {
            for (auto& stars : star_sprites) {
                stars.sprite.move(-100 * dt.asSeconds(), 0);
            }
        }

        window.clear(sf::Color::White);

        // Drawing
        window.draw(text);
        window.draw(vynyl_sprite);
        for (auto& star : star_sprites) {
            window.draw(star.sprite);
        }

        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown the ImGui context
    ImGui::SFML::Shutdown();
    return 0;
}
