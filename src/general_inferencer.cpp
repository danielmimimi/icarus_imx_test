

#include "general_inferencer.h"
#include <numeric>
#include "tensorResultToPassOnUint8.h"
#include "tensorResultToPassOnFloat.h"
#include <cstring>
#include <iostream>
#include <fstream>

generalInferencer::generalInferencer() : mFirstAttempt(true)
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
	if(mFirstAttempt){	
		std::cout << "Started Initialize Network... \n" << std::endl;
	}
    int returnValue = tflite_inference_t::init(modelPath, use_nnapi, num_threads);
	if(mFirstAttempt){	
	std::cout << "Finished Initialize Network :  " << modelPath << " \n" <<   std::endl;
	}
    returnValue = tflite_inference_t::get_input_tensor_shape(&outImageDimensions);
	if(mFirstAttempt){	
	std::cout << "Got Network input Dimensions \n" <<   std::endl;
	}
    mInputImageDimension = outImageDimensions;
	
    return returnValue;
}

void generalInferencer::inference(const std::vector<uint8_t> &inputImage, std::vector<tensorResultToPassOn*> &outResults)
{
	if(mFirstAttempt){	
	std::cout << "Set Data" << std::endl;
	}
    tflite_inference_t::setup_input_tensor_fast((uint8_t*)inputImage.data());
    //tflite_inference_t::setup_input_tensor(mInputImageDimension.at(3), mInputImageDimension.at(2), mInputImageDimension.at(1), (uint8_t *)inputImage.data());   
    //tflite_inference_t::setup_input_tensor(mInputImageDimension /* NOT REALLY NEEDED */, (uint8_t*)inputImage.data());
	if(mFirstAttempt){	
	std::cout << "Tries Inference" << std::endl;
	}
    tflite_inference_t::inference();
	if(mFirstAttempt){	
	std::cout << "Pack inference result" << std::endl;
	}
    packoutput(outResults);
	mFirstAttempt = false;
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
