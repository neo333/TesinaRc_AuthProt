/*
 * main.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */


#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <iostream>
#include "Client.hpp"

int main(int n_param, char** l_param){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	try{
		tesina_rc::Client app;
		app.Run();
	}catch(const char* err){
		std::cerr << err << "\n\n";
	}

	SDLNet_Quit();
	SDL_Quit();

#ifdef WIN32
	system("pause");
#endif
	return 0;
}
