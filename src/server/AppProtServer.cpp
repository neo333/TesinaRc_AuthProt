/*
 * AppProtServer.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include "AppProtServer.hpp"
#include <cstdlib>
#include <ctime>

namespace tesina_rc {

AppProtServer::AppProtServer():login_request(false),auth_ok(false){

}

int AppProtServer::ElaboraRichiesta(std::string& _in, std::string& _out) {
	if(_out.size()>0){
		_out.clear();
	}
	if(_in.size()<5){
		return 0;
	}
	
	Packet mess;
	if(this->MakePacket(_in,mess)==0){
		if(this->auth_ok==false){
			if(mess.header.len_payload>1024){
				AppProtServer::MakeError(_out,"Richiesta non autorizzata!\n");
				return -1;
			}
		}
	}


	switch(mess.header.type_cmd){
	case 0x20:
		srand(time(NULL));
		this->nonce=rand();
		AppProtServer::AppendHeader(_out,0x22,sizeof(this->nonce));
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


int AppProtServer::MakePacket(std::string& buffer,Packet& _out){
	_out.body.clear();
	AppProtServer::CastingBuffer(buffer,0,&_out.header.type_cmd);
	AppProtServer::CastingBuffer(buffer,1,&_out.header.len_payload);
	if(buffer.size()-5 < _out.header.len_payload) return 0;
	_out.body=buffer.substr(4,_out.header.len_payload);
	buffer=buffer.substr(4+_out.header.len_payload,std::string::npos);
	return 1;
}

}
