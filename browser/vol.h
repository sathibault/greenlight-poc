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

template <class T, int N, int M, int O> class Vol {
 protected:
  T data[N*M*O];
 public:
  Vol() {}
  Vol(const T x) { for (uint16_t idx = 0; idx < N*M*O; idx++) data[idx] = x; }

  T& operator[](uint16_t idx) {
    return data[idx];
  }
  const T& operator[](uint16_t idx) const {
    return data[idx];
  }
  T& operator()(int row, int col, int elm)  {
    return data[(row*M+col)*O+elm];
  }
  const T& operator()(int row, int col, int elm) const {
    return data[(row*M+col)*O+elm];
  }

  T minimal() const {
    T res = std::numeric_limits<T>::max();
    for (uint16_t idx = 0; idx < N*M*O; idx++) {
      T val = data[idx];
      if (val < res)
	res = val;
    }
    return res;
  }

  T maximal() const {
    T res = std::numeric_limits<T>::min();
    for (uint16_t idx = 0; idx < N*M*O; idx++) {
      T val = data[idx];
      if (val > res)
	res = val;
    }
    return res;
  }


  T sum() const {
    T res = (T)0;
    for (uint16_t idx = 0; idx < N*M*O; idx++)
      res += data[idx];
    return res;
  }
};
