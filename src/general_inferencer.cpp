

#include "general_inferencer.h"
#include <numeric>
#include "tensorResultToPassOnUint8.h"
#include "tensorResultToPassOnFloat.h"

generalInferencer::generalInferencer()
{
}

generalInferencer::~generalInferencer()
{
}

int generalInferencer::init(
    const std::string &modelPath,
    std::vector<int32_t> &outImageDimensions,
    int use_nnapi,
    int num_threads)
{
    int returnValue = tflite_inference_t::init(modelPath, use_nnapi, num_threads);
    returnValue = tflite_inference_t::get_input_tensor_shape(&outImageDimensions);
    tflite_inference_t::get_input_tensor_shape(&outImageDimensions);
    mInputImageDimension = outImageDimensions;
    return returnValue;
}

void generalInferencer::inference(const std::vector<uint8_t> &inputImage, std::vector<tensorResultToPassOn*> &outResults)
{
    //tflite_inference_t::setup_input_tensor(mInputImageDimension.at(3), mInputImageDimension.at(2), mInputImageDimension.at(1), (uint8_t *)inputImage.data());
    tflite_inference_t::setup_input_tensor(mInputImageDimension /* NOT REALLY NEEDED */, (uint8_t*)inputImage.data());
    tflite_inference_t::inference();
    packoutput(outResults);
}

void generalInferencer::packoutput(std::vector<tensorResultToPassOn*> &outResults)
{
    std::vector<int> outputs = interpreter_->outputs();
    for (int i = 0; i < (int)outputs.size(); i++)
    {     
        outResults.push_back(packTesor(i));
    }
}

tensorResultToPassOn* generalInferencer::packTesor(int currentTensorIndex)
{
    TfLiteType inputTensorType = interpreter_.get()->output_tensor(currentTensorIndex)->type;
    if (inputTensorType == TfLiteType::kTfLiteUInt8) {
        tensorResultToPassOnUint8* outTensor = new tensorResultToPassOnUint8();
        outTensor->name = interpreter_.get()->GetOutputName(currentTensorIndex);
        TfLiteIntArray* dimensions = interpreter_.get()->output_tensor(currentTensorIndex)->dims;
        
        if (dimensions) {
            for (int i = 0; i < dimensions->size; i++) {
                outTensor->dimensions.push_back(dimensions->data[i]);
            }
        }

        // input and output are type dependent - 
        int sizeOfvector = std::accumulate(outTensor->dimensions.begin(), outTensor->dimensions.end(), 1, std::multiplies<int>())* sizeof(uint8_t);
        outTensor->data = new uint8_t[sizeOfvector];
        memcpy(outTensor->data, interpreter_.get()->output_tensor(currentTensorIndex)->data.uint8, sizeOfvector );

        return outTensor;
    }
    else {

        tensorResultToPassOnFloat* outTensor = new tensorResultToPassOnFloat();
        outTensor->name = interpreter_.get()->GetOutputName(currentTensorIndex);
        TfLiteIntArray* dimensions = interpreter_.get()->output_tensor(currentTensorIndex)->dims;

        if (dimensions) {
            for (int i = 0; i < dimensions->size; i++) {
                outTensor->dimensions.push_back(dimensions->data[i]);
            }
        }

        // input and output are type dependent - 
        int sizeOfvector = std::accumulate(outTensor->dimensions.begin(), outTensor->dimensions.end(), 1, std::multiplies<int>()) * sizeof(float);
        outTensor->data = new float[sizeOfvector];
        memcpy(outTensor->data, interpreter_.get()->output_tensor(currentTensorIndex)->data.f, sizeOfvector);
        return outTensor;
    }


}
