/*
 * Server.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include <iostream>
#include "Server.hpp"

namespace tesina_rc {

static unsigned int MAX_CLIENT_ACTIVE = 10;
static Uint16 PORT_NUMBER_DEFAULT=9999;


Server::Server():port_number(PORT_NUMBER_DEFAULT),listner(NULL),server(NULL),count_id(1){

}

Server::~Server() {
	this->CloseConnection();
}

void Server::Run(const Uint16 port_num) throw(const char*){
	std::cout << "Inizializzazione server ...\n";
	this->OpenConnection(port_num);
	std::cout << "\tOperazione conclusa con successo!\n"
			"\tPorta Aperta: "<<this->port_number<<"\n\n";
	int result;
	while(true){
		result=SDLNet_CheckSockets(this->listner,10);
		if(result==-1){
			throw SDLNet_GetError();
		}else{
			if(result>0){
#ifdef _DEBUG
				std::cout << "@DEBUG: Attivita' in corso!\n";
#endif
				if(this->IsReadyHost(0)){
					std::cout << "Nuova connessione in arrivo ...\n\n";
					this->AcceptNuovoClient();
				}
			}
		}
		SDL_Delay(100);
	}
}

void Server::OpenConnection(const Uint16 port_num) throw(const char*){
	if(this->Is_ServerActive()==true){
		throw "Server già attivo!";
	}
	if(port_num!=0) this->port_number=port_num;
	if(SDLNet_ResolveHost(&this->indirizzo_ip,NULL,this->port_number)!=0){
		throw SDLNet_GetError();
	}

	this->connessioni[0]=SDLNet_TCP_Open(&this->indirizzo_ip);
	this->server=this->connessioni[0];
	if(this->server==NULL){
		throw SDLNet_GetError();
	}

	this->listner=SDLNet_AllocSocketSet(MAX_CLIENT_ACTIVE+1);
	if(this->listner==NULL){
		throw SDLNet_GetError();
	}

	this->AddTCPSocket_ToListner(this->server);
}

void Server::CloseConnection(void) {
	if(this->Is_ServerActive()==true){
		IteratoreConnessioni it;
		TCPsocket curr;
		for(it=this->connessioni.begin(); it!=this->connessioni.end(); it++){
			if((*it).first!=0){
				curr=(*it).second;
				SDLNet_TCP_Close(curr);
				curr=NULL;
				this->connessioni.erase(it);
			}
		}
		SDLNet_TCP_Close(this->server);
		this->server=NULL;
		SDLNet_FreeSocketSet(this->listner);
		this->listner=NULL;
	}
}

void Server::AddTCPSocket_ToListner(TCPsocket socket) throw(const char*){
	if(this->Is_ServerActive()==false){
		throw "Impossibile inserire nel lister una socket a server non attivo!";
	}
	if(socket==NULL){
		throw "Impossibile inserire nel listner una socket nulla!";
	}
	if(SDLNet_TCP_AddSocket(this->listner, socket)==-1){
		throw SDLNet_GetError();
	}
}

void Server::AcceptNuovoClient(void) throw(const char*){
	if(this->Is_ServerActive()==false){
		throw "Impossibile accettare nuova connessioni con server non attivo!";
	}
	TCPsocket new_client=SDLNet_TCP_Accept(this->server);
	if(new_client==NULL){
		throw SDLNet_GetError();
	}
	if(this->connessioni.size()>MAX_CLIENT_ACTIVE){
		//busy
		const char mess_busy[]="BUSY";
		SDLNet_TCP_Send(new_client,mess_busy,strlen(mess_busy));
		SDLNet_TCP_Close(new_client);
		new_client=NULL;
	}else{
		this->connessioni.insert(std::pair<ID_Host,TCPsocket>(this->count_id,new_client));
		this->AddTCPSocket_ToListner(new_client);
		this->count_id++;

#if _DEBUG
		const char mess_enter[]="Welcome! Server TesinaRC al suo servizio!\r\n";
		SDLNet_TCP_Send(new_client,mess_enter,strlen(mess_enter));
#endif
		SDL_CreateThread(Server::script_client, void *data);
	}
}

}
