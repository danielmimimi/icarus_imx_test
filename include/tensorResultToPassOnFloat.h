#ifndef TENSORRESULTTOPASSONFLOAT
#define TENSORRESULTTOPASSONFLOAT

#include <string>
#include <vector>
#include "tensorResultToPassOn.h"
#include <cstring>

class tensorResultToPassOnFloat : public tensorResultToPassOn {
public:
	tensorResultToPassOnFloat();
	~tensorResultToPassOnFloat();

	tensorResultToPassOnFloat(const tensorResultToPassOnFloat& obj);

	float* data;
};

#endif