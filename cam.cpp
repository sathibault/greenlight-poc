#include <stdio.h>
#include <semaphore.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>

#define OPENCV_DISPLAY 0
#define DETECTOR_RATE 2

#if !OPENCV_DISPLAY
#include <dlib/gui_widgets.h>
#endif
#include <dlib/image_processing/render_face_detections.h>

using namespace cv;

extern void init_pose();
extern void update_dets(dlib::array2d<dlib::bgr_pixel>& img, std::vector<dlib::rectangle>& dets);
extern void get_pose(dlib::array2d<dlib::bgr_pixel>&, std::vector<dlib::rectangle>&, int scale, std::vector<dlib::full_object_detection>& shapes, int& closest, dlib::rectangle& bbox);
extern Mat alignFace(Mat image, int indices[], std::vector<Point2f>& landmarks, int size);
extern Mat alignFaceH(Mat image, int indices[], std::vector<Point2f>& landmarks, int size);
extern void init_classify(sem_t *start, sem_t *done);
extern void classify_image(Mat& next);
extern void classify_label(char *buf);

Mat get_camera_matrix(float focal_length, Point2d center) {
  Mat camera_matrix = (Mat_<double>(3,3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
  return camera_matrix;
}

std::vector<float> rotationMatrixToEulerAngles(Mat &R) {
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

#if 0
std::vector<cv::Point3d> get_3d_model_points() {
  std::vector<Point3d> modelPoints;

  modelPoints.push_back(cv::Point3d(0.0f, 0.0f, 0.0f)); //The first must be (0,0,0) while using POSIT
  modelPoints.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));
  modelPoints.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));
  modelPoints.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));
  modelPoints.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));
  modelPoints.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));
  
  return modelPoints;
}
#else
std::vector<cv::Point3d> get_3d_model_points() {
  std::vector<Point3d> object_pts;

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
#endif

#if 0
std::vector<Point2d> get_2d_image_points(dlib::full_object_detection &d) {
  std::vector<Point2d> image_points;
  image_points.push_back( Point2d( d.part(30).x(), d.part(30).y() ) );    // Nose tip
  image_points.push_back( Point2d( d.part(8).x(), d.part(8).y() ) );      // Chin
  image_points.push_back( Point2d( d.part(36).x(), d.part(36).y() ) );    // Left eye left corner
  image_points.push_back( Point2d( d.part(45).x(), d.part(45).y() ) );    // Right eye right corner
  image_points.push_back( Point2d( d.part(48).x(), d.part(48).y() ) );    // Left Mouth corner
  image_points.push_back( Point2d( d.part(54).x(), d.part(54).y() ) );    // Right mouth corner
  return image_points;
}
#else

std::vector<Point2d> get_2d_image_points(dlib::full_object_detection &shape) {
  std::vector<Point2d> image_pts;
            image_pts.push_back(cv::Point2d(shape.part(17).x(), shape.part(17).y())); //#17 left brow left corner
            image_pts.push_back(cv::Point2d(shape.part(21).x(), shape.part(21).y())); //#21 left brow right corner
            image_pts.push_back(cv::Point2d(shape.part(22).x(), shape.part(22).y())); //#22 right brow left corner
            image_pts.push_back(cv::Point2d(shape.part(26).x(), shape.part(26).y())); //#26 right brow right corner
            image_pts.push_back(cv::Point2d(shape.part(36).x(), shape.part(36).y())); //#36 left eye left corner
            image_pts.push_back(cv::Point2d(shape.part(39).x(), shape.part(39).y())); //#39 left eye right corner
            image_pts.push_back(cv::Point2d(shape.part(42).x(), shape.part(42).y())); //#42 right eye left corner
            image_pts.push_back(cv::Point2d(shape.part(45).x(), shape.part(45).y())); //#45 right eye right corner
            image_pts.push_back(cv::Point2d(shape.part(31).x(), shape.part(31).y())); //#31 nose left corner
            image_pts.push_back(cv::Point2d(shape.part(35).x(), shape.part(35).y())); //#35 nose right corner
            image_pts.push_back(cv::Point2d(shape.part(48).x(), shape.part(48).y())); //#48 mouth left corner
            image_pts.push_back(cv::Point2d(shape.part(54).x(), shape.part(54).y())); //#54 mouth right corner
            image_pts.push_back(cv::Point2d(shape.part(57).x(), shape.part(57).y())); //#57 mouth central bottom corner
            image_pts.push_back(cv::Point2d(shape.part(8).x(), shape.part(8).y()));   //#8 chin corner
  return image_pts;
}
#endif

std::vector<Point2f> get_image_align_points(dlib::full_object_detection &shape,
					    int indices[], int n) {
  std::vector<Point2f> image_pts;
  for (int i = 0; i < n; i++)
    image_pts.push_back(cv::Point2f(shape.part(indices[i]).x(),
				    shape.part(indices[i]).y()));
  return image_pts;
}

static int OUTER_EYES_AND_NOSE[3] = {36, 45, 33};
static int LANDMARK_INDICES[14] = {17,21,22,26,36,39,42,45,31,35,48,54,57,8};
static int PLANAR_LANDMARKS[5] = {17,22,36,45,8};

static std::vector<Point3d> model_points = get_3d_model_points();

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame, small;
    Mat edges;
    dlib::array2d<dlib::bgr_pixel> dlibSmImg, dlibImg, dlibFace;
    std::vector<dlib::full_object_detection> shapes;
    int no;
    double t, fps;
    int closest;
    dlib::rectangle loc;
    std::vector<dlib::rectangle> dets;

    Mat face = Mat(96, 96, CV_8UC3, Scalar(128,128,128));

#if OPENCV_DISPLAY
    namedWindow("edges",1);
#else
    dlib::image_window win;
    dlib::image_window win2;
#endif

    bool needClass = true;
    int classTimeout = 0;
    sem_t classStart, classDone;
    char label[80]={0};
    
    sem_init(&classStart, 0, 0);
    sem_init(&classDone, 0, 0);
    
    init_pose();
    init_classify(&classStart, &classDone);
    
    no = 0;
    t = (double)getTickCount();
    fps = 0.0;
    for(;;) {
      cap >> frame; // get a new frame from camera
      resize(frame, small, Size(), 0.5, 0.5);
      dlib::assign_image(dlibSmImg, dlib::cv_image<dlib::bgr_pixel>(small));
      dlib::assign_image(dlibImg, dlib::cv_image<dlib::bgr_pixel>(frame));

      if ((no % DETECTOR_RATE) == 0)
	update_dets(dlibSmImg, dets);
      get_pose(dlibImg, dets, 2, shapes, closest, loc);

      bool greenlight = false;
      
      if (closest >= 0) {
	std::vector<Point2d> image_points=get_2d_image_points(shapes[closest]);
	double focal_length=frame.cols;
	Mat camera_matrix=get_camera_matrix(focal_length,
					  Point2d(frame.cols/2,
						  frame.rows/2));
	Mat tv,rv,rm;
	Mat dist_coeffs = Mat::zeros(4,1,DataType<double>::type);
      
	solvePnP(model_points, image_points, camera_matrix, dist_coeffs,
		 rv, tv);
	Rodrigues(rv, rm);
	std::vector<float> euler = rotationMatrixToEulerAngles(rm);
      //printf("\r %d %d %0.2f %0.2f %0.2f    ", rv.rows, rv.cols, rv.at<double>(0,0), rv.at<double>(1,0), rv.at<double>(2,0));
	printf("\r %0.2f %0.2f %0.2f %0.2f fps          ", euler[0], euler[1], euler[2], fps);
	fflush(stdout);
	greenlight |= fabsf(euler[1]) < 0.25f;
	//greenlight |= fabs(rv.at<double>(2,0)) < 0.7 && fabs(-3.14 - rv.at<double>(0,0)) < 0.14;

#if 1
	std::vector<Point2f> alignMarks = get_image_align_points(shapes[closest], OUTER_EYES_AND_NOSE, 3);
	face = alignFace(frame, OUTER_EYES_AND_NOSE, alignMarks, 96);
#else
	std::vector<Point2f> alignMarks = get_image_align_points(shapes[closest], PLANAR_LANDMARKS, 5);
	face = alignFaceH(frame, PLANAR_LANDMARKS, alignMarks, 96);
#endif
	if (greenlight && needClass) {
	  classify_image(face);
	  sem_post(&classStart);
	  needClass = false;
	}
      }
      
      if (classTimeout > 0) {
	classTimeout--;
	if (classTimeout == 0)
	  needClass = true;
      }
      
      no++;
      if ((no % 100) == 0) {
	double dt = ((double)getTickCount() - t)/getTickFrequency();
	fps = 100.0/dt;
        t = (double)getTickCount();
      }

#if OPENCV_DISPLAY
      imshow("edges", frame);
      if (waitKey(1) >= 0) {
	imwrite("snapshot.png",frame);
	break;
      }
#else
      win.clear_overlay();
      win.set_image(dlibImg);
      if (greenlight)
	win.add_overlay(render_face_detections(shapes, dlib::rgb_pixel(0,255,0)));
      else
	win.add_overlay(render_face_detections(shapes, dlib::rgb_pixel(255,0,0)));
      if (label[0]) {
	dlib::rectangle rect(10,110,20,110);
	dlib::rgb_pixel color(255,255,255);
	win.add_overlay(dlib::image_window::overlay_rect(rect,color,label));
      }
      dlib::assign_image(dlibFace, dlib::cv_image<dlib::bgr_pixel>(face));
      win2.set_image(dlibFace);
      if (sem_trywait(&classDone) == 0) {
	classify_label(label);
	classTimeout = 15;
      }
#endif
    }
    return 0;
}
