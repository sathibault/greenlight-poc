@echo off
rem call emcc -O2 -DDLIB_ISO_CPP_ONLY=1 -Ic:/tools/dlib-19.16/build/install/include -c dlib.cpp
call emcc -O2 -Ic:/tools/dlib-19.16/build/install/include -I./opencv/install/include/opencv4 -c vision.cpp
call emcc -O2 -c api.cpp
call emcc -O2 -o output.js api.o vision.o dlib.o opencv/install/lib/libopencv_core.a opencv/install/lib/libopencv_calib3d.a opencv/install/lib/opencv4/3rdparty/libzlib.a -s TOTAL_MEMORY=260MB -s FETCH=1 -s MODULARIZE=1 -s "EXPORTED_FUNCTIONS=['_init_models','_model_status','_detect','_get_box','_pose']" -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"
