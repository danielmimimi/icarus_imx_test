#include "coralIcarusInference.h"

coralIcarusInference::coralIcarusInference() 
{
}

coralIcarusInference::~coralIcarusInference()
{
}

void coralIcarusInference::init(const std::string& modelPath, std::vector<int32_t>& outImageDimensions, int use_nnapi, int num_threads)
{
	mInferencer.init(modelPath, outImageDimensions, use_nnapi, num_threads);
}

void coralIcarusInference::inference(const std::vector<uint8_t>& inputImage, std::vector<tensorResultToPassOn*>& outResults)
{
	mInferencer.inference(inputImage, outResults);
}

