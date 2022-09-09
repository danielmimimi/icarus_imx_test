

#ifndef GENERAL_INFERENCER_H
#define GENERAL_INFERENCER_H

#include "tflite_inference.h"
#include "tensorResultToPassOn.h"
#include <cstring>

class generalInferencer : public tflite_inference_t
{
public:
    generalInferencer();
    virtual ~generalInferencer();

    int init(
        const std::string& modelPath,
        std::vector<int32_t>& outImageDimensions,
        int use_nnapi = 2,
        int num_threads = 4);

    void inference(const std::vector<uint8_t>& inputImage, std::vector<tensorResultToPassOn*>& outResults);

private:

    void packoutput(std::vector<tensorResultToPassOn*>& outResults);

    tensorResultToPassOn* packTesor(int currentTensorIndex);

    std::vector<std::string> mOutLayerNames; // Names of the network to be output

    std::vector<int> mInputImageDimension;
};

#endif