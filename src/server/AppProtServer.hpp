/*
 * AppProt.hpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#ifndef APPPROT_HPP_
#define APPPROT_HPP_

#include <boost/noncopyable.hpp>
#include <com_def.hpp>
#include <SDL/SDL.h>
#include <string>

namespace tesina_rc {

class AppProtServer: private boost::noncopyable{
public:
	AppProtServer();

	/*Ritorna 1 se è necessario trasmettere _out al client
	 * ritorna -1 se è necessario trasmettere _out al client e poi disconnetterlo
	 * ritorna 0 se NON bisogna trasmettere _out al client*/
	int ElaboraRichiesta(std::string& _in, std::string& _out);
private:
	bool login_request;
	bool auth_ok;
	Uint32 nonce;

	struct Packet{
		HeaderAuthProt header;
		char* body;
	};

	static void MakeError(std::string& _out,const char* mess){
		HeaderAuthProt head;
		head.type_cmd=0xFF;
		head.len_payload=strlen(mess);
		AppendData_intoString(&head,sizeof(head),_out);
		AppendData_intoString(mess,strlen(mess),_out);
	}
};

}
#endif /* APPPROT_HPP_ */
