/*
 * main.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */


#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include "Client.hpp"

int main(int n_param, char** l_param){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	tesina_rc::Client myclient;
	myclient.Run();

	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
