#ifndef __CLIENT__HPP
#define __CLIENT__HPP

#include <boost/noncopyable.hpp>
#include <SDL/SDL_net.h>

namespace tesina_rc {

class Client: private boost::noncopyable{
public:
	Client(void){

	}

	void Run();
private:
	IPaddress indirizzo_ip;
	TCPsocket connessione;

	void OpenConnection();
	void CloseConnection();
};

}

#endif