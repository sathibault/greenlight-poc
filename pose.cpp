#include <stdio.h>
#include <iostream>
#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

using namespace dlib;

const char *facefile="/usr/share/dlib/shape_predictor_68_face_landmarks.dat";
static shape_predictor sp;

void init_pose() {
  deserialize(facefile) >> sp;
}

void update_dets(array2d<bgr_pixel>& img, std::vector<rectangle>& dets) {
  static frontal_face_detector detector = get_frontal_face_detector();
  dets = detector(img);
}

void get_pose(array2d<bgr_pixel>& img,
	      std::vector<dlib::rectangle>& dets, int scaleDets,
	      std::vector<dlib::full_object_detection>& shapes,
	      int& closest, rectangle& bbox) {  
  long maxArea = 0;
  closest = -1;
  shapes.clear();
  for (size_t i = 0; i < dets.size(); i++) {
    rectangle rt((long)(dets[i].left() * scaleDets),
		(long)(dets[i].top() * scaleDets),
		(long)(dets[i].right() * scaleDets),
		(long)(dets[i].bottom() * scaleDets));
    if (rt.area() > maxArea) {
      closest = i;
      bbox = rt;
      maxArea = rt.area();
    }
    full_object_detection shape = sp(img, rt);
    shapes.push_back(shape);
  }
}
