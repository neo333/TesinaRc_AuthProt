/*
 * Client.cpp
 *
 *  Created on: 15/gen/2013
 *      Author: neo
 */

#include "Client.hpp"
#include <iostream>
#include <RSA/RSA.h>
#include <com_def.hpp>

namespace tesina_rc {

Client::Client():connessione(NULL){

}

void Client::Run() {
	std::cout << "Connessione ...\n\n";
	this->OpenConnection();

	//DEBUG
	std::cout << "Debug ...\n\n";
	std::string request;
	HeaderAuthProt head;
	int bytes;
	char buffer[1024];

	head.type_cmd=0x20;
	head.len_payload=0;
	AppendData_intoString(&head,sizeof(HeaderAuthProt),request);
	SDLNet_TCP_Send(this->connessione,request.c_str(),request.size());
	bytes=SDLNet_TCP_Recv(this->connessione,buffer,1024);
	request.clear();
	AppendData_intoString(buffer,bytes,request);
	PopData_fromString(&head,sizeof(HeaderAuthProt),request);
	if(head.type_cmd!=0x22){
		std::cerr << "errore!1\n";
	}
	std::string to_cryp;
	to_cryp.assign(request.c_str(),head.len_payload);
	std::string cryp=RSA::Encrypt(to_cryp,private_key_client);

	std::string pay_final=cryp;
	pay_final+="biagio\r\nbiagio";
	head.type_cmd=0x21;
	head.len_payload=pay_final.size();
	request.clear();
	AppendData_intoString(&head,sizeof(HeaderAuthProt),request);
	//AppendData_intoString(pay_final.c_str(),pay_final.size(),request);
	SDLNet_TCP_Send(this->connessione,request.c_str(),request.size());
	//----

	std::cout << "termina...\n";
	this->CloseConnection();
}

void Client::OpenConnection(void) {
	SDLNet_ResolveHost(&this->indirizzo_ip,"localhost",9999);
	this->connessione=SDLNet_TCP_Open(&this->indirizzo_ip);
}

void Client::CloseConnection(void) {
	SDLNet_TCP_Close(this->connessione);
	this->connessione=NULL;
}

}

 /* namespace tesina_rc */
