/*
 * Client.hpp
 *
 *  Created on: 15/gen/2013
 *      Author: neo
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <SDL/SDL_net.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace tesina_rc {

class Client: private boost::noncopyable{
public:
	Client();
	void Run(const std::string&,const std::string&);

private:
	TCPsocket connessione;
	IPaddress indirizzo_ip;

	void OpenConnection(const std::string&, const Uint16);
	void CloseConnection(void);
};

}
#endif
