/*
 * main.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include <iostream>
#include "Server.hpp"

int main(int n_param, char** l_param){
	SDL_Init(SDL_INIT_EVENTTHREAD);
	SDLNet_Init();
	try{
		tesina_rc::Server myserver;
		myserver.Run();
	}catch(const char* err){
		std::cerr << err << "\n\n";
		return -1;
	}
	SDLNet_Quit();
	SDL_Quit();

#ifdef WIN32
	system("pause");
#endif
	return 0;
}
