#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <vector>

#include "sxbrowser.h"

extern void init_pose();
extern int pose_status();
extern void find_faces(const Img<Rgb8>& img, std::vector<Box>& boxes);
extern int get_pose(const Img<Rgb8>& img, std::vector<Box>& dets, int scaleDets, std::vector<Point2f>& points, std::vector<float>& euler, Box& bbox);

extern "C" void init_models() {
  init_pose();
}

extern "C" int model_status() {
  return pose_status();
}

static std::vector<Box> boxes;

extern "C" int detect(int width, int height, uint8_t *rgba) {
  Buffer<Rgb> *buf = new Buffer<Rgb>((Rgb *)rgba, width*height);
  Img<Rgb> *img = new Img<Rgb>(height, width, buf);
  boxes.clear();
  find_faces(*img, boxes);
  delete img;
  delete buf;
  return (int)boxes.size();
}

extern "C" void get_box(int idx, uint32_t *ary) {
  ary[0] = boxes[idx].left;
  ary[1] = boxes[idx].top;
  ary[2] = boxes[idx].right;
  ary[3] = boxes[idx].bottom;
}

extern "C" int pose(int width, int height, uint8_t *rgba, int32_t *ary, float *euler) {
  Box loc;
  std::vector<Point2f> points;
  std::vector<float> euvec;
  Buffer<Rgb> *buf = new Buffer<Rgb>((Rgb *)rgba, width*height);
  Img<Rgb> *img = new Img<Rgb>(height, width, buf);
  int closest = get_pose(*img, boxes, 1, points, euvec, loc);
  if (closest >= 0) {
    for (int i = 0; i < 14; i++) {
      //printf("%f %f\n", points[i]._1,points[i]._2);
      ary[(i<<1)] = points[i]._1;
      ary[(i<<1)+1] = points[i]._2;
    }
    for (int i = 0; i < 3; i++)
      euler[i] = euvec[i];
  }
  delete img;
  delete buf;
  return (int)closest;
}
