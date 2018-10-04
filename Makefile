DLIB_INC=/home/thibault/dlib
DLIB_LIB=/home/thibault/dlib/build/dlib_build/libdlib.a
TORCH_INC=/usr/local/cpp-torch/include
TORCH_LIB=-L/usr/local/cpp-torch/lib -lcpptorch -lTH -lTHNN
CPPFLAGS=-g --std=c++11 -Wno-deprecated -I$(DLIB_INC) -I$(TORCH_INC) -Wreturn-type -DDLIB_PNG_SUPPORT -DDLIB_JPEG_SUPPORT -DDLIB_USE_BLAS -DDLIB_USE_LAPACK -O3 -DNDEBUG -msse4
LDFLAGS=-Wreturn-type -DDLIB_PNG_SUPPORT -DDLIB_JPEG_SUPPORT -DDLIB_USE_BLAS -DDLIB_USE_LAPACK -O3 -DNDEBUG

cam: cam.o pose.o align.o classify_bg.o embedding.o
	g++ $(LDFLAGS) -o $@ $^ $(DLIB_LIB) $(TORCH_LIB) -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_core -lblas -llapack -lpthread -lX11

classify: classify.o pose.o align.o embedding.o
	g++ $(LDFLAGS) -o $@ $^ $(DLIB_LIB) $(TORCH_LIB) -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_core -lblas -llapack -lpthread -lX11

train: train.o pose.o align.o embedding.o
	g++ $(LDFLAGS) -o $@ $^ $(DLIB_LIB) $(TORCH_LIB) -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_core -lblas -llapack -lpthread -lX11

dpmallick: dpmallick.o
	g++ $(LDFLAGS) -o $@ dpmallick.o -ldlib -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_core -ldlib -lblas -llapack

%.o: %.cpp
	g++ $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f cam dpmallick *.o
