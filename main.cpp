#include "coralIcarusInterface.h"
#include "tensorResultToPassOn.h"
#include "tensorResultToPassOnFloat.h"
#include "tensorResultToPassOnUint8.h"

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <numeric>
#include <chrono>

typedef unsigned char uint8_t;

constexpr size_t kBmpFileHeaderSize = 14;
constexpr size_t kBmpInfoHeaderSize = 40;
constexpr size_t kBmpHeaderSize = kBmpFileHeaderSize + kBmpInfoHeaderSize;

struct ImageDims
{
  int height;
  int width;
  int depth;
};

int32_t ToInt32(const uint8_t p[4])
{
  return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

std::vector<uint8_t> ReadBmp(const std::string &filename, ImageDims *out_dims)
{
  std::ifstream file(filename, std::ios::binary);

  uint8_t header[kBmpHeaderSize];
  file.read(reinterpret_cast<char *>(header), sizeof(header));

  const uint8_t *file_header = header;
  const uint8_t *info_header = header + kBmpFileHeaderSize;

  const int channels = info_header[14] / 8;

  const uint32_t offset = ToInt32(&file_header[10]);
  if (offset > kBmpHeaderSize)
    file.seekg(offset - kBmpHeaderSize, std::ios::cur);

  int width = ToInt32(&info_header[4]);

  int height = ToInt32(&info_header[8]);
  const bool top_down = height < 0;
  if (top_down)
    height = -height;

  const int line_bytes = width * channels;
  const int line_padding_bytes =
      4 * ((8 * channels * width + 31) / 32) - line_bytes;

  std::vector<uint8_t> image(line_bytes * height);
  for (int i = 0; i < height; ++i)
  {
    uint8_t *line = &image[(top_down ? i : (height - 1 - i)) * line_bytes];
    file.read(reinterpret_cast<char *>(line), line_bytes);
    file.seekg(line_padding_bytes, std::ios::cur);

    // BGR => RGB or BGRA => RGBA
    if (channels == 3 || channels == 4)
    {
      for (int j = 0; j < width; ++j)
        std::swap(line[channels * j], line[channels * j + 2]);
    }
  }

  if (out_dims)
    *out_dims = {height, width, channels};

  return image;
}

int main(void)
{
  bool posenet = false;
  bool mnist = false;
  bool ssd = true;
  coralIcarusInference inferencer;
  std::string modelName;
  if (mnist)
  {
    modelName = "mnist_pytorch_onnx_8_bit_edgetpu.tflite";
  }
  if (posenet)
  {
    modelName = "posenet_mobilenet_v1_075_353_481_quant_decoder.tflite";
  }
  if (ssd)
  {
    modelName = "mobilenet_ssd_v2_coco_quant_postprocess.tflite";
  }

  std::ifstream file(modelName);
  std::vector<int32_t> outImageDimensions;
  if (file.is_open())
  {
    file.close();
    inferencer.init(modelName, outImageDimensions, 2, 4);
  }
  else
  {
    throw std::runtime_error("Path to model does not exist");
  }

  std::cout
      << "inputsize 1 :" + std::to_string(outImageDimensions[1]) << std::endl;

  ImageDims image_dims;
  std::vector<uint8_t> input;
  if (posenet)
  {
    input = ReadBmp("persons.bmp", &image_dims);
  }
  if (mnist)
  {
    input = ReadBmp("three.bmp", &image_dims);
  }
  if (ssd)
  {
    input = ReadBmp("persons_ssd.bmp", &image_dims);
  }

  auto start = std::chrono::high_resolution_clock::now();
  int amountOfInferences = 1;
  for (int i = 0; i < amountOfInferences; i++)
  {
    std::vector<tensorResultToPassOn *> outResults;
    inferencer.inference(input, outResults);

    for (int i = 0; i < outResults.size(); i++)
    {
      std::ofstream myFile_Handler, myFile_Handler_dim;
      myFile_Handler.open(outResults.at(i)->name + ".txt");
      myFile_Handler_dim.open(outResults.at(i)->name + "_dim.txt");
      if (myFile_Handler.is_open())
      {
        if (myFile_Handler_dim.is_open())
        {
          for (int k = 0; k < outResults.at(i)->dimensions.size(); k++)
          {
            myFile_Handler_dim << outResults.at(i)->dimensions.at(k) << std::endl;
          }
        }
        if (typeid(*outResults.at(i)) == typeid(tensorResultToPassOnFloat))
        {
          tensorResultToPassOnFloat *floatTensor = dynamic_cast<tensorResultToPassOnFloat *>(outResults.at(i));
          int sizeOfvector = std::accumulate(floatTensor->dimensions.begin(), floatTensor->dimensions.end(), 1, std::multiplies<int>());
          std::vector<float> dest(floatTensor->data, floatTensor->data + sizeOfvector);
          for (int vecSize = 0; vecSize < dest.size(); vecSize++)
          {
            if (dest.at(vecSize) == 0)
            {
              myFile_Handler << dest.at(vecSize) << ".f," << std::endl;
            }
            else
            {
              myFile_Handler << dest.at(vecSize) << "f," << std::endl;
            }
          }
        }
        if (typeid(*outResults.at(i)) == typeid(tensorResultToPassOnUint8))
        {
          tensorResultToPassOnUint8 *uint8Tensor = dynamic_cast<tensorResultToPassOnUint8 *>(outResults.at(i));
          int sizeOfvector = std::accumulate(uint8Tensor->dimensions.begin(), uint8Tensor->dimensions.end(), 1, std::multiplies<int>());
          std::vector<uint8_t> dest(uint8Tensor->data, uint8Tensor->data + sizeOfvector);
          for (int vecSize = 0; vecSize < dest.size(); vecSize++)
          {
            myFile_Handler << dest.at(vecSize) << "," << std::endl;
          }
        }
      }
      myFile_Handler.close();
      myFile_Handler_dim.close();
    }
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

  std::cout << duration.count() / amountOfInferences << "Microseconds" << std::endl;

  return 0;
}
