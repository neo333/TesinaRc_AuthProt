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
private:
	void OpenConnection(const Uint16) throw(const char*);
	void CloseConnection(void);

	Uint16 port_number;
	struct Data_scriptClient{
		SDL_Thread* id_thread;
		TCPsocket connessione;
		ID_Host id_host;
		bool exit;

		Data_scriptClient(const ID_Host _id=0,SDL_Thread* _th=NULL,TCPsocket _sk=NULL):id_thread(_th),connessione(_sk),id_host(_id),exit(false){ }
	};
	typedef std::map<ID_Host, Data_scriptClient> ListaHost;
	typedef ListaHost::iterator IteratoreLista;

	ListaHost connessioni;
	IPaddress indirizzo_ip;
	TCPsocket server;
	ID_Host count_id;

	void AcceptNuovoClient(void) throw(const char*);
	void UpData_Connections(void);

	static int script_client(void*) throw(const char*);
};

}
#endif
