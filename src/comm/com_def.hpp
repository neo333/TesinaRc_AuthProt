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
#include <string>

namespace tesina_rc{
	static const Key private_key_client(BigInt("45809645939"),BigInt("5222892113"));
	static const Key public_key_client(BigInt("45809645939"),BigInt("65537"));

	struct HeaderAuthProt{
		Uint8 type_cmd;
		Uint32 len_payload;
	};

	inline static void AppendData_intoString(const void* data, const size_t size_data, std::string& _out){
		_out.append((const char*)(data),size_data);
	}
	inline static void PopData_fromString(void* data, const size_t data_to_preleve, std::string& _inout, const size_t offset=0){
		char* _data=(char*)(data);
		for(register size_t i=0; i<data_to_preleve; i++){
			_data[i]=_inout[i+offset];
		}
		_inout.erase(offset,data_to_preleve);
	}
	inline static void PopData_fromString_andNoRemove(void* data, const size_t data_to_preleve, const std::string& _inout, const size_t offset=0){
		char* _data=(char*)(data);
		for(register size_t i=0; i<data_to_preleve; i++){
			_data[i]=_inout[i+offset];
		};
	}
}


#endif /* COM_DEF_HPP_ */
