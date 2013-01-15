/*
 * AppProtServer.cpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#include "AppProtServer.hpp"
#include <cstdlib>
#include <ctime>
#include <RSA/RSA.h>
#include <com_def.hpp>

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
	PopData_fromString_andNoRemove(&mess.header,sizeof(HeaderAuthProt),_in);
	if(this->auth_ok==false && mess.header.type_cmd!=0x20 && mess.header.type_cmd!=0x21){
		AppProtServer::MakeError(_out,"E' necessario autenticarsi prima di richiedere servizi!\n");
		return -1;
	}
	if(_in.size()-5 < mess.header.len_payload){
		if(this->auth_ok==false){
			if(mess.header.len_payload>1024){
				AppProtServer::MakeError(_out,"Richiesta non autorizzata!\n");
				return -1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}
	PopData_fromString(&mess,5+mess.header.len_payload,_in);


	switch(mess.header.type_cmd){
	case 0x20:
		srand(time(NULL));
		this->nonce=rand();
		HeaderAuthProt head_snd;
		head_snd.type_cmd=0x22;
		head_snd.len_payload=sizeof(this->nonce);
		AppendData_intoString(&head_snd,sizeof(HeaderAuthProt),_out);
		AppendData_intoString(&this->nonce,sizeof(this->nonce),_out);
		this->login_request=true;
		return 1;
	case 0x21:
		if(this->login_request==true){
			std::cout << "Provo a decriptare\n";
			std::string decryp_mess=RSA::Decrypt(mess.body,public_key_client);
			std::cout << "decriptato!\n";
			std::string nonce_str=decryp_mess.substr(0,sizeof(this->nonce));
			decryp_mess.erase(0,sizeof(this->nonce));
			size_t find=decryp_mess.find("\r\n");
			if(find==std::string::npos){
				AppProtServer::MakeError(_out,"Richiesta di autorizzazione non corretta!\n");
				return -1;
			}
			std::string nickname=decryp_mess.substr(0,find);
			decryp_mess.erase(0,find+2);
			std::string pass=decryp_mess;

			Uint32 nonce_rcv;
			PopData_fromString_andNoRemove(&nonce_rcv,sizeof(Uint32),nonce_str);
			if(nonce_rcv==this->nonce){
				HeaderAuthProt head;
				head.type_cmd=0x23;
				head.len_payload=0;
				AppendData_intoString(&head_snd,sizeof(HeaderAuthProt),_out);
				this->auth_ok=true;
				return 1;
			}else{
				AppProtServer::MakeError(_out,"Autorizzazione non corretta, tentativo di intrusione nel sistema rilevato!\n");
				return -1;
			}
		}else{
			AppProtServer::MakeError(_out,"Autorizzazione non avviata!\n");
			return -1;
		}
		break;
	default:
		AppProtServer::MakeError(_out,"Comando non riconosciuto!\n");
		return -1;
	}

	return 0;
}

}
