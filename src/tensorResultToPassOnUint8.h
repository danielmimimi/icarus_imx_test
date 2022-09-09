#ifndef TENSORRESULTTOPASSONUINT8
#define TENSORRESULTTOPASSONUINT8

#include <string>
#include <vector>
#include "tensorResultToPassOn.h"

class tensorResultToPassOnUint8 : public tensorResultToPassOn {
public:
	tensorResultToPassOnUint8();
	~tensorResultToPassOnUint8();

	tensorResultToPassOnUint8(const tensorResultToPassOnUint8& obj);

	uint8_t* data;
};

#endif