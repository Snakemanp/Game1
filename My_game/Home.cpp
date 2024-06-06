#ifndef _Home_
#define _Home_

#include <SFML/Graphics.hpp>
#include "game.cpp"
#include <imgui.h>
#include <imgui-SFML.h>

class Home {
    sf::RenderWindow& window;
    bool isgamerunning = false;
    bool issettings = false;
    std::shared_ptr<game> Game = nullptr;
    long int frames = 0, lastenemy = 0;
    ImVec4 playercolor = ImVec4(sf::Color::Blue.r / 255.0f, sf::Color::Blue.g / 255.0f, sf::Color::Blue.b / 255.0f, 1.0f);
    float player_vel = 5;
    float bullet_vel = 10;
    sf::Sprite backgroundsprite;
    bool aswd = true;
    int e_min = 3, e_range = 5;
    float spawn_time = 1;
    ImFont* font1 = nullptr;
    ImFont* font2 = nullptr;
    sf::Texture fontTexture;

public:
    Home(sf::RenderWindow& window_) : window(window_) {
        ImGui::SFML::Init(window);

        // Init
        ImGuiIO& io = ImGui::GetIO();
        font1 = io.Fonts->AddFontFromFileTTF("Wonderbar-pALD.ttf",80);
        font2= io.Fonts->AddFontFromFileTTF("Wonderbar-pALD.ttf", 20);
        if(font1&&font2) {
            unsigned char* tex_pixels = nullptr;
            int tex_width, tex_height;
            io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

            // Upload texture to graphics system
            fontTexture.create(tex_width, tex_height);
            fontTexture.update(tex_pixels);

            // Store our identifier
            io.Fonts->TexID = (void*)fontTexture.getNativeHandle();

            // Clear texture data
            io.Fonts->ClearTexData();
        }
    }

    ~Home() {
        ImGui::SFML::Shutdown();
    }

    void update() {
        if (font2)
        {
            ImGui::PushFont(font2);
        }
        if (!isgamerunning && !issettings) {
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) window.close();
                }
            }
            ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set the position of the child window
            ImGuiWindowFlags childWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

            ImGui::Begin("Menu", nullptr, childWindowFlags);
            ImGui::SetWindowSize(ImVec2(window.getSize().x, window.getSize().y));
            if (font1) {
                ImGui::PushFont(font1);
            }
            ImGui::SetCursorPos(ImVec2(window.getSize().x * 0.5 - ImGui::CalcTextSize("My Game 1").x/2, window.getSize().y * 0.2));
            ImGui::Text("My Game 1");
            if (font1) {
                ImGui::PopFont();
            }
            ImGui::SetCursorPos(ImVec2(window.getSize().x * 0.2, window.getSize().y * 0.4));
            if (ImGui::Button("New Game", ImVec2(200, 50))) {
                isgamerunning = true;
                startGame();
            }
            ImGui::SetCursorPos(ImVec2(window.getSize().x * 0.2, ImGui::GetCursorPosY()));
            if (ImGui::Button("Settings", ImVec2(200, 50))) {
                issettings = true;
            }
            ImGui::End();
        }
        if (isgamerunning) {
            updateGame();
            if (Game->close) {
                Game.reset();
                Game = nullptr;
                isgamerunning = false;
            }
        }
        if (issettings) {
            settings();
        }
        ImGui::PopFont();
    }

    void render() {
        if (isgamerunning) {
            renderGame();
        }
        ImGui::SFML::Render(window);
    }

private:
    void startGame() {
        Game = std::make_shared<game>(window);
        Game->player_color = sf::Color(playercolor.x * 255, playercolor.y * 255, playercolor.z * 255, 255);
        Game->player_vel = player_vel;
        Game->bullet_vel = bullet_vel;
        if (!aswd) {
            Game->up = sf::Keyboard::Up;
            Game->down = sf::Keyboard::Down;
            Game->left = sf::Keyboard::Left;
            Game->right = sf::Keyboard::Right;
        }
        Game->enemy_min = e_min;
        Game->enemyrange = e_range;
        Game->spawn_player();
        frames = 0;
        lastenemy = 60;
    }

    void updateGame() {
        Game->input();
        if (!Game->ispaused && !Game->end) {
            Game->playermovement();
            if ((frames - lastenemy) > spawn_time * 30) {
                Game->spawn_enemy();
                lastenemy = frames;
            }
            Game->update();
        }
        frames++;
    }

    void renderGame() {
        window.clear(sf::Color::Black);
        Game->display();
    }

    void settings() {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) issettings = false;
            }
        }
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGuiWindowFlags childWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin("Settings", nullptr, childWindowFlags);
        ImGui::SetWindowSize(ImVec2(window.getSize().x, window.getSize().y));
        ImGui::ColorEdit3("Set Player Color", (float*)&playercolor);
        ImGui::SliderFloat("Set Player Speed", &player_vel, 5.0f, 10.0f);
        ImGui::SliderFloat("Set Bullets Speed", &bullet_vel, 10.0f, 20.0f);
        ImGui::SliderFloat("Set Enemy Spawn Time(s)", &spawn_time, 0.5f, 2.0f);
        ImGui::Text("Player Controls:");
        if (ImGui::RadioButton("ASWD", aswd)) {
            aswd = true;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Arrow Keys", !aswd)) {
            aswd = false;
        }
        ImGui::SliderInt("Set Enemy Minspeed", &e_min, 2, 5);
        ImGui::SliderInt("Set Enemy Speedrange", &e_range, 5, 10);
        ImGui::End();
    }
};

#endif // !_Home_
