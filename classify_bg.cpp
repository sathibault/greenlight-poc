#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <dlib/svm_threaded.h>

using namespace cv;

extern void init_embedding();
extern void embedding(const unsigned char *img, float *features);

static void *classifier(void *arg);

float diff_time_ms(struct timespec start, struct timespec end) {
  return (1e9 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec)/1e6;
}

typedef dlib::matrix<float,128,1> sample_type;
typedef dlib::linear_kernel<sample_type> lin_kernel;

static pthread_t thread;
static sem_t *semStart, *semDone;

static Mat image;
static dlib::multiclass_linear_decision_function<lin_kernel,std::string> df;
static std::pair<std::string,float> guess;

void init_classify(sem_t *start, sem_t *done) {
  semStart = start;
  semDone = done;

  init_embedding();
  
  std::ifstream inf("faces.svm");
  dlib::deserialize(df, inf);

  pthread_create(&thread, NULL, classifier, NULL);
}

void classify_image(Mat& next) {
  image = next.clone();
}

void classify_label(char *buf) {
  sprintf(buf, "%s (%f)", guess.first.c_str(), guess.second);
}

static void *classifier(void *arg) {
  float features[128];
  sample_type mat;
  struct timespec t1,t2,t3;

  while (sem_wait(semStart) == 0) {
    clock_gettime(CLOCK_MONOTONIC, &t1);
    embedding(image.ptr(0), features);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    for (int i=0; i<128; i++) mat(i) = features[i];
    guess = df.predict(mat);
    clock_gettime(CLOCK_MONOTONIC, &t3);
    //printf("\n%s %f (%0.1f %0.1f)\n", guess.first.c_str(), guess.second, diff_time_ms(t1, t2), diff_time_ms(t2, t3));
    sem_post(semDone);
  }
  return NULL;
}
