#ifndef game_class
#define game_class
#include <SFML/Graphics.hpp>
#include "entitymanager.h"
#include"physics.h"
#include"vec2.h"
#include<memory>
#include<ctime>
class game {
    sf::RenderWindow& window;
    EntityManager Manager;
    user_input uinput;
    std::shared_ptr<Entity> player;
public:
    bool pause = false;
    bool end = false;
    vec2 win;
    game(sf::RenderWindow& window_) : window(window_),win(window_.getSize().x,window_.getSize().y){
        srand(time(0));
    }
    void display() {
        entity_list& entities = Manager.get_entities();
        for (int i = 0; i < entities.size(); i++) {
            window.draw(*(entities[i]->shape));
        }
    }
    void spawn_player() {
        std::shared_ptr<Entity> temp=Manager.add_entity("Player");
        temp->obj_trans = std::make_shared<objtrans>(vec2(0, 0), vec2(0, 0), 5.0f, 0);
        temp->shape = std::make_shared<sf::CircleShape>(30.0f,8);
        temp->shape->setFillColor(sf::Color::Black);
        temp->shape->setOutlineThickness(10.0f);
        temp->shape->setOutlineColor(sf::Color::Blue);
        temp->shape->setOrigin(30.0f,30.0f);
        temp->obj_trans->change_pos(vec2(window.getSize().x / 2, window.getSize().y / 2));
        player = temp;
    }
    void spawn_bullet() {
        vec2 temppos = uinput.shootpos;
        std::shared_ptr<Entity> temp = Manager.add_entity("Bullet");
        temp->shape = std::make_shared<sf::CircleShape>(8.0);
        temppos = temppos - player->obj_trans->pos;
        temppos=temppos.normalize();
        uinput.shootpos = player->obj_trans->pos + temppos * 40;
        temp->obj_trans = std::make_shared<objtrans>(uinput.shootpos, temppos * 8);
        temp->life = std::make_shared<lifespan>(45);
    }
    void create_enemy(int sides) {
        vec2 temppos,tempvel;
        int rad = 20 + rand() % 30;
        temppos.x = (rand() % (window.getSize().x - 2 * rad)) + rad;
        temppos.y = (rand() % (window.getSize().y - 2 * rad)) + rad;
        while (player->obj_trans->pos.obj_dist(temppos) < 500.0) {
            temppos.x = (rand() % (window.getSize().x - 2 * rad)) + rad;
            temppos.y = (rand() % (window.getSize().y - 2 * rad)) + rad;
        }
        tempvel.x = 2+rand() % 10;
        tempvel.y = 2+rand() % 10;
        tempvel = tempvel.normalize();
        std::shared_ptr<Entity> temp = Manager.add_entity("Enemy");
        temp->shape = std::make_shared<sf::CircleShape>(rad, sides);
        temp->shape->setFillColor(sf::Color(rand()%255,rand()%255,rand()%255,255));
        temp->obj_trans = std::make_shared<objtrans>(temppos,tempvel*(3+rand()%5), 3+rand() % 5);
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
            if (tempcol.a <= 0) Manager.del_entity(bullets[i]);
        }
        entity_list& enemies = Manager.get_entities("Enemy");
        for (int i = 0; i < enemies.size(); i++) {
            physics::bounce(*enemies[i], win.x, win.y);
        }
        entity_list& dead = Manager.get_entities("dead");
        for (int i = 0; i < dead.size(); i++) {
            tempcol = dead[i]->shape->getFillColor();
            tempcol.a = dead[i]->life->getalpha();
            dead[i]->shape->setFillColor(tempcol);
            dead[i]->shape->setOutlineColor(sf::Color(255, 255, 255, tempcol.a));
            dead[i]->life->update();
            if (tempcol.a <= 0) Manager.del_entity(dead[i]);
            physics::bounce(*dead[i], win.x, win.y);
        }
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < enemies.size(); j++) {
                if (bullets[i]->alive == false) continue;
                if (enemies[j]->alive == false) continue;
                if (physics::collision(*bullets[i], *enemies[j])&&enemies[j]->alive) {
                    player->score ++;
                    enemies[j]->alive = false;
                    bullets[i]->alive = false;
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
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    uinput.shoot = true;
                    uinput.shootpos.x = event.mouseButton.x;
                    uinput.shootpos.y = event.mouseButton.y;
                    spawn_bullet();
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A ) uinput.A = true;
                if (event.key.code == sf::Keyboard::D ) uinput.D = true;
                if (event.key.code == sf::Keyboard::W ) uinput.W = true;
                if (event.key.code == sf::Keyboard::S ) uinput.S = true;
                if (event.key.code == sf::Keyboard::P ) pause = !pause;
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::A ) uinput.A = false;
                if (event.key.code == sf::Keyboard::D ) uinput.D = false;
                if (event.key.code == sf::Keyboard::W ) uinput.W = false;
                if (event.key.code == sf::Keyboard::S ) uinput.S = false;
            }
        }
    }
    void playermovement() {
        vec2 temp;
        if (uinput.A == true) temp.x -= 1;
        if (uinput.S == true) temp.y += 1;
        if (uinput.W == true) temp.y -= 1;
        if (uinput.D == true) temp.x += 1;
        temp = temp.normalize();
        temp = temp * 3;
        player->obj_trans->change_vel(temp);
        physics::clip(*player, win.x, win.y);
    }
    int get_player_score() {
        return(player->score);
    }
};

#endif // !game_class
