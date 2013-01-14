/*
 * main.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include <iostream>
#include "Server.hpp"


int main(int n_param, char** l_param){
	try{
		tesina_rc::Server myserver;
		myserver.Run();
	}catch(const char* err){
		std::cerr << err << "\n\n";
		return -1;
	}
	return 0;
}
