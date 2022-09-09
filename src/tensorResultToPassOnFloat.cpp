
#include "tensorResultToPassOnFloat.h"
#include <numeric>


tensorResultToPassOnFloat::tensorResultToPassOnFloat() : data(NULL)
{
}


tensorResultToPassOnFloat::~tensorResultToPassOnFloat()
{
	if (data != NULL) {
		delete data;
	}
}

tensorResultToPassOnFloat::tensorResultToPassOnFloat(const tensorResultToPassOnFloat& obj)
{
	dimensions = obj.dimensions;
	name = obj.name;
	int sizeOfvector = std::accumulate(obj.dimensions.begin(), obj.dimensions.end(), 1, std::multiplies<float>()) * sizeof(float);
	data = new float[sizeOfvector];
	memcpy(data, obj.data, sizeOfvector);
}

