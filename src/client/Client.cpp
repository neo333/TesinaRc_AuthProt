#include "Client.hpp"
#include <string>

namespace tesina_rc{

void Client::Run(){
	this->OpenConnection();
	std::string buffer_snd;
	buffer_snd.append(1,0x20);
	buffer_snd.append(1,0x0);
	buffer_snd.append(1,0x0);
	buffer_snd.append(1,0x0);
	buffer_snd.append(1,0x1);
	SDLNet_TCP_Send(this->connessione,buffer_snd.data(),buffer_snd.size());
	this->CloseConnection();
}

void Client::OpenConnection(){
	SDLNet_ResolveHost(&this->indirizzo_ip,"127.0.0.1",9999);
	this->connessione=SDLNet_TCP_Open(&this->indirizzo_ip);
	if(!this->connessione){
		throw SDLNet_GetError();
	}
}
void Client::CloseConnection(){
	SDLNet_TCP_Close(this->connessione);
	this->connessione=NULL;
}

}