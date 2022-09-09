
#include "tensorResultToPassOnUint8.h"
#include <numeric>


tensorResultToPassOnUint8::tensorResultToPassOnUint8() : data(NULL)
{
}


tensorResultToPassOnUint8::~tensorResultToPassOnUint8()
{
	if (data != NULL) {
		delete data;
	}
}

tensorResultToPassOnUint8::tensorResultToPassOnUint8(const tensorResultToPassOnUint8& obj)
{
	dimensions = obj.dimensions;
	name = obj.name;
	int sizeOfvector = std::accumulate(obj.dimensions.begin(), obj.dimensions.end(), 1, std::multiplies<uint8_t>()) * sizeof(uint8_t);
	data = new uint8_t[sizeOfvector];
	memcpy(data, obj.data, sizeOfvector);
}

