/*

Copyright 2016 Green Mountain Computing Systems, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

template <class T, int N, int M> class Mat {
 protected:
  T data[N*M];
 public:
  Mat() {}
  Mat(const T x) { clear(x); }

  T& operator[](uint16_t idx) {
    return data[idx];
  }
  const T& operator[](uint16_t idx) const {
    return data[idx];
  }
  T& operator()(int row, int col)  {
    return data[row*M+col];
  }
  const T& operator()(int row, int col) const {
    return data[row*M+col];
  }

  void clear(const T x) { for (uint16_t idx = 0; idx < N*M; idx++) data[idx] = x; }

  T minimal() const {
    T res = std::numeric_limits<T>::max();
    for (uint16_t idx = 0; idx < N*M; idx++) {
      T val = data[idx];
      if (val < res)
	res = val;
    }
    return res;
  }

  T maximal() const {
    T res = std::numeric_limits<T>::min();
    for (uint16_t idx = 0; idx < N*M; idx++) {
      T val = data[idx];
      if (val > res)
	res = val;
    }
    return res;
  }


  T sum() const {
    T res = (T)0;
    for (uint16_t idx = 0; idx < N*M; idx++)
      res += data[idx];
    return res;
  }
};

template <class T, int O, int N, int M> class Mat3D {
 protected:
  T data[O*N*M];
 public:
  Mat3D() {}
  Mat3D(const T x) { clear(x); }
  Mat3D(std::initializer_list<T> values) {
    uint16_t idx = 0;
    for (auto x : values)
      data[idx++] = x;
  }

  void clear(const T x=0) { for (uint16_t idx = 0; idx < O*N*M; idx++) data[idx] = x; }

  T& operator[](uint16_t idx) {
    return data[idx];
  }
  const T& operator[](uint16_t idx) const {
    return data[idx];
  }
  T& operator()(int z, int y, int x)  {
    return data[(z*N+y)*M+x];
  }
  const T& operator()(int z, int y, int x) const {
    return data[(z*N+y)*M+x];
  }

  T minimal() const {
    T res = std::numeric_limits<T>::max();
    for (uint16_t idx = 0; idx < O*N*M; idx++) {
      T val = data[idx];
      if (val < res)
	res = val;
    }
    return res;
  }

  T maximal() const {
    T res = std::numeric_limits<T>::min();
    for (uint16_t idx = 0; idx < O*N*M; idx++) {
      T val = data[idx];
      if (val > res)
	res = val;
    }
    return res;
  }


  T sum() const {
    T res = (T)0;
    for (uint16_t idx = 0; idx < O*N*M; idx++)
      res += data[idx];
    return res;
  }
};
