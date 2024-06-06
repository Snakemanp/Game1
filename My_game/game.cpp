#ifndef game_class
#define game_class
#include <SFML/Graphics.hpp>
#include "entitymanager.h"
#include"physics.h"
#include"vec2.h"
#include<memory>
#include<ctime>
#include<imgui-SFML.h>
#include<imgui.h>
#include <iostream>
class game {
    sf::RenderWindow& window;
    EntityManager Manager;
    user_input uinput;
    std::shared_ptr<Entity> player;
    sf::View view;
    sf::Text text;
    sf::Font font;
public:
    bool ispaused = false;
    bool end = false;
    bool close = false;
    sf::Keyboard::Key up = sf::Keyboard::W;
    sf::Keyboard::Key down = sf::Keyboard::S;
    sf::Keyboard::Key left = sf::Keyboard::A;
    sf::Keyboard::Key right = sf::Keyboard::D;
    sf::Keyboard::Key pause = sf::Keyboard::P;
    float player_vel=5;
    float bullet_vel = 8;
    sf::Color player_color = sf::Color::Blue;
    int enemy_min = 3, enemyrange = 5;
    int power_shoot = 0;

    game(sf::RenderWindow& window_) : window(window_){
        view = window.getView();
        srand(time(0));
        font.loadFromFile("Wonderbar-pALD.ttf");
        text.setFont(font);
    }
    void display() {
        std::string score = "Score " + std::to_string(player->score);
        text.setString(score);
        text.setCharacterSize(24);
        text.setFillColor(player_color);
        window.setView(view);
        entity_list& entities = Manager.get_entities();
        for (int i = 0; i < entities.size(); i++) {
            window.draw(*(entities[i]->shape));
        }
        window.draw(text);
    }
    void spawn_player() {
        std::shared_ptr<Entity> temp=Manager.add_entity("Player");
        temp->obj_trans = std::make_shared<objtrans>(vec2(0, 0), vec2(0, 0), 5.0f, 0);
        temp->shape = std::make_shared<sf::CircleShape>(30.0f,8);
        temp->shape->setFillColor(sf::Color::Black);
        temp->shape->setOutlineThickness(10.0f);
        temp->shape->setOutlineColor(player_color);
        temp->shape->setOrigin(30.0f,30.0f);
        temp->obj_trans->change_pos(vec2(view.getSize().x / 2, view.getSize().y / 2));
        player = temp;
    }
    void spawn_bullet() {
        vec2 temppos = uinput.shootpos;
        std::shared_ptr<Entity> temp = Manager.add_entity("Bullet");
        temp->shape = std::make_shared<sf::CircleShape>(8.0);
        temppos = temppos - player->obj_trans->pos;
        temppos=temppos.normalize();
        uinput.shootpos = player->obj_trans->pos + temppos * 40;
        temp->obj_trans = std::make_shared<objtrans>(uinput.shootpos, temppos * bullet_vel);
        temp->life = std::make_shared<lifespan>(45);
    }
    void special_shoot() {
        std::shared_ptr<Entity> temp;
        vec2 temppos;
        int no_bullets = 15;
        float angle = 360 / no_bullets;
        for (int i = 0; i < no_bullets; i++) {
            temp = Manager.add_entity("Bullet");
            temp->shape = std::make_shared<sf::CircleShape>(8.0);
            temppos = player->obj_trans->pos;
            temp->obj_trans = std::make_shared<objtrans>(temppos, vec2(angle*i)* bullet_vel);
            temp->life = std::make_shared<lifespan>(45);
        }
    }
    void create_enemy(int sides) {
        vec2 temppos,tempvel;
        int rad = 20 + rand() % 30;
        temppos.x = (rand() % int(view.getSize().x - 2 * rad)) + rad;
        temppos.y = (rand() % int(view.getSize().y - 2 * rad)) + rad;
        while (player->obj_trans->pos.obj_dist(temppos) < 500.0) {
            temppos.x = (rand() % int(view.getSize().x - 2 * rad)) + rad;
            temppos.y = (rand() % int(view.getSize().y - 2 * rad)) + rad;
        }
        tempvel.x = 2+rand() % 10;
        if (rand() % 2 == 1) tempvel.x *= -1;
        tempvel.y = 2+rand() % 10;
        if (rand() % 2 == 1) tempvel.y *= -1;
        tempvel = tempvel.normalize();
        std::shared_ptr<Entity> temp = Manager.add_entity("Enemy");
        temp->shape = std::make_shared<sf::CircleShape>(rad, sides);
        temp->shape->setFillColor(sf::Color(rand()%255,rand()%255,rand()%255,255));
        temp->obj_trans = std::make_shared<objtrans>(temppos,tempvel*(enemy_min+rand()%enemyrange), 3+rand() % 5);
        temp->shape->setOrigin(rad, rad);
        temp->score = sides;
        temp->shape->setOutlineThickness(3);
    }
    void spawn_enemy() {
        int sides = 3 + rand() % 7;
        create_enemy(sides);
    }


    std::shared_ptr<Entity> spawn_enemy(const vec2& pos, float radius, int sides) {
        std::shared_ptr<Entity> temp = Manager.add_entity("dead");
        temp->life = std::make_shared<lifespan>(45);
        temp->shape = std::make_shared<sf::CircleShape>(radius / 2, sides);
        temp->shape->setOrigin(radius/2, radius/2);
        temp->obj_trans = std::make_shared<objtrans>(pos);
        temp->shape->setOutlineThickness(3);
        return temp;
    }
    void remove_enemy(std::shared_ptr<Entity>& enemy) {
        vec2 pos = enemy->obj_trans->pos;
        float radius = enemy->shape->getRadius();
        float rspeed = enemy->obj_trans->rspeed;
        float vel = enemy->obj_trans->vel.length();
        int sides = enemy->score;
        sf::Color color = enemy->shape->getFillColor();
        std::shared_ptr<Entity> temp;
        for (int i = 0; i < sides; i++) {
            temp = spawn_enemy(pos,radius,sides);
            temp->alive = false;
            temp->obj_trans->rspeed = rspeed;
            temp->obj_trans->change_vel(vec2((360 / sides) * i)*vel);
            temp->shape->setFillColor(color);
        }
        Manager.del_entity(enemy);
    }
    void update() {
        entity_list& entities = Manager.get_entities();
        vec2 temp;
        sf::Color tempcol;
        Manager.update();
        for (int i = 0; i < entities.size(); i++) {
            temp = entities[i]->obj_trans->pos;
            entities[i]->obj_trans->upd_pos();
            entities[i]->obj_trans->upd_angle();
            entities[i]->shape->setPosition(temp.x,temp.y);
            entities[i]->shape->setRotation(entities[i]->obj_trans->angle);
        }
        entity_list& bullets = Manager.get_entities("Bullet");
        for (int i = 0; i < bullets.size(); i++) {
            tempcol = bullets[i]->shape->getFillColor();
            tempcol.a = bullets[i]->life->getalpha();
            bullets[i]->shape->setFillColor(tempcol);
            bullets[i]->life->update();
            if (tempcol.a <= 0) {
                Manager.del_entity(bullets[i]);
                power_shoot = 0;
            }
        }
        entity_list& enemies = Manager.get_entities("Enemy");
        for (int i = 0; i < enemies.size(); i++) {
            physics::bounce(*enemies[i], view.getSize().x, view.getSize().y);
        }
        entity_list& dead = Manager.get_entities("dead");
        for (int i = 0; i < dead.size(); i++) {
            tempcol = dead[i]->shape->getFillColor();
            tempcol.a = dead[i]->life->getalpha();
            dead[i]->shape->setFillColor(tempcol);
            dead[i]->shape->setOutlineColor(sf::Color(255, 255, 255, tempcol.a));
            dead[i]->life->update();
            if (tempcol.a <= 0) Manager.del_entity(dead[i]);
            physics::bounce(*dead[i], view.getSize().x, view.getSize().y);
        }
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < enemies.size(); j++) {
                if (bullets[i]->alive == false) continue;
                if (enemies[j]->alive == false) continue;
                if (physics::collision(*bullets[i], *enemies[j])&&enemies[j]->alive) {
                    player->score ++;
                    enemies[j]->alive = false;
                    bullets[i]->alive = false;
                    if (power_shoot < 3) power_shoot++;
                }
            }
        }
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i]->alive == false) {
                Manager.del_entity(bullets[i]);
                i--;
            }
        }
        for (int i = 0; i < enemies.size(); i++) {
            if (enemies[i]->alive == false) {
                remove_enemy(enemies[i]);
                i--;
            }
        }
        for (int i = 0; i < enemies.size(); i++) {
            if (physics::collision(*player, *enemies[i])) {
                end = true;
            }
        }
    }

    void input() {
        sf::Event event;
        sf::Vector2f originalSize(1200, 800);
        while (window.pollEvent(event)) {
            // Process SFML events
            ImGui::SFML::ProcessEvent(event);

            // Close window if requested
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle mouse button pressed event
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    uinput.shoot = true;
                    uinput.shootpos.x = event.mouseButton.x;
                    uinput.shootpos.y = event.mouseButton.y;
                    spawn_bullet();
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (power_shoot == 3) {
                        special_shoot();
                        power_shoot = 0;
                    }
                }
            }

            // Handle key pressed event
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == left) uinput.A = true;
                if (event.key.code == right) uinput.D = true;
                if (event.key.code == up) uinput.W = true;
                if (event.key.code == down) uinput.S = true;
                if (event.key.code == pause) ispaused = !ispaused;
                if (event.key.code == sf::Keyboard::Escape) close = true;
            }

            // Handle key released event
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == left) uinput.A = false;
                if (event.key.code == right) uinput.D = false;
                if (event.key.code == up) uinput.W = false;
                if (event.key.code == down) uinput.S = false;
            }

            if (event.type == sf::Event::Resized)
            {   
                vec2 temp = (view.getCenter().x, view.getCenter().y);
                view.setSize(window.getSize().x,window.getSize().y);
                view.setCenter(view.getSize().x/2, view.getSize().y/2);
            }
        }

        // Handle ImGui input separately
        ImGuiIO& io = ImGui::GetIO();
        if (window.isOpen()) io.MousePos.x = sf::Mouse::getPosition().x;
        if (window.isOpen()) io.MousePos.y = sf::Mouse::getPosition().y;
        if (window.isOpen()) io.MouseDown[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }


    void playermovement() {
        vec2 temp;
        if (uinput.A == true) temp.x -= 1;
        if (uinput.S == true) temp.y += 1;
        if (uinput.W == true) temp.y -= 1;
        if (uinput.D == true) temp.x += 1;
        temp = temp.normalize();
        temp = temp * player_vel;
        player->obj_trans->change_vel(temp);
        physics::clip(*player, view.getSize().x, view.getSize().y);
    }
    int get_player_score() {
        return(player->score);
    }

};

#endif // !game_class
