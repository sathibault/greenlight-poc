#include <opencv2/opencv.hpp>

using namespace cv;

static float MODEL[68][2] = {
  {0.0,0.178569133532},
  {0.00412830601608,0.31259224426},
  {0.0196792970763,0.447709361376},
  {0.0480987184299,0.580072668399},
  {0.10028343838,0.703495242521},
  {0.179997818737,0.812086546497},
  {0.276273077293,0.904677964516},
  {0.384637269508,0.980062775594},
  {0.50735612188,1.0},
  {0.630141094206,0.976111737793},
  {0.738677654937,0.899213830829},
  {0.835474722285,0.805132851835},
  {0.914344649588,0.694562266538},
  {0.964350400537,0.568262043381},
  {0.988705762133,0.432443979527},
  {0.99931225901,0.295292946609},
  {1.0,0.159097178645},
  {0.094855307921,0.0760331339503},
  {0.155348728739,0.0249246520577},
  {0.237747416258,0.0113909916648},
  {0.323134023588,0.0241577799401},
  {0.403669907096,0.0578007122853},
  {0.568646486546,0.0521157030467},
  {0.651281626566,0.0154396603177},
  {0.737960775514,0.0},
  {0.822909251086,0.011915431992},
  {0.887397631947,0.0602570786771},
  {0.488933118582,0.155131886509},
  {0.489915355815,0.243430169338},
  {0.490921477169,0.331765173016},
  {0.492093557886,0.422106987684},
  {0.397399003124,0.480046632864},
  {0.444262519946,0.499067773416},
  {0.49495091481,0.514441322035},
  {0.545582676074,0.496828744401},
  {0.591750872966,0.477226026542},
  {0.194156994659,0.169266902397},
  {0.246003075147,0.136930235517},
  {0.310004957832,0.137356326641},
  {0.363784947083,0.177946861567},
  {0.306369624466,0.190822492738},
  {0.243905145615,0.191381849456},
  {0.618963133941,0.172778125634},
  {0.672494288085,0.129881043641},
  {0.736285702287,0.127908511096},
  {0.788859082386,0.15817115093},
  {0.741151329061,0.181558099712},
  {0.679137144783,0.183703891807},
  {0.307110230073,0.641849676188},
  {0.375970334097,0.610959410564},
  {0.446702576097,0.597050771779},
  {0.497215570365,0.608726414325},
  {0.550020070937,0.595432665705},
  {0.623301628293,0.607091076511},
  {0.695414025867,0.634142882361},
  {0.628067961127,0.709068305177},
  {0.557395398414,0.743447086262},
  {0.500203960126,0.750584384383},
  {0.445287483508,0.745802669321},
  {0.375082071618,0.714542448919},
  {0.337287836556,0.646164628927},
  {0.447014634586,0.640646617543},
  {0.497952035275,0.64496329796},
  {0.551394273556,0.638593667299},
  {0.665022774596,0.639559109047},
  {0.553055606876,0.676479278624},
  {0.498648099493,0.68417641859},
  {0.446572045559,0.67860469929}
};

Mat alignFace(Mat image, int indices[], std::vector<Point2f>& landmarks,
	      int size) {
  Mat aligned;

  std::vector<Point2f> model;
  for (int i = 0; i < 3; i++)
    model.push_back(cv::Point2f(size*MODEL[indices[i]][0],
				size*MODEL[indices[i]][1]));

  Mat T = getAffineTransform(landmarks, model);
  warpAffine(image, aligned, T, Size(size,size));

  return aligned;
}

Mat alignFaceH(Mat image, int indices[], std::vector<Point2f>& landmarks,
	       int size) {
  Mat aligned;

  std::vector<Point2f> model;
  for (int i = 0; i < landmarks.size(); i++)
    model.push_back(cv::Point2f(size*MODEL[indices[i]][0],
				size*MODEL[indices[i]][1]));

  Mat H = findHomography(landmarks, model);
  warpPerspective(image, aligned, H, Size(size,size));

  return aligned;
}