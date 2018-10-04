#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cpptorch/cpptorch.h>

using namespace cv;

const int img_dim = 96;

static cpptorch::Tensor<float> input;
static std::shared_ptr<cpptorch::nn::Layer<float>> net;

void init_embedding() {
  std::ifstream fs_net(std::string("nn4.small2.v1.t7"), std::ios::binary);
  if (!fs_net.good()) {
    std::cerr << "Cannot find torch module: nn4.small2.v1.t7" << std::endl;
    return;
  }
  auto obj_t = cpptorch::load(fs_net);
  net = cpptorch::read_net<float>(obj_t.get());

  input.create();
  input.resize({1, 3, img_dim, img_dim});
}

void embedding(const unsigned char *img, float *features) {
  float *ten = input.data();
  for (size_t c = 0; c < 3; c++) {
    for (size_t p = 0; p < img_dim * img_dim; p++) {
      // normalize  to [0,1]
      ten[c * img_dim * img_dim + p] = (float)img[p * 3 + 2 - c] / 255;
    }
  }


  cpptorch::Tensor<float> output = net->forward(input);
  const float *output_ptr = output.data();
  memcpy(features, output_ptr, 128*sizeof(float));
#if 0
  for (int i = 0; i < 128; i++)
    std::cout << output_ptr[i] << " ";
  std::cout << std::endl;
#endif
}

void embedding(Mat image, float *features) {
  embedding(image.ptr(0), features);
}
