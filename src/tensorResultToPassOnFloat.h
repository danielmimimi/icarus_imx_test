#ifndef TENSORRESULTTOPASSONFLOAT
#define TENSORRESULTTOPASSONFLOAT

#include <string>
#include <vector>
#include "tensorResultToPassOn.h"

class tensorResultToPassOnFloat : public tensorResultToPassOn {
public:
	tensorResultToPassOnFloat();
	~tensorResultToPassOnFloat();

	tensorResultToPassOnFloat(const tensorResultToPassOnFloat& obj);

	float* data;
};

#endif