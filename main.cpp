#include <ctime>
#include <iostream>
#include "renderer.h"
#include "player.h"

int main()
{
    uint lastTime = 0;
    uint lastFPSDisplay = 0;
    int fps = 60;

    Map map;
	Player* player = new Player(&map);
    Renderer* renderer = new Renderer(player, &map);

    if(!renderer->init_sdl("Thanksgiving Party", 1280, 720))
        return 0;
              
    while (!player->wants_to_quit)
    {
        float dt = (SDL_GetTicks() - lastTime) / 1000.0;
        lastTime = SDL_GetTicks();
        if(dt == 0) dt = 1; //prevent division by zero

        if(lastFPSDisplay + 200 < lastTime)
        {
            lastFPSDisplay = lastTime;
            fps = 1 / dt;
            std::cout<<fps<<" FPS"<<std::endl;

            //sort sprites only 5 times per seconds
            map.sort_sprites(player->get_x(), player->get_y());
            map.animate_sprites();

            if(map.pickup_keys())
                player->key_count++;

            if(!player->pause)
                player->health -= map.damage_player();
        }

        if(!player->pause && !player->game_over && player->key_count > 0 && map.update_doors(player->get_x(), player->get_y(), dt))
            player->key_count--;
        
        if(!player->pause)
            map.update_ai(player->get_x(), player->get_y());

        player->handle_events(dt);
        renderer->draw();
    }

    if(player->health < 1)
        std::cout<<std::endl<<"GAME OVER"<<std::endl<<std::endl;
    
    delete player;
    delete renderer;
    return 0;
}

