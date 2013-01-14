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

	static void AppendHeader(std::string& buffer,const Uint8 cmd,const Uint32 data_len);
	static void AppendBuffer(const void*, const size_t,std::string&);

	static void MakeError(std::string& _buffer, const char*);

	struct Packet{
		HeaderAuthProt header;
		std::string body;
	};
	int MakePacket(std::string&,Packet&);

	template<class Type> static void CastingBuffer(const std::string& buffer,const size_t da,Type* _out){
		size_t size_of_type=sizeof(Type);
#ifdef _DEBUG
		char prova[sizeof(Type)];
		memcpy(prova,buffer.substr(da,sizeof(Type)).c_str(),sizeof(Type));
#endif
		memcpy(_out,buffer.substr(da,sizeof(Type)).c_str(),sizeof(Type));
	}
};

}
#endif /* APPPROT_HPP_ */
