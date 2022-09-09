#include "coralIcarusInterface.h"
#include "tensorResultToPassOn.h
#include "tensorResultToPassOnFloat.h
#include "tensorResultToPassOnUint8.h


typedef unsigned char uint8_t;

constexpr size_t kBmpFileHeaderSize = 14;
constexpr size_t kBmpInfoHeaderSize = 40;
constexpr size_t kBmpHeaderSize = kBmpFileHeaderSize + kBmpInfoHeaderSize;


int32_t ToInt32(const uint8_t p[4]) {
  return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

std::vector<uint8_t> ReadBmp(const std::string& filename, ImageDims* out_dims) {
  std::ifstream file(filename, std::ios::binary);

  uint8_t header[kBmpHeaderSize];
  file.read(reinterpret_cast<char*>(header), sizeof(header));

  const uint8_t* file_header = header;
  const uint8_t* info_header = header + kBmpFileHeaderSize;

  const int channels = info_header[14] / 8;

  const uint32_t offset = ToInt32(&file_header[10]);
  if (offset > kBmpHeaderSize)
    file.seekg(offset - kBmpHeaderSize, std::ios::cur);

  int width = ToInt32(&info_header[4]);

  int height = ToInt32(&info_header[8]);
  const bool top_down = height < 0;
  if (top_down) height = -height;

  const int line_bytes = width * channels;
  const int line_padding_bytes =
      4 * ((8 * channels * width + 31) / 32) - line_bytes;

  std::vector<uint8_t> image(line_bytes * height);
  for (int i = 0; i < height; ++i) {
    uint8_t* line = &image[(top_down ? i : (height - 1 - i)) * line_bytes];
    file.read(reinterpret_cast<char*>(line), line_bytes);
    file.seekg(line_padding_bytes, std::ios::cur);

    // BGR => RGB or BGRA => RGBA
    if (channels == 3 || channels == 4) {
      for (int j = 0; j < width; ++j)
        std::swap(line[channels * j], line[channels * j + 2]);
    }
  }

  if (out_dims) *out_dims = {height, width, channels};

  return image;
}

int main(void)
{
	coralIcarusInference inferencer;
		
    std::string modelName = "posenet_mobilenet_v1_075_353_481_quant_decoder.tflite";
	std::vector<int32_t>& outImageDimensions;
	inferencer.init(modelName,outImageDimensions,1,1)
	
	std::cout << "inputsize 0 :" + outImageDimensions.at(0) << std::endl;
	
	ImageDims image_dims;
    std::vector<uint8_t> input = ReadBmp("persons.bmp", &image_dims);
	
	std::vector<tensorResultToPassOn*> outResults;
	inferencer.inference(std::vector<uint8_t>& inputImage,outResults)

    return 0;
}




