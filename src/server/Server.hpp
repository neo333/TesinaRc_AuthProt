/*
 * Server.hpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <SDL/SDL_net.h>
#include <map>
#include <boost/noncopyable.hpp>

namespace tesina_rc {

typedef unsigned int ID_Host;

class Server: private boost::noncopyable{
public:
	Server();
	~Server();

	void Run(const Uint16 =0) throw(const char*);
	inline const bool Is_ServerActive(void) const{
		if(listner!=NULL) return true;
		return false;
	}
private:
	void OpenConnection(const Uint16) throw(const char*);
	void CloseConnection(void);

	Uint16 port_number;
	typedef struct {
		SDL_Thread* id_thread;
		TCPsocket connessione;
		ID_Host id_host;
	} Data_scriptClient;
	typedef std::map<ID_Host, Data_scriptClient> ListaHost;

	std::map<ID_Host, TCPsocket> connessioni;
	typedef std::map<ID_Host,TCPsocket>::iterator IteratoreConnessioni;
	typedef std::map<ID_Host,TCPsocket>::const_iterator IteratoreConnessioniConst;
	SDLNet_SocketSet listner;
	IPaddress indirizzo_ip;
	TCPsocket server;
	ID_Host count_id;

	void AddTCPSocket_ToListner(TCPsocket) throw(const char*);
	inline const bool IsReadyHost(const ID_Host _id){
		IteratoreConnessioni it=this->connessioni.find(_id);
		if(it==this->connessioni.end()) return false;
		TCPsocket sock=(*it).second;
		if(SDLNet_SocketReady(sock)) return true;
		return false;
	}
	void AcceptNuovoClient(void) throw(const char*);

	static int script_client(void* data);
};

}
#endif
