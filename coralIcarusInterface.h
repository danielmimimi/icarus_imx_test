

#ifndef CORAL_ICARUS_INFERENCER_H
#define CORAL_ICARUS_INFERENCER_H


#include "tensorResultToPassOnFloat.h"
#include "tensorResultToPassOn.h"
#include "tensorResultToPassOnUint8.h"
#include "coralIcarusInference.h"


class coralIcarusInference
{
public:
    coralIcarusInference();
    ~coralIcarusInference();

    void init(const std::string& modelPath,
        std::vector<int32_t>& outImageDimensions,
        int use_nnapi,
        int num_threads);

    void inference(const std::vector<uint8_t>& inputImage, std::vector<tensorResultToPassOn*>& outResults);
};

#endif