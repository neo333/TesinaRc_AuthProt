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

void Client::Run(const std::string& _host, const std::string& _port) {
	std::cout << "Connessione al server...\n\n";
	this->OpenConnection(_host,atoi(_port.c_str()));

	//DEBUG
	std::string nickname;
	std::string password;
	std::cout << "Inserire dati identificativi\n";
	std::cout << "\tNickname: ";
	std::cin >> nickname;
	std::cout << "\tPassword: ";
	std::cin >> password;
	std::cout << "\n\n";

	std::string request;
	HeaderAuthProt head;
	int bytes;
	char buffer[1024];

	head.type_cmd=0x20;
	head.len_payload=0;
	AppendData_intoString(&head,sizeof(head),request);
	SDLNet_TCP_Send(this->connessione,request.c_str(),request.size());
	std::cout << "Invio richiesta di login...\n\n";
	bytes=SDLNet_TCP_Recv(this->connessione,buffer,1024);
	request.clear();
	AppendData_intoString(buffer,bytes,request);
	PopData_fromString(&head,sizeof(HeaderAuthProt),request);
	if(head.type_cmd!=0x22){
		std::cerr << "errore!1\n";
	}
	std::string to_cryp;
	to_cryp.assign(request.c_str(),head.len_payload);
	Uint32 nonce_reuq;
	tesina_rc::PopData_fromString_andNoRemove(&nonce_reuq,sizeof(nonce_reuq),to_cryp);
	to_cryp+=nickname;
	to_cryp+="\r\n";
	to_cryp+=password;
	std::cout << "Il server ha richiesto la verifica del nonce!\n"
		"   Nonce richiesto: " << nonce_reuq << "\n"
		         "   Codifica in corso...\n\n";
	std::string cryp=RSA::Encrypt(to_cryp,private_key_client);

	head.type_cmd=0x21;
	head.len_payload=cryp.size();
	request.clear();
	AppendData_intoString(&head,sizeof(HeaderAuthProt),request);
	AppendData_intoString(cryp.c_str(),cryp.size(),request);
	SDLNet_TCP_Send(this->connessione,request.c_str(),request.size());
	std::cout << "Nonce codificato e richiesta di autorizzazione inviata al server!\n\n";
	//----

	std::cout << "Client termina la connessione col server! E termina programma!\n\n";
	this->CloseConnection();
}

void Client::OpenConnection(const std::string& hostname, const Uint16 port) {
	SDLNet_ResolveHost(&this->indirizzo_ip,hostname.c_str(),port);
	this->connessione=SDLNet_TCP_Open(&this->indirizzo_ip);
	if(this->connessione==NULL){
		throw "Impossibile connettersi!";
	}
}

void Client::CloseConnection(void) {
	SDLNet_TCP_Close(this->connessione);
	this->connessione=NULL;
}

}

 /* namespace tesina_rc */
