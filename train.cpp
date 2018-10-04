#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/svm_threaded.h>

using namespace cv;

extern void init_pose();
extern void update_dets(dlib::array2d<dlib::bgr_pixel>& img, std::vector<dlib::rectangle>& dets);
extern void get_pose(dlib::array2d<dlib::bgr_pixel>&, std::vector<dlib::rectangle>&, int scale, std::vector<dlib::full_object_detection>& shapes, int& closest, dlib::rectangle& bbox);
extern Mat alignFace(Mat image, int indices[], std::vector<Point2f>& landmarks, int size);
extern Mat alignFaceH(Mat image, int indices[], std::vector<Point2f>& landmarks, int size);
extern void init_embedding();
extern void embedding(Mat img, float *features);

std::vector<Point2f> get_image_align_points(dlib::full_object_detection &shape,
					    int indices[], int n) {
  std::vector<Point2f> image_pts;
  for (int i = 0; i < n; i++)
    image_pts.push_back(cv::Point2f(shape.part(indices[i]).x(),
				    shape.part(indices[i]).y()));
  return image_pts;
}

static int OUTER_EYES_AND_NOSE[3] = {36, 45, 33};

void calc_features(Mat image, float *features) {
  dlib::array2d<dlib::bgr_pixel> dlibImg;
  dlib::assign_image(dlibImg, dlib::cv_image<dlib::bgr_pixel>(image));

  std::vector<dlib::rectangle> dets;
  update_dets(dlibImg, dets);
  
  int closest;
  dlib::rectangle loc;
  std::vector<dlib::full_object_detection> shapes;
  get_pose(dlibImg, dets, 1, shapes, closest, loc);

  std::vector<Point2f> alignMarks = get_image_align_points(shapes[closest], OUTER_EYES_AND_NOSE, 3);
  Mat face = alignFace(image, OUTER_EYES_AND_NOSE, alignMarks, 96);
  embedding(face, features);
}

typedef dlib::matrix<float,128,1> sample_type;
typedef dlib::linear_kernel<sample_type> lin_kernel;

int main(int argc, char *argv[]) {
  float features[128];
  
  namedWindow("face",1);
  init_pose();
  init_embedding();

  std::vector<std::string> labels;
  std::vector<sample_type> samples;
  for (int arg=1; arg<argc; arg++) {
    DIR *dir = opendir(argv[arg]);
    if (dir == NULL) {
      fprintf(stderr, "Skipping invalid directory %s\n", argv[arg]);
      continue;
    }
    struct dirent *entry;
    char filename[255], label[255];
    char *last = strrchr(argv[arg], '/');
    strcpy(label, last != NULL ? last+1 : argv[arg]);
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0]=='.') continue;
      sprintf(filename, "%s/%s", argv[arg], entry->d_name);
      printf("%s: %s\n", label, filename);
      calc_features(imread(filename, CV_LOAD_IMAGE_COLOR), features);
      sample_type mat;
      for (int i=0; i<128; i++) mat(i) = features[i];
      samples.push_back(mat);
      labels.push_back(label);
    }    
  }
  puts("TRAINING...");
  dlib::svm_multiclass_linear_trainer <lin_kernel,std::string> trainer;
  dlib::multiclass_linear_decision_function<lin_kernel,std::string> df = trainer.train(samples, labels);
  std::ofstream outf("faces.svm");
  dlib::serialize(df, outf);
}
