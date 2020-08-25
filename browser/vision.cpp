#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <emscripten/fetch.h>

#include <iostream>
#include <istream>
#include <streambuf>

#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "sxbrowser.h"
#include "img_dlib.h"

using namespace dlib;

const char *facefile="shape_predictor_68_face_landmarks.dat";
static shape_predictor sp;
static int loaded = -1;

struct membuf : std::streambuf {
  membuf(const char *begin, const char *end) {
    this->setg((char *)begin, (char *)begin, (char *)end);
  }
};

static cv::Mat get_camera_matrix(float focal_length, cv::Point2d center) {
  cv::Mat camera_matrix = (cv::Mat_<double>(3,3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
  return camera_matrix;
}

static std::vector<float> rotationMatrixToEulerAngles(cv::Mat &R) {
  float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );
  bool singular = sy < 1e-6; // If

  float x, y, z;
  if (!singular) {
    x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
    y = atan2(-R.at<double>(2,0), sy);
    z = atan2(R.at<double>(1,0), R.at<double>(0,0));
  } else {
    x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
    y = atan2(-R.at<double>(2,0), sy);
    z = 0;
  }
  std::vector<float> res;
  res.push_back(x);
  res.push_back(y);
  res.push_back(z);
  return res;
}

static std::vector<cv::Point3d> get_3d_model_points() {
  std::vector<cv::Point3d> object_pts;
  
  object_pts.push_back(cv::Point3d(6.825897, 6.760612, 4.402142));     //#33 left brow left corner
  object_pts.push_back(cv::Point3d(1.330353, 7.122144, 6.903745));     //#29 left brow right corner
  object_pts.push_back(cv::Point3d(-1.330353, 7.122144, 6.903745));    //#34 right brow left corner
  object_pts.push_back(cv::Point3d(-6.825897, 6.760612, 4.402142));    //#38 right brow right corner
  object_pts.push_back(cv::Point3d(5.311432, 5.485328, 3.987654));     //#13 left eye left corner
  object_pts.push_back(cv::Point3d(1.789930, 5.393625, 4.413414));     //#17 left eye right corner
  object_pts.push_back(cv::Point3d(-1.789930, 5.393625, 4.413414));    //#25 right eye left corner
  object_pts.push_back(cv::Point3d(-5.311432, 5.485328, 3.987654));    //#21 right eye right corner
  object_pts.push_back(cv::Point3d(2.005628, 1.409845, 6.165652));     //#55 nose left corner
  object_pts.push_back(cv::Point3d(-2.005628, 1.409845, 6.165652));    //#49 nose right corner
  object_pts.push_back(cv::Point3d(2.774015, -2.080775, 5.048531));    //#43 mouth left corner
  object_pts.push_back(cv::Point3d(-2.774015, -2.080775, 5.048531));   //#39 mouth right corner
  object_pts.push_back(cv::Point3d(0.000000, -3.116408, 6.097667));    //#45 mouth central bottom corner
  object_pts.push_back(cv::Point3d(0.000000, -7.415691, 4.070434));    //#6 chin corner
  return object_pts;
}

static void get_2d_image_points(full_object_detection &shape,
				std::vector<cv::Point2d>& pts) {
  pts.push_back(cv::Point2d(shape.part(17).x(), shape.part(17).y())); //#17 left brow left corner
  pts.push_back(cv::Point2d(shape.part(21).x(), shape.part(21).y())); //#21 left brow right corner
  pts.push_back(cv::Point2d(shape.part(22).x(), shape.part(22).y())); //#22 right brow left corner
  pts.push_back(cv::Point2d(shape.part(26).x(), shape.part(26).y())); //#26 right brow right corner
  pts.push_back(cv::Point2d(shape.part(36).x(), shape.part(36).y())); //#36 left eye left corner
  pts.push_back(cv::Point2d(shape.part(39).x(), shape.part(39).y())); //#39 left eye right corner
  pts.push_back(cv::Point2d(shape.part(42).x(), shape.part(42).y())); //#42 right eye left corner
  pts.push_back(cv::Point2d(shape.part(45).x(), shape.part(45).y())); //#45 right eye right corner
  pts.push_back(cv::Point2d(shape.part(31).x(), shape.part(31).y())); //#31 nose left corner
  pts.push_back(cv::Point2d(shape.part(35).x(), shape.part(35).y())); //#35 nose right corner
  pts.push_back(cv::Point2d(shape.part(48).x(), shape.part(48).y())); //#48 mouth left corner
  pts.push_back(cv::Point2d(shape.part(54).x(), shape.part(54).y())); //#54 mouth right corner
  pts.push_back(cv::Point2d(shape.part(57).x(), shape.part(57).y())); //#57 mouth central bottom corner
  pts.push_back(cv::Point2d(shape.part(8).x(), shape.part(8).y()));   //#8 chin corner
}

static void get_head_orientation(Coord width, Coord height,
				 std::vector<cv::Point2d>& image_points, std::vector<float>& euler) {
  static auto model_points = get_3d_model_points();
  double focal_length = width;
  cv::Mat camera_matrix = get_camera_matrix(focal_length, cv::Point2d(width/2.0, width/2.0));
  cv::Mat tv,rv,rm;
  cv::Mat dist_coeffs = cv::Mat::zeros(4,1,cv::DataType<double>::type);
  
  cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs,
	       rv, tv);
  cv::Rodrigues(rv, rm);
  euler = rotationMatrixToEulerAngles(rm);
}

static void downloadSucceeded(emscripten_fetch_t *fetch) {
  membuf buf(fetch->data, fetch->data+fetch->numBytes);
  std::istream ins(&buf);
  deserialize(sp, ins);
  loaded = 1;
  emscripten_fetch_close(fetch);
}

static void downloadFailed(emscripten_fetch_t *fetch) {
  emscripten_fetch_close(fetch);
  loaded = 0;
}

void init_pose() {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess = downloadSucceeded;
  attr.onerror = downloadFailed;
  emscripten_fetch(&attr, facefile);
}

int pose_status() {
  return loaded;
}

static frontal_face_detector face_det = get_frontal_face_detector();

void find_faces(const Img<Rgb8>& img, std::vector<Box>& boxes) {
  std::vector<rectangle> dets = face_det(img);
  for (std::vector<rectangle>::iterator it=dets.begin(); it!=dets.end(); ++it)
    boxes.push_back(Box(it->left(), it->top(), it->right(), it->bottom()));
}

int get_pose(const Img<Rgb8>& img,
	     std::vector<Box>& dets, int scaleDets,
	     std::vector<Point2f>& points, std::vector<float>& euler, Box& bbox) {  
  int closest = -1;
  long maxArea = 0;
  rectangle facert;
  for (size_t i = 0; i < dets.size(); i++) {
    rectangle rt((long)(dets[i].left * scaleDets),
		(long)(dets[i].top * scaleDets),
		 (long)(dets[i].right * scaleDets),
		 (long)(dets[i].bottom * scaleDets));
    if (rt.area() > maxArea) {
      closest = i;
      facert = rt;
      maxArea = rt.area();
    }
  }
  if (closest >= 0) {
    std::vector<cv::Point2d> image_points;
    full_object_detection shape = sp(img, facert);
    get_2d_image_points(shape, image_points);
    get_head_orientation(img.Width(), img.Height(), image_points, euler);
    for (auto pt: image_points)
      points.push_back(Point2f(pt.x, pt.y));
    bbox = Box(facert.left(), facert.top(), facert.right(), facert.bottom());
  }
  return closest;
}
