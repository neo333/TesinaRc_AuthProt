/*
 * Server.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include <iostream>
#include <string>
#include <queue>
#include "Server.hpp"
#include "AppProtServer.hpp"

namespace tesina_rc {

static const unsigned int MAX_CLIENT_ACTIVE = 10;
static const Uint16 PORT_NUMBER_DEFAULT=9999;
static const unsigned int MAX_BYTES_BUFFER = 1024;

Server::Server():port_number(PORT_NUMBER_DEFAULT),server(NULL),count_id(1){

}

Server::~Server() {
	this->CloseConnection();
}

void Server::Run(const Uint16 port_num) throw(const char*){
	std::cout << "Inizializzazione server ...\n";
	this->OpenConnection(port_num);
	std::cout << "\tOperazione conclusa con successo!\n"
			"\tPorta Aperta: "<<this->port_number<<"\n\n";
	while(true){
		this->AcceptNuovoClient();
		this->UpData_Connections();
		SDL_Delay(100);
	}
}

void Server::OpenConnection(const Uint16 port_num) throw(const char*){
	if(port_num!=0) this->port_number=port_num;
	if(SDLNet_ResolveHost(&this->indirizzo_ip,NULL,this->port_number)!=0){
		throw SDLNet_GetError();
	}

	Data_scriptClient _new(0,NULL,NULL);
	_new.connessione=SDLNet_TCP_Open(&this->indirizzo_ip);
	this->connessioni[0]=_new;
	this->server=this->connessioni[0].connessione;
	if(this->server==NULL){
		throw SDLNet_GetError();
	}
}

void Server::CloseConnection(void) {
	IteratoreLista it;
	TCPsocket curr;
	for(it=this->connessioni.begin(); it!=this->connessioni.end(); it++){
		if((*it).first!=0){
			curr=(*it).second.connessione;
			SDLNet_TCP_Close(curr);
			curr=NULL;
			(*it).second.exit=true;
			SDL_WaitThread((*it).second.id_thread,NULL);
			this->connessioni.erase(it);
		}
	}
	SDLNet_TCP_Close(this->server);
	this->server=NULL;
}

void Server::AcceptNuovoClient(void) throw(const char*){
	TCPsocket new_client=SDLNet_TCP_Accept(this->server);
	if(new_client==NULL){
		return;
	}
	if(this->connessioni.size()>MAX_CLIENT_ACTIVE){
		//busy
		const char mess_busy[]="BUSY";
		SDLNet_TCP_Send(new_client,mess_busy,strlen(mess_busy));
		SDLNet_TCP_Close(new_client);
		new_client=NULL;
	}else{
		//TODO: rivedere l'algoritmo!
		Data_scriptClient _temp(this->count_id,NULL,new_client);
		this->connessioni.insert(std::pair<ID_Host,Data_scriptClient>(this->count_id,_temp));
		this->connessioni[this->count_id].id_thread=SDL_CreateThread(Server::script_client, &this->connessioni[this->count_id]);
#if _DEBUG
		const char mess_enter[]="Welcome! Server TesinaRC al suo servizio!\r\n";
		SDLNet_TCP_Send(new_client,mess_enter,strlen(mess_enter));
#endif
	}
}

void Server::UpData_Connections(void){
	IteratoreLista it;
	Data_scriptClient* curr;
	for(it=this->connessioni.begin(); it!=this->connessioni.end(); ){
		curr=&(*it).second;
		if(curr->id_host!=0 && curr->exit==true){
			SDL_WaitThread(curr->id_thread,NULL);
			SDLNet_TCP_Close(curr->connessione);
			curr->connessione=NULL;
			it=this->connessioni.erase(it);
		}else{
			it++;
		}
	}
}

int Server::script_client(void* data) throw(const char*){
	Data_scriptClient* _data_cast=static_cast<Data_scriptClient*>(data);
	if(_data_cast==NULL){
		throw "Impossibile eseguire il casting dei dati nello script client!";
	}
	int bytes_rcv;
	char buffer[MAX_BYTES_BUFFER];
	std::string data_store_rcv;
	std::string data_store_snd;
	int result;
	AppProtServer client_prot;
	while(_data_cast->exit==false){
		bytes_rcv=SDLNet_TCP_Recv(_data_cast->connessione, buffer, MAX_BYTES_BUFFER);
		if(bytes_rcv<=0){
			_data_cast->exit=true;
		}else{
			data_store_rcv.append(buffer,bytes_rcv);
			result=client_prot.ElaboraRichiesta(data_store_rcv,data_store_snd);
			if(result){
				int bytes_snd=SDLNet_TCP_Send(_data_cast->connessione, data_store_snd.data(), data_store_snd.size());
				if(bytes_snd!=data_store_snd.size()){
					_data_cast->exit=true;
				}
			}
			if(result==-1){
				_data_cast->exit=true;
			}
		}
	}
	return 0;
}

}
