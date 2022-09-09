#ifndef TENSORRESULTTOPASSON
#define TENSORRESULTTOPASSON

#include <string>
#include <vector>

class tensorResultToPassOn {
public:
	tensorResultToPassOn();
	virtual ~tensorResultToPassOn();

	tensorResultToPassOn(const tensorResultToPassOn& obj);

	std::string name;
	std::vector<int32_t> dimensions;
};

#endif