/*
 * com_def.hpp
 *
 *  Created on: 14/gen/2013
 *      Author: neo
 */

#ifndef COM_DEF_HPP_
#define COM_DEF_HPP_

#include "RSA/Key.h"
#include <SDL/SDL.h>

namespace tesina_rc{
	static const Key private_key_client(BigInt("45809645939"),BigInt("5222892113"));
	static const Key public_key_client(BigInt("45809645939"),BigInt("65537"));

	struct HeaderAuthProt{
		Uint8 type_cmd;
		Uint32 len_payload;
	};
}


#endif /* COM_DEF_HPP_ */
