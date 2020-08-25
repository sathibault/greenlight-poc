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

inline void zero(int& a){ a = 0; }
inline void zero(float& a){ a = 0; }

template <class T1, class T2> class Tuple2 {
 public:
  T1 _1;
  T2 _2;
  Tuple2() {}
  Tuple2(T1 a, T2 b) : _1(a), _2(b) {}
  Tuple2(int k) : _1(k), _2(k) {}
  Tuple2(float k) : _1(k), _2(k) {}
};

template <class T1, class T2> void zero(Tuple2<T1,T2>& a) { a._1 = 0; a._2 = 0; }

template <class T1, class T2> Tuple2<T1,T2>& operator+=(Tuple2<T1,T2>& a, const Tuple2<T1,T2>& b) {
    a._1 += b._1; a._2 += b._2; return a;
}
template <class T1, class T2> Tuple2<T1,T2>& operator-=(Tuple2<T1,T2>& a, const Tuple2<T1,T2>& b) {
    a._1 -= b._1; a._2 -= b._2; return a;
}
template <class T1, class T2> Tuple2<T1,T2>& operator/=(Tuple2<T1,T2>& a, int k) {
    a._1 /= (T1)k; a._2 /= (T2)k; return a;
}
template <class T1, class T2> Tuple2<T1,T2>& operator/=(Tuple2<T1,T2>& a, double k) {
    a._1 /= (T1)k; a._2 /= (T2)k; return a;
}

template <class T1, class T2> Tuple2<T1,T2> operator+(Tuple2<T1,T2>& a, const Tuple2<T1,T2>& b) {
  return Tuple2<T1,T2>(a._1 + b._1, a._2 + b._2);
}
template <class T1, class T2> Tuple2<T1,T2> operator-(Tuple2<T1,T2>& a, const Tuple2<T1,T2>& b) {
  return Tuple2<T1,T2>(a._1 - b._1, a._2 - b._2);
}
template <class T1, class T2> Tuple2<T1,T2> operator*(Tuple2<T1,T2>& a, int k) {
  return Tuple2<T1,T2>(a._1 * (T1)k, a._2 * (T2)k);
}
template <class T1, class T2> Tuple2<T1,T2> operator*(Tuple2<T1,T2>& a, double k) {
  return Tuple2<T1,T2>(a._1 * (T1)k, a._2 * (T2)k);
}
template <class T1, class T2> Tuple2<T1,T2> operator/(Tuple2<T1,T2>& a, int k) {
  return Tuple2<T1,T2>(a._1 / (T1)k, a._2 / (T2)k);
}
template <class T1, class T2> Tuple2<T1,T2> operator/(Tuple2<T1,T2>& a, double k) {
  return Tuple2<T1,T2>(a._1 / (T1)k, a._2 / (T2)k);
}

template <class T1, class T2>
  using Pair = Tuple2<T1,T2>;

template <class T1, class T2, class T3> class Tuple3 {
 public:
  T1 _1;
  T2 _2;
  T3 _3;
  Tuple3() {}
  Tuple3(T1 a, T2 b, T3 c) : _1(a), _2(b), _3(c) {}
  Tuple3(int k) : _1(k), _2(k), _3(k) {}
  Tuple3(float k) : _1(k), _2(k), _3(k) {}
};

template <class T1, class T2, class T3> void zero(Tuple3<T1,T2,T3>& a) { a._1 = 0; a._2 = 0; a._3 = 0; }

template <class T1, class T2, class T3> Tuple3<T1,T2,T3>& operator+=(Tuple3<T1,T2,T3>& a, const Tuple3<T1,T2,T3>& b) {
    a._1 += b._1; a._2 += b._2; a._3 += b._3; return a;
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3>& operator-=(Tuple3<T1,T2,T3>& a, const Tuple3<T1,T2,T3>& b) {
    a._1 -= b._1; a._2 -= b._2; a._3 -= b._3; return a;
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3>& operator*=(Tuple3<T1,T2,T3>& a, int k) {
    a._1 *= (T1)k; a._2 *= (T2)k; a._3 *= (T3)k; return a;
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3>& operator*=(Tuple3<T1,T2,T3>& a, double k) {
    a._1 *= (T1)k; a._2 *= (T2)k; a._3 *= (T3)k; return a;
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3>& operator/=(Tuple3<T1,T2,T3>& a, int k) {
    a._1 /= (T1)k; a._2 /= (T2)k; a._3 /= (T3)k; return a;
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3>& operator/=(Tuple3<T1,T2,T3>& a, double k) {
    a._1 /= (T1)k; a._2 /= (T2)k; a._3 /= (T3)k; return a;
}

template <class T1, class T2, class T3> Tuple3<T1,T2,T3> operator+(Tuple3<T1,T2,T3>& a, const Tuple3<T1,T2,T3>& b) {
  return Tuple3<T1,T2,T3>(a._1 + b._1, a._2 + b._2, a._3 + b._3);
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3> operator-(Tuple3<T1,T2,T3>& a, const Tuple3<T1,T2,T3>& b) {
  return Tuple3<T1,T2,T3>(a._1 - b._1, a._2 - b._2, a._3 - b._3);
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3> operator*(Tuple3<T1,T2,T3>& a, int k) {
  return Tuple3<T1,T2,T3>(a._1 * (T1)k, a._2 * (T2)k, a._3 * (T3)k);
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3> operator*(Tuple3<T1,T2,T3>& a, double k) {
  return Tuple3<T1,T2,T3>(a._1 * (T1)k, a._2 * (T2)k, a._3 * (T3)k);
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3> operator/(Tuple3<T1,T2,T3>& a, int k) {
  return Tuple3<T1,T2,T3>(a._1 / (T1)k, a._2 / (T2)k, a._3 / (T3)k);
}
template <class T1, class T2, class T3> Tuple3<T1,T2,T3> operator/(Tuple3<T1,T2,T3>& a, double k) {
  return Tuple3<T1,T2,T3>(a._1 / (T1)k, a._2 / (T2)k, a._3 / (T3)k);
}

template <class T1, class T2, class T3, class T4> class Tuple4 {
 public:
  T1 _1;
  T2 _2;
  T3 _3;
  T4 _4;
  Tuple4() {}
  Tuple4(T1 a, T2 b, T3 c, T4 d) : _1(a), _2(b), _3(c), _4(d) {}
  Tuple4(int k) : _1(k), _2(k), _3(k), _4(k) {}
  Tuple4(float k) : _1(k), _2(k), _3(k), _4(k) {}
};

template <class T1, class T2, class T3, class T4> void zero(Tuple4<T1,T2,T3,T3>& a) {
  a._1 = 0; a._2 = 0; a._3 = 0; a._4 = 0;
}

template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4>& operator+=(Tuple4<T1,T2,T3,T4>& a, const Tuple4<T1,T2,T3,T4>& b) {
  a._1 += b._1; a._2 += b._2; a._3 += b._3; a._4 += b._4; return a;
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4>& operator-=(Tuple4<T1,T2,T3,T4>& a, const Tuple4<T1,T2,T3,T4>& b) {
  a._1 -= b._1; a._2 -= b._2; a._3 -= b._3; a._4 -= b._4; return a;
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4>& operator*=(Tuple4<T1,T2,T3,T4>& a, int k) {
  a._1 *= k; a._2 *= k; a._3 *= k; a._4 *= k; return a;
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4>& operator*=(Tuple4<T1,T2,T3,T4>& a, float k) {
  a._1 *= k; a._2 *= k; a._3 *= k; a._4 *= k; return a;
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4>& operator/=(Tuple4<T1,T2,T3,T4>& a, int k) {
  a._1 /= k; a._2 /= k; a._3 /= k; a._4 /= k; return a;
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4>& operator/=(Tuple4<T1,T2,T3,T4>& a, float k) {
  a._1 /= k; a._2 /= k; a._3 /= k; a._4 /= k; return a;
}

template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4> operator+(Tuple4<T1,T2,T3,T4>& a, const Tuple4<T1,T2,T3,T4>& b) {
  return Tuple4<T1,T2,T3,T4>(a._1 + b._1, a._2 + b._2, a._3 + b._3, a._4 + b._4);
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4> operator-(Tuple4<T1,T2,T3,T4>& a, const Tuple4<T1,T2,T3,T4>& b) {
  return Tuple4<T1,T2,T3,T4>(a._1 - b._1, a._2 - b._2, a._3 - b._3, a._4 - b._4);
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4> operator*(Tuple4<T1,T2,T3,T4>& a, int k) {
  return Tuple4<T1,T2,T3,T4>(a._1 * k, a._2 * k, a._3 * k, a._4 * k);
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4> operator*(Tuple4<T1,T2,T3,T4>& a, float k) {
  return Tuple4<T1,T2,T3,T4>(a._1 * k, a._2 * k, a._3 * k, a._4 * k);
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4> operator/(Tuple4<T1,T2,T3,T4>& a, int k) {
  return Tuple4<T1,T2,T3,T4>(a._1 / k, a._2 / k, a._3 / k, a._4 / k);
}
template <class T1, class T2, class T3, class T4> Tuple4<T1,T2,T3,T4> operator/(Tuple4<T1,T2,T3,T4>& a, float k) {
  return Tuple4<T1,T2,T3,T4>(a._1 / k, a._2 / k, a._3 / k, a._4 / k);
}
