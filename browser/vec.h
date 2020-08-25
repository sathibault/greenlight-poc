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

template <class T, int len> class Vec {
 protected:
  T data[len];
 public:
  Vec() {}
  Vec(const T x) { clear(x); }
  T& operator[](const uint16_t idx) {
    return data[idx];
  }
  const T& operator[](const uint16_t idx) const {
    return data[idx];
  }

  void clear(const T x) { for (uint16_t idx = 0; idx < len; idx++) data[idx] = x; }

  bool operator==(const Vec<T,len>& other) const {
    for (int i = 0; i < len; i++)
      if (data[i] != other.data[i]) return false;
    return true;
  }

  bool operator!=(const Vec<T,len>& other) const {
    for (int i = 0; i < len; i++)
      if (data[i] != other.data[i]) return true;
    return false;
  }

  T minimal() const {
    T res = std::numeric_limits<T>::max();
    for (uint16_t idx = 0; idx < len; idx++) {
      T val = data[idx];
      if (val < res)
	res = val;
    }
    return res;
  }

  uint16_t argmin() const {
    uint16_t arg = 0;
    T res = std::numeric_limits<T>::max();
    for (uint16_t idx = 0; idx < len; idx++) {
      T val = data[idx];
      if (val < res) {
	res = val;
	arg = idx;
      }
    }
    return arg;
  }

  T maximal() const {
    T res = std::numeric_limits<T>::min();
    for (uint16_t idx = 0; idx < len; idx++) {
      T val = data[idx];
      if (val > res)
	res = val;
    }
    return res;
  }

  T argmax() const {
    uint16_t arg = 0;
    T res = std::numeric_limits<T>::min();
    for (uint16_t idx = 0; idx < len; idx++) {
      T val = data[idx];
      if (val > res) {
	res = val;
	arg = idx;
      }
    }
    return arg;
  }

  T sum() const {
    T res = (T)0;
    for (uint16_t idx = 0; idx < len; idx++)
      res += data[idx];
    return res;
  }

  template <class U> Vec<U,len> map(std::function<U (const T&)> fun) const {
    Vec<U,len> result;
    for (uint16_t idx = 0; idx < len; idx++)
      result[idx] = fun(data[idx]);
    return result;
  }

  template <class U, class V>
  void spread(const U& x, Vec<V,len> result,
	 std::function<V (const T&, const U&)> fun) const {
    for (uint16_t idx = 0; idx < len; idx++)
      result[idx] = fun(data[idx], x);
  }

  template <class U, class V> Vec<V,len>
  zip(const Vec<U,len>& x,
	 std::function<V (const T&, const U&)> fun) const {
    Vec<V,len> result;
    for (uint16_t idx = 0; idx < len; idx++)
      result[idx] = fun(data[idx], x[idx]);
    return result;
  }
};

template <class T, int N> Vec<T,N> operator+(const Vec<T,N>& a, T c) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] + c;
  return res;
}
template <class T, int N> Vec<T,N> operator-(const Vec<T,N>& a, T c) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] - c;
  return res;
}
template <class T, int N> Vec<T,N> operator+(const Vec<T,N>& a, const Vec<T,N>& b) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] + b[i];
  return res;
}
template <class T, int N> Vec<T,N> operator-(const Vec<T,N>& a, const Vec<T,N>& b) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] - b[i];
  return res;
}
template <class T, int N> Vec<T,N> operator*(const Vec<T,N>& a, int k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] * (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator*(const Vec<T,N>& a, double k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] * (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator*(const Vec<T,N>& a, const Vec<T,N>& b) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] * b[i];
  return res;
}
template <class T, int N> Vec<T,N> operator/(const Vec<T,N>& a, int k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] / (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator/(const Vec<T,N>& a, unsigned k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] / (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator/(const Vec<T,N>& a, uint16_t k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] / (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator/(const Vec<T,N>& a, uint8_t k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] / (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator/(const Vec<T,N>& a, double k) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i] / (T)k;
  return res;
}
template <class T, int N> Vec<T,N> operator>>(const Vec<T,N>& a, uint8_t s) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i]>>s;
  return res;
}
template <class T, int N> Vec<T,N> operator<<(const Vec<T,N>& a, uint8_t s) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = a[i]<<s;
  return res;
}

template <class T, int N> Vec<float,N> tofloat(const Vec<T,N>& v) {
  Vec<float,N> res;
  for (int i = 0; i < N; i++)
    res[i] = (float)v[i];
  return res;
}

template <class T, int N, class U> void tostd(const Vec<T,N>& vec, std::vector<U>& sv) {
  for (int i = 0; i < N; i++)
    sv.push_back(vec[i]);
}

template <class T, int N> Vec<float,N> sqrtf(const Vec<T,N>& v) {
  Vec<float,N> res;
  for (int i = 0; i < N; i++)
    res[i] = sqrtf((float)v[i]);
  return res;
}

template <class T, int N> Vec<T,N> absdiff(const Vec<T,N>& a, const Vec<T,N>& b) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = (a[i] >= b[i]) ? a[i]-b[i] : b[i]-a[i];
  return res;
}

template <class T, int N> Vec<T,N> vecmin(const Vec<T,N>& a, const Vec<T,N>& b) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = (a[i] <= b[i]) ? a[i] : b[i];
  return res;
}
template <class T, int N> Vec<T,N> vecmax(const Vec<T,N>& a, const Vec<T,N>& b) {
  Vec<T,N> res;
  for (int i = 0; i < N; i++)
    res[i] = (a[i] >= b[i]) ? a[i] : b[i];
  return res;
}

template <class T, int N> int argmin(const Vec<T,N>& v) {
  T res = std::numeric_limits<T>::max();
  int arg = -1;
  for (uint16_t idx = 0; idx < N; idx++) {
    T val = v[idx];
    if (val <= res) {
      res = val;
      arg = idx;
    }
  }
  return arg;
}

template <class T, int N> int argmax(const Vec<T,N>& v) {
  T res = std::numeric_limits<T>::min();
  int arg = -1;
  for (uint16_t idx = 0; idx < N; idx++) {
    T val = v[idx];
    if (val >= res) {
      res = val;
      arg = idx;
    }
  }
  return arg;
}
