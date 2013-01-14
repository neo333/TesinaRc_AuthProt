/*
 * AppProtServer.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include "AppProtServer.hpp"
#include <com_def.hpp>
#include <cstdlib>
#include <ctime>

namespace tesina_rc {

AppProtServer::AppProtServer():login_request(false),auth_ok(false){

}

int AppProtServer::ElaboraRichiesta(std::string& _in, std::string& _out) {
	_out.clear();
	if(_in.size()<4){
		return 0;
	}
	HeaderAuthProt header;
	header.type_cmd=(Uint8)(_in[0]);
	if(this->auth_ok==false){
		if(header.type_cmd!=0x20 && header.type_cmd!=0x21){
			AppProtServer::MakeError(_out,"E' necessario autorizzare il client prima di eseguire una richiesta\n");
			return -1;
		}
	}
	header.len_payload=(Uint32)(_in.substr(1,4).data());
	if(_in.size()-4 < header.len_payload){
		if(this->auth_ok==false && header.len_payload>1024){
			AppProtServer::MakeError(_out,"Comando di autorizzazione non valido!\n");
			return -1;
		}
		return 0;
	}

	std::string payload=_in.substr(4,header.len_payload);
	_in=_in.substr(4+header.len_payload,std::string::npos);
	switch(header.type_cmd){
	case 0x20:
		srand(time(NULL));
		this->nonce=rand();
		AppProtServer::AppendHeader(_out,0x20,sizeof(this->nonce));
		AppProtServer::AppendBuffer(&this->nonce,sizeof(this->nonce),_out);
		this->login_request=true;
		return 1;
	case 0x21:
		if(this->login_request==true){
			//TODO:: fare!
			AppProtServer::MakeError(_out,"Server non ha ancora implementato certificato di autorizzazione!\n");
			return -1;
		}else{
			AppProtServer::MakeError(_out,"Autorizzazione non avviata!\n");
			return -1;
		}
		break;
	default:
		AppProtServer::MakeError(_out,"Comando non riconosciuto!\n");
		return 0;
	}

	return 0;
}

void AppProtServer::AppendHeader(std::string& buffer, const Uint8 cmd, const Uint32 data_len) {
	AppProtServer::AppendBuffer(&cmd,sizeof(Uint8),buffer);
	AppProtServer::AppendBuffer(&data_len,sizeof(Uint32),buffer);
}

void AppProtServer::AppendBuffer(const void* data, const size_t len,std::string& buffer) {
	char* p=(char*)(data);
	for(register size_t i=0; i<len; i++){
		buffer.push_back(p[i]);
	}
}

void AppProtServer::MakeError(std::string& _buffer, const char* mess) {
	AppProtServer::AppendHeader(_buffer,0xFF,strlen(mess));
	AppProtServer::AppendBuffer(mess,strlen(mess),_buffer);
}



}
