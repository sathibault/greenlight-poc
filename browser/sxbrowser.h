#ifndef SXBROWSER_H
#define SXBROWSER_H

#include <map>
#include <string>

#include "vec.h"
#include "mat.h"
#include "vol.h"
#include "tuple.h"
#include "metadata.h"

typedef Pair<float,float> Point2f;

#define SXMIN(x,y) ((x)<(y) ? (x) : (y))
#define SXMAX(x,y) ((x)>(y) ? (x) : (y))

#define MAX_COORD 65535u
typedef uint16_t Coord;

typedef uint16_t Gray16;
typedef uint8_t Gray8;

#ifndef SL_SYNTH
class Rgb8 {
 public:
  uint32_t rgb;
  Rgb8() {}
  Rgb8(uint8_t r, uint8_t g, uint8_t b) { rgb = (uint32_t)b<<16 | (uint32_t)g<<8 | r; }
  Rgb8(uint8_t v) { rgb = (uint32_t)v<<16 | (uint32_t)v<<8 | v; }
  Rgb8(const Rgb8& other) { rgb = other.rgb; }
  template <typename U> Rgb8(const Vec<U,3>& other) {
    uint8_t r = (other[0] >= 0) ? ((other[0] <= 255) ? (uint8_t)other[0] : 255) : 0;
    uint8_t g = (other[1] >= 0) ? ((other[1] <= 255) ? (uint8_t)other[1] : 255) : 0;
    uint8_t b = (other[2] >= 0) ? ((other[2] <= 255) ? (uint8_t)other[2] : 255) : 0;
    rgb = (uint32_t)b<<16 | (uint32_t)g<<8 | r;
  }
  inline void set(uint8_t r, uint8_t g, uint8_t b) {
    rgb = (uint32_t)b<<16 | (uint32_t)g<<8 | r;
  }
  inline uint8_t R() const { return (uint8_t)rgb; }
  inline uint8_t G() const { return (uint8_t)(rgb>>8); }
  inline uint8_t B() const { return (uint8_t)(rgb>>16); }
  Rgb8& operator+=(const Rgb8 &other) {
    set(R()+other.R(), G()+other.G(), B()+other.B());
    return *this;
  }
  Rgb8& operator-=(const Rgb8 &other) {
    set(R()-other.R(), G()-other.G(), B()-other.B());
    return *this;
  }
  bool operator ==(const Rgb8& o) const {
    return rgb == o.rgb;
  }
  bool operator !=(const Rgb8& o) const {
    return rgb != o.rgb;
  }
  bool operator <(const Rgb8& o) const {
    return R()<o.R() && G()<o.G() && B()<o.B();
  }
  bool operator <=(const Rgb8& o) const {
    return R()<=o.R() && G()<=o.G() && B()<=o.B();
  }
  bool operator >(const Rgb8& o) const {
    return R()>o.R() && G()>o.G() && B()>o.B();
  }
  bool operator >=(const Rgb8& o) const {
    return R()>=o.R() && G()>=o.G() && B()>=o.B();
  }
};


inline Rgb8 operator+(const Rgb8& x, const Rgb8& y) {
  return Rgb8(x.R()+y.R(), x.G()+y.G(), x.B()+y.B());
}
inline Rgb8 operator-(const Rgb8& x, const Rgb8& y) {
  return Rgb8(x.R()-y.R(), x.G()-y.G(), x.B()-y.B());
}

inline Rgb8 operator*(const Rgb8& x, uint32_t k) {
  return Rgb8(x.R()*k, x.G()*k, x.B()*k);
}
inline Rgb8 operator/(const Rgb8& x, uint32_t k) {
  return Rgb8(x.R()/k, x.G()/k, x.B()/k);
}
inline Rgb8 operator/(const Rgb8& x, uint16_t k) {
  return Rgb8(x.R()/k, x.G()/k, x.B()/k);
}
inline Rgb8 operator/(const Rgb8& x, uint8_t k) {
  return Rgb8(x.R()/k, x.G()/k, x.B()/k);
}
inline Rgb8 operator>>(const Rgb8& x, uint8_t s) {
  return Rgb8(x.R()>>s, x.G()>>s, x.B()>>s);
}
inline Rgb8 operator<<(const Rgb8& x, uint8_t s) {
  return Rgb8(x.R()<<s, x.G()<<s, x.B()<<s);
}
#endif

template <typename T> class RgbV : public Vec<T,3> {
 public:
  RgbV() {}
  RgbV(T v) {
    this->data[0] = v;
    this->data[1] = v;
    this->data[2] = v;
  }
  RgbV(T r, T g, T b) {
    this->data[0] = r;
    this->data[1] = g;
    this->data[2] = b;
  }
#ifndef SL_SYNTH
  RgbV(const Rgb8& other) {
    this->data[0] = other.R();
    this->data[1] = other.G();
    this->data[2] = other.B();
  }
#endif
  template <typename U> RgbV(const Vec<U,3>& other) {
    this->data[0] = other[0];
    this->data[1] = other[1];
    this->data[2] = other[2];
  }
  inline void set(uint8_t r, uint8_t g, uint8_t b) {
    this->data[0] = r;
    this->data[1] = g;
    this->data[2] = b;
  }
  inline T R() const { return this->data[0]; }
  inline T G() const { return this->data[1]; }
  inline T B() const { return this->data[2]; }
#ifndef SL_SYNTH
  RgbV& operator+=(const Rgb8 &other) {
    this->data[0] = R()+other.R();
    this->data[1] = G()+other.G();
    this->data[2] = B()+other.B();
    return *this;
  }
  RgbV& operator-=(const Rgb8 &other) {
    this->data[0] = R()-other.R();
    this->data[1] = G()-other.G();
    this->data[2] = B()-other.B();
    return *this;
  }
#endif
  template <typename U> RgbV& operator+=(const RgbV<U> &other) {
    this->data[0] = R()+other.R();
    this->data[1] = G()+other.G();
    this->data[2] = B()+other.B();
    return *this;
  }
  template <typename U> RgbV& operator-=(const RgbV<U> &other) {
    this->data[0] = R()-other.R();
    this->data[1] = G()-other.G();
    this->data[2] = B()-other.B();
    return *this;
  }
  bool operator ==(const RgbV<T>& o) const {
    return R()==o.R() && G()==o.G() && B()==o.B();
  }
  bool operator !=(const RgbV<T>& o) const {
    return R()!=o.R() || G()!=o.G() || B()!=o.B();
  }
  bool operator <(const RgbV<T>& o) const {
    return R()<o.R() && G()<o.G() && B()<o.B();
  }
  bool operator <=(const RgbV<T>& o) const {
    return R()<=o.R() && G()<=o.G() && B()<=o.B();
  }
  bool operator >(const RgbV<T>& o) const {
    return R()>o.R() && G()>o.G() && B()>o.B();
  }
  bool operator >=(const RgbV<T>& o) const {
    return R()>=o.R() && G()>=o.G() && B()>=o.B();
  }
};

#ifdef SL_SYNTH
typedef RgbV<uint8_t> Rgb8;
#endif
typedef RgbV<int8_t> Rgb8i;
typedef RgbV<uint16_t> Rgb16;
typedef RgbV<int16_t> Rgb16i;
typedef RgbV<uint32_t> Rgb32;
typedef RgbV<int32_t> Rgb32i;
typedef RgbV<float> Rgb32f;

#ifndef SL_SYNTH
class Rgba8 {
 public:
  uint32_t rgba;
  Rgba8() {rgba=0;}
  Rgba8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { rgba = (uint32_t)a<<24 | (uint32_t)b<<16 | (uint32_t)g<<8 | r; }
  Rgba8(uint8_t v) { rgba = (uint32_t)v<<24 | (uint32_t)v<<16 | (uint32_t)v<<8 | v; }
  Rgba8(const Rgba8& other) { rgba = other.rgba; }
  inline uint8_t R() const { return (uint8_t)rgba; }
  inline uint8_t G() const { return (uint8_t)(rgba>>8); }
  inline uint8_t B() const { return (uint8_t)(rgba>>16); }
  inline uint8_t A() const { return (uint8_t)(rgba>>24); }
  inline void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    rgba = (uint32_t)a<<24 | (uint32_t)b<<16 | (uint32_t)g<<8 | r;
  }
  bool operator ==(const Rgba8& o) const {
    return rgba == o.rgba;
  }
  bool operator !=(const Rgba8& o) const {
    return rgba != o.rgba;
  }
  bool operator <(const Rgba8& o) const {
    return R()<o.R() && G()<o.G() && B()<o.B() && A()<o.A();
  }
  bool operator <=(const Rgba8& o) const {
    return R()<=o.R() && G()<=o.G() && B()<=o.B() && A()<=o.A();
  }
  bool operator >(const Rgba8& o) const {
    return R()>o.R() && G()>o.G() && B()>o.B() && A()>o.A();
  }
  bool operator >=(const Rgba8& o) const {
    return R()>=o.R() && G()>=o.G() && B()>=o.B() && A()>=o.A();
  }
};

inline Rgba8 operator+(const Rgba8& x, const Rgba8& y) {
  return Rgba8(x.R()+y.R(), x.G()+y.G(), x.B()+y.B(), x.A()+y.A());
}
inline Rgba8 operator-(const Rgba8& x, const Rgba8& y) {
  return Rgba8(x.R()-y.R(), x.G()-y.G(), x.B()-y.B(), x.A()-y.A());
}

#endif

template <typename T> class RgbaV : public Vec<T,4> {
 public:
  RgbaV() {}
  RgbaV(T v) {
    this->data[0] = v;
    this->data[1] = v;
    this->data[2] = v;
    this->data[3] = v;
  }
  RgbaV(T r, T g, T b, T a) {
    this->data[0] = r;
    this->data[1] = g;
    this->data[2] = b;
    this->data[3] = a;
  }
#ifndef SL_SYNTH
  RgbaV(const Rgba8& other) {
    this->data[0] = other.R();
    this->data[1] = other.G();
    this->data[2] = other.B();
    this->data[3] = other.A();
  }
  RgbaV(const Rgb8& other) {
    this->data[0] = other.R();
    this->data[1] = other.G();
    this->data[2] = other.B();
    this->data[3] = std::numeric_limits<T>::max();
  }
#endif
  template <typename U> RgbaV(const Vec<U,4>& other) {
    this->data[0] = other[0];
    this->data[1] = other[1];
    this->data[2] = other[2];
    this->data[3] = other[3];
  }
  template <typename U> RgbaV(const Vec<U,3>& other) {
    this->data[0] = other[0];
    this->data[1] = other[1];
    this->data[2] = other[2];
    this->data[3] = std::numeric_limits<T>::max();
  }
  inline void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->data[0] = r;
    this->data[1] = g;
    this->data[2] = b;
    this->data[3] = a;
  }
  inline T R() const { return this->data[0]; }
  inline T G() const { return this->data[1]; }
  inline T B() const { return this->data[2]; }
  inline T A() const { return this->data[3]; }
#ifndef SL_SYNTH
  RgbaV& operator+=(const Rgba8 &other) {
    this->data[0] = R()+other.R();
    this->data[1] = G()+other.G();
    this->data[2] = B()+other.B();
    this->data[3] = A()+other.A();
    return *this;
  }
  RgbaV& operator-=(const Rgba8 &other) {
    this->data[0] = R()-other.R();
    this->data[1] = G()-other.G();
    this->data[2] = B()-other.B();
    this->data[3] = A()-other.A();
    return *this;
  }
#endif
  template <typename U> RgbaV& operator+=(const RgbaV<U> &other) {
    this->data[0] = R()+other.R();
    this->data[1] = G()+other.G();
    this->data[2] = B()+other.B();
    this->data[3] = A()+other.A();
    return *this;
  }
  template <typename U> RgbaV& operator-=(const RgbaV<U> &other) {
    this->data[0] = R()-other.R();
    this->data[1] = G()-other.G();
    this->data[2] = B()-other.B();
    this->data[3] = A()-other.A();
    return *this;
  }
  bool operator ==(const RgbaV<T>& o) const {
    return R()==o.R() && G()==o.G() && B()==o.B() && A()==o.A();
  }
  bool operator !=(const RgbaV<T>& o) const {
    return R()!=o.R() && G()!=o.G() && B()!=o.B() && A()!=o.A();
  }
  bool operator <(const RgbaV<T>& o) const {
    return R()<o.R() && G()<o.G() && B()<o.B() && A()<o.A();
  }
  bool operator <=(const RgbaV<T>& o) const {
    return R()<=o.R() && G()<=o.G() && B()<=o.B() && A()<=o.A();
  }
  bool operator >(const RgbaV<T>& o) const {
    return R()>o.R() && G()>o.G() && B()>o.B() && A()>o.A();
  }
  bool operator >=(const RgbaV<T>& o) const {
    return R()>=o.R() && G()>=o.G() && B()>=o.B() && A()>=o.A();
  }
};

#ifdef SL_SYNTH
typedef RgbaV<uint8_t> Rgba8;
#endif
typedef RgbaV<uint16_t> Rgba16;
typedef RgbaV<uint32_t> Rgba32;
typedef RgbaV<int8_t> Rgba8i;
typedef RgbaV<int16_t> Rgba16i;
typedef RgbaV<int32_t> Rgba32i;
typedef RgbaV<float> Rgba32f;

#ifndef SL_SYNTH
class Hsv8 {
 public:
  uint32_t hsv;
  Hsv8() {}
  Hsv8(uint8_t h, uint8_t s, uint8_t v) { hsv = (uint32_t)h<<16 | (uint32_t)s<<8 | v; }
  Hsv8(uint32_t val) { hsv = val; }
  Hsv8(const Hsv8& other) { hsv = other.hsv; }
  inline uint8_t H() const { return (uint8_t)(hsv>>16); }
  inline uint8_t S() const { return (uint8_t)(hsv>>8); }
  inline uint8_t V() const { return (uint8_t)hsv; }
  inline void set(uint8_t h, uint8_t s, uint8_t v) { hsv = (uint32_t)h<<16 | (uint32_t)s<<8 | v; }
  bool operator ==(const Hsv8& o) const {
    return hsv == o.hsv;
  }
  bool operator !=(const Hsv8& o) const {
    return hsv != o.hsv;
  }
  bool operator <(const Hsv8& o) const {
    return H()<o.H() && S()<o.S() && V()<o.V();
  }
  bool operator <=(const Hsv8& o) const {
    return H()<=o.H() && S()<=o.S() && V()<=o.V();
  }
  bool operator >(const Hsv8& o) const {
    return H()>o.H() && S()>o.S() && V()>o.V();
  }
  bool operator >=(const Hsv8& o) const {
    return H()>=o.H() && S()>=o.S() && V()>=o.V();
  }
};
#endif

template <typename T> class HsvV : public Vec<T,3> {
 public:
  HsvV() {}
  HsvV(T v) {
    this->data[0] = v;
    this->data[1] = v;
    this->data[2] = v;
  }
  HsvV(T h, T s, T v) {
    this->data[0] = h;
    this->data[1] = s;
    this->data[2] = v;
  }
  template <typename U> HsvV(const Vec<U,3>& other) {
    this->data[0] = other[0];
    this->data[1] = other[1];
    this->data[2] = other[2];
  }
  inline void set(uint8_t h, uint8_t s, uint8_t v) {
    this->data[0] = h;
    this->data[1] = s;
    this->data[2] = v;
  }
  inline T H() const { return this->data[0]; }
  inline T S() const { return this->data[1]; }
  inline T V() const { return this->data[2]; }
  bool operator ==(const HsvV<T>& o) const {
    return H()==o.H() && S()==o.S() && V()==o.V();
  }
  bool operator !=(const HsvV<T>& o) const {
    return H()!=o.H() && S()!=o.S() && V()!=o.V();
  }
  bool operator <(const HsvV<T>& o) const {
    return H()<o.H() && S()<o.S() && V()<o.V();
  }
  bool operator <=(const HsvV<T>& o) const {
    return H()<=o.H() && S()<=o.S() && V()<=o.V();
  }
  bool operator >(const HsvV<T>& o) const {
    return H()>o.H() && S()>o.S() && V()>o.V();
  }
  bool operator >=(const HsvV<T>& o) const {
    return H()>=o.H() && S()>=o.S() && V()>=o.V();
  }
};

#ifdef SL_SYNTH
typedef HsvV<uint8_t> Hsv8;
#endif

#ifndef SL_SYNTH
class Yuv8 {
 public:
  uint32_t yuv;
  Yuv8() {}
  Yuv8(uint8_t y, uint8_t u, uint8_t v) { yuv = (uint32_t)y<<16 | (uint32_t)u<<8 | v; }
  Yuv8(uint32_t val) { yuv = val; }
  Yuv8(const Yuv8& other) { yuv = other.yuv; }
  inline uint8_t Y() const { return (uint8_t)(yuv>>16); }
  inline uint8_t U() const { return (uint8_t)(yuv>>8); }
  inline uint8_t V() const { return (uint8_t)yuv; }
  inline void set(uint8_t y, uint8_t u, uint8_t v) { yuv = (uint32_t)y<<16 | (uint32_t)u<<8 | v; }
  bool operator ==(const Yuv8& o) const {
    return yuv == o.yuv;
  }
  bool operator !=(const Yuv8& o) const {
    return yuv != o.yuv;
  }
  bool operator <(const Yuv8& o) const {
    return Y()<o.Y() && U()<o.U() && V()<o.V();
  }
  bool operator <=(const Yuv8& o) const {
    return Y()<=o.Y() && U()<=o.U() && V()<=o.V();
  }
  bool operator >(const Yuv8& o) const {
    return Y()>o.Y() && U()>o.U() && V()>o.V();
  }
  bool operator >=(const Yuv8& o) const {
    return Y()>=o.Y() && U()>=o.U() && V()>=o.V();
  }
};
#endif

template <typename T> class YuvV : public Vec<T,3> {
 public:
  YuvV() {}
  YuvV(T v) {
    this->data[0] = v;
    this->data[1] = v;
    this->data[2] = v;
  }
  YuvV(T y, T u, T v) {
    this->data[0] = y;
    this->data[1] = u;
    this->data[2] = v;
  }
  template <typename U> YuvV(const Vec<U,3>& other) {
    this->data[0] = other[0];
    this->data[1] = other[1];
    this->data[2] = other[2];
  }
  inline void set(uint8_t y, uint8_t u, uint8_t v) {
    this->data[0] = y;
    this->data[1] = u;
    this->data[2] = v;
  }
  inline T Y() const { return this->data[0]; }
  inline T U() const { return this->data[1]; }
  inline T V() const { return this->data[2]; }
  bool operator ==(const YuvV<T>& o) const {
    return Y()==o.Y() && U()==o.U() && V()==o.V();
  }
  bool operator !=(const YuvV<T>& o) const {
    return Y()!=o.Y() && U()!=o.U() && V()!=o.V();
  }
  bool operator <(const YuvV<T>& o) const {
    return Y()<o.Y() && U()<o.U() && V()<o.V();
  }
  bool operator <=(const YuvV<T>& o) const {
    return Y()<=o.Y() && U()<=o.U() && V()<=o.V();
  }
  bool operator >(const YuvV<T>& o) const {
    return Y()>o.Y() && U()>o.U() && V()>o.V();
  }
  bool operator >=(const YuvV<T>& o) const {
    return Y()>=o.Y() && U()>=o.U() && V()>=o.V();
  }
};

#ifdef SL_SYNTH
typedef YuvV<uint8_t> Yuv8;
#endif


typedef Rgb8 Rgb;
typedef Rgba8 Rgba;
typedef Hsv8 Hsv;
typedef Yuv8 Yuv;
typedef Gray8 Gray;


template <typename T> class Pixel {
 public:
  Coord x, y;
  T value;
};

inline unsigned char parseHex(const char *cp) {
  const unsigned char *byte=(const unsigned char *)cp;
  unsigned char b;

  b = 0;
  if ((byte[0]>='0')&&(byte[0]<='9'))
    b=byte[0]-'0';
  else if ((byte[0]>='A')&&(byte[0]<='F'))
    b=10+(byte[0]-'A');
  else if ((byte[0]>='a')&&(byte[0]<='f'))
    b=10+(byte[0]-'a');
  b=b<<4;

  if ((byte[1]>='0')&&(byte[1]<='9'))
    b+=byte[1]-'0';
  else if ((byte[1]>='A')&&(byte[1]<='F'))
    b+=10+(byte[1]-'A');
  else if ((byte[1]>='a')&&(byte[1]<='f'))
    b+=10+(byte[1]-'a');

  return b;
}

inline void parseColor(const char *str, unsigned len, Gray8& color) {
  color = Gray8(atoi(str));
}

inline void parseColor(const char *str, unsigned len, Rgb8& color) {
  if (str[0] == '#' && len == 7)
    color = Rgb8(parseHex(str+1),parseHex(str+3),parseHex(str+5));
  else if (len == 6)
    color = Rgb8(parseHex(str),parseHex(str+2),parseHex(str+4));
  else
    color = Rgb8(0);
}

inline void parseColor(const char *str, unsigned len, Rgba8& color) {
  if (str[0] == '#' && len == 9)
    color = Rgba8(parseHex(str+1),parseHex(str+3),parseHex(str+5),parseHex(str+7));
  else if (str[0] == '#' && len == 7)
    color = Rgba8(parseHex(str+1),parseHex(str+3),parseHex(str+5),255);
  else if (len == 8)
    color = Rgba8(parseHex(str),parseHex(str+2),parseHex(str+4),parseHex(str+6));
  else if (len == 6)
    color = Rgba8(parseHex(str),parseHex(str+2),parseHex(str+4),255);
  else
    color = Rgba8(0);
}

template <typename T> class ColorMap {
 public:
  std::map<std::string,T> map;
  ColorMap() {}
  ColorMap(std::string str) { parse(str); }

  void parse(const char *str) {
    T color;
    std::string key;
    const char *cp,*sp;
    cp=str;
    while (*cp && isspace(*cp)) cp++;
    while (*cp) {
      sp = cp;
      while (*cp && !isspace(*cp) && *cp != '=') cp++;
      key = std::string(sp, cp-sp);
      if (*cp == 0) break;
      while (*cp && (isspace(*cp)||*cp=='=')) cp++;
      if (*cp == 0) break;
      sp = cp;
      while (*cp && !isspace(*cp) && *cp != ',') cp++;
      parseColor(sp, cp-sp, color);
      map[key] = color;
      if (*cp == 0) break;
      while (*cp && (isspace(*cp)||*cp==',')) cp++;
    }
  }

  void parse(const std::string s) {
    parse(s.c_str());
  }

  bool get(std::string key, T& value) {
    auto entry = map.find(key);
    if (entry != map.end()) {
      value = entry->second;
      return true;
    }
    return false;
  }
};

class Margin {
 public:
  short top, left;
  short bottom, right;

  Margin() : top(0), right(0), bottom(0), left(0) {}
  Margin(short t, short r, short b, short l) : top(t),right(r),bottom(b),left(l) {}
  void set(short t, short r, short b, short l) {
    top = t; left = l; bottom = b; right = r;
  }
  void add(short x, short y) {
    if (x < left) left = x;
    if (x > right) right = x;
    if (y < top) top = y;
    else if (y > bottom) bottom = y;
  }
};

class Size {
 public:
  Coord rows, cols;

  Size() : rows(0), cols(0) {}
  Size(Coord rows, Coord cols) : rows(rows), cols(cols) {}
};

class Box {
 public:
  Coord top, left;
  Coord bottom, right;

  Box() : top(0),right(0),bottom(0),left(0) {}
  Box(Coord l, Coord t, Coord r, Coord b) : top(t),right(r),bottom(b),left(l) {}

  void set(Coord l, Coord t, Coord r, Coord b) {
    top = t; left = l; bottom = b; right = r;
  }
  void add(Coord x, Coord y) {
    if (x < left) left = x;
    if (x > right) right = x;
    if (y < top) top = y;
    if (y > bottom) bottom = y;
  }

  Coord Width() const { return right-left+1; }
  Coord Height() const { return bottom-top+1; }

  bool empty() const { return right < left || bottom < top; }
  bool contains(Coord x, Coord y) const {
    if (x < left) return false;
    if (x > right) return false;
    if (y < top) return false;
    if (y > bottom) return false;
    return true;
  }

  uint32_t area() const { return (uint32_t)Width()*(uint32_t)Height(); }

  template <class T> Box operator*(const T k) const {
    return Box((Coord)(k*left),(Coord)(k*top),
	       (Coord)(k*(right+1)-1),(Coord)(k*(bottom+1)-1));
  }
  bool operator ==(const Box& o) const {
    return top==o.top && left==o.left && bottom==o.bottom && right==o.right;
  }
  bool operator !=(const Box& o) const {
    return top!=o.top || left!=o.left || bottom!=o.bottom || right!=o.right;
  }
  bool operator <(const Box& o) const {
    return area()<o.area();
  }
  bool operator <=(const Box& o) const {
    return area()<=o.area();
  }
  bool operator >(const Box& o) const {
    return area()>o.area();
  }
  bool operator >=(const Box& o) const {
    return area()>=o.area();
  }
};

inline Box box_intersect(const Box& box1, const Box& box2) {
  return Box(SXMAX(box1.left,box2.left),
	     SXMAX(box1.top,box2.top),
	     SXMIN(box1.right,box2.right),
	     SXMIN(box1.bottom,box2.bottom));

}

inline Box affine(const Box& box, const Mat<float,2,3>& mat) {
  float left = mat[0]*box.left+mat[1]*box.top+mat[2];
  float top = mat[3]*box.left+mat[4]*box.top+mat[5];
  float right = mat[0]*box.right+mat[1]*box.bottom+mat[2];
  float bottom = mat[3]*box.right+mat[4]*box.bottom+mat[5];
  return Box(SXMAX(left,0.0f),
	     SXMAX(top,0.0f),
	     SXMAX(right,0.0f),
	     SXMAX(bottom,0.0f));
}

inline Box boxOf(const Box& box) { return box; }

// -1/0/1 = left/center/right
// -1/0/1 = top/center/bottom
inline void anchorOf(const Box& box, int halign, int valign, double& x, double& y) {
  x = box.left+((1+halign)/2.0)*(box.right-box.left);
  y = box.top+((1+valign)/2.0)*(box.bottom-box.top);
}


class LabelData {
 public:
  uint32_t label;
  uint32_t count;
  Box bbox;

  LabelData() : label(0), count(0) { }
  LabelData(uint32_t lbl, uint32_t area, const Box& box) : label(lbl), count(area), bbox(box) { }
};

// support for std::sort
inline bool operator<(const LabelData& x, const LabelData& y) {
  return x.bbox.area() < y.bbox.area();
}

inline Box boxOf(const LabelData& ld) { return ld.bbox; }


template <typename T> class UpSize {
 public:
};

template <> class UpSize<int8_t> {
 public:
  typedef int16_t type;
  typedef int16_t stype;
  typedef int16_t scalar_type;
  typedef int16_t scalar_stype;
};

template <> class UpSize<uint8_t> {
 public:
  typedef uint16_t type;
  typedef int16_t stype;
  typedef uint16_t scalar_type;
  typedef int16_t scalar_stype;
};

template <> class UpSize<uint16_t> {
 public:
  typedef uint32_t type;
  typedef int32_t stype;
  typedef uint32_t scalar_type;
  typedef int32_t scalar_stype;
};

template <> class UpSize<int16_t> {
 public:
  typedef int32_t type;
  typedef int32_t stype;
  typedef int32_t scalar_type;
  typedef int32_t scalar_stype;
};

template <> class UpSize<Rgb8> {
 public:
  typedef Rgb16 type;
  typedef Rgb16i stype;
  typedef uint16_t scalar_type;
  typedef int16_t scalar_stype;
};

template <> class UpSize<Rgb16> {
 public:
  typedef Rgb32 type;
  typedef Rgb32i stype;
  typedef uint32_t scalar_type;
  typedef int32_t scalar_stype;
};

template <typename T> class ArrayInfo {
 public:
  typedef T elmtype;
  enum { size = 0 };
};

template <typename T, int N> class ArrayInfo<Vec<T,N>> {
 public:
  typedef T elmtype;
  enum { size = N };
};

template <> class ArrayInfo<Rgb8> {
 public:
  typedef uint8_t elmtype;
  enum { size = 3 };
};

template <> class ArrayInfo<Hsv8> {
 public:
  typedef uint8_t elmtype;
  enum { size = 3 };
};

template <typename T> class TypeInfo {
 public:
  enum { max_value = std::numeric_limits<T>::max() };
};

template <typename T, int N> class TypeInfo<Vec<T,N>> {
 public:
  enum { unsigned_bias = TypeInfo<T>::unsigned_bias };
  enum { max_value = std::numeric_limits<T>::max() };
  enum { compliment = TypeInfo<T>::compliment };
};

template <> class TypeInfo<Rgb8> {
 public:
  enum { unsigned_bias = 128 };
  enum { max_value = 255 };
  enum { compliment = 255 };
};

template <> class TypeInfo<Hsv8> {
 public:
  enum { unsigned_bias = 128 };
  enum { max_value = 255 };
  enum { compliment = 255 };
};

template <> class TypeInfo<unsigned char> {
 public:
  enum { unsigned_bias = 128 };
  enum { max_value = 255 };
  enum { compliment = 255 };
};

template <> class TypeInfo<signed char> {
 public:
  enum { unsigned_bias = 0 };
  enum { max_value = 127 };
  enum { compliment = -1 };
};

template <> class TypeInfo<unsigned short> {
 public:
  enum { unsigned_bias = 32768 };
  enum { max_value = 65535 };
  enum { compliment = 65535 };
};

template <> class TypeInfo<short> {
 public:
  enum { unsigned_bias = 0 };
  enum { max_value = 32767 };
  enum { compliment = -1 };
};


#define FLOAT2UINT8(x) (uint8_t)((x)*256)

template <typename T> class CvMatImport {
 public:
  void grayRow(T *dst, uint8_t *ptr, int len, int sz) {}
  void bgrRow(T *dst, uint8_t *ptr, int len, int sz) {}
  void bgraRow(T *dst, uint8_t *ptr, int len, int sz) {}
  void floatRow(T *dst, uint8_t *ptr, int len, int chan, int sz) {}
};

template <> class CvMatImport<Gray8> {
 public:
  void grayRow(Gray8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      *dst = *ptr;
  }
  void bgrRow(Gray8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz) {
      // 0.299r + 0.587g + 0.114b
      uint32_t fx16;
      fx16 = (uint32_t)ptr[2] * 0x4c8b43u;
      fx16 += (uint32_t)ptr[1] * 0x9645a1u;
      fx16 += (uint32_t)ptr[0] * 0x1d2f1au;
      *dst = fx16>>24;
    }
  }
  void bgraRow(Gray8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz) {
      // 0.299r + 0.587g + 0.114b
      uint32_t fx16;
      fx16 = (uint32_t)ptr[2] * 0x4c8b43u;
      fx16 += (uint32_t)ptr[1] * 0x9645a1u;
      fx16 += (uint32_t)ptr[0] * 0x1d2f1au;
      *dst = fx16>>24;
    }
  }
  void floatRow(Gray8 *dst, uint8_t *ptr, int len, int chan, int sz) {
    if (chan >= 1) {
      for (int i=0; i<len; i++, dst++, ptr+=sz)
	*dst = FLOAT2UINT8(*(float *)ptr);
    }
  }
};

template <> class CvMatImport<Rgb8> {
 public:
  void grayRow(Rgb8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(*ptr, *ptr, *ptr);
  }
  void bgrRow(Rgb8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(ptr[2], ptr[1], ptr[0]);
  }
  void bgraRow(Rgb8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(ptr[2], ptr[1], ptr[0]);
  }
  void floatRow(Rgb8 *dst, uint8_t *ptr, int len, int chan, int sz) {
    if (chan >= 3) {
      for (int i=0; i<len; i++, dst++, ptr+=sz)
	dst->set(FLOAT2UINT8(((float *)ptr)[0]),
		 FLOAT2UINT8(((float *)ptr)[1]),
		 FLOAT2UINT8(((float *)ptr)[2]));
    }
  }
};

template <> class CvMatImport<Rgba8> {
 public:
  void grayRow(Rgba8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(*ptr, *ptr, *ptr, 255);
  }
  void bgrRow(Rgba8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(ptr[2], ptr[1], ptr[0], 255);
  }
  void bgraRow(Rgba8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(ptr[2], ptr[1], ptr[0], ptr[3]);
  }
  void floatRow(Rgba8 *dst, uint8_t *ptr, int len, int chan, int sz) {
    if (chan >= 4) {
      for (int i=0; i<len; i++, dst++, ptr+=sz)
	dst->set(FLOAT2UINT8(((float *)ptr)[0]),
		 FLOAT2UINT8(((float *)ptr)[1]),
		 FLOAT2UINT8(((float *)ptr)[2]),
		 FLOAT2UINT8(((float *)ptr)[3]));
    }
  }
};

template <> class CvMatImport<Hsv8> {
 public:
  void grayRow(Hsv8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(0, 0, *ptr);
  }
  void bgrRow(Hsv8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(ptr[0], ptr[1], ptr[2]);
  }
  void bgraRow(Hsv8 *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->set(ptr[0], ptr[1], ptr[2]);
  }
  void floatRow(Hsv8 *dst, uint8_t *ptr, int len, int chan, int sz) {
    if (chan >= 3) {
      for (int i=0; i<len; i++, dst++, ptr+=sz)
	dst->set(FLOAT2UINT8(((float *)ptr)[0]),
		 FLOAT2UINT8(((float *)ptr)[1]),
		 FLOAT2UINT8(((float *)ptr)[2]));
    }
  }
};

template <typename S, int N> class CvMatImport<Vec<S,N>> {
 public:
  void grayRow(Vec<S,N> *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz)
      dst->clear(*ptr);
  }
  void bgrRow(Vec<S,N> *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz) {
      (*dst)[0] = ptr[2];
      (*dst)[1] = ptr[1];
      (*dst)[2] = ptr[0];
    }
  }
  void bgraRow(Vec<S,N> *dst, uint8_t *ptr, int len, int sz) {
    for (int i=0; i<len; i++, dst++, ptr+=sz) {
      (*dst)[0] = ptr[2];
      (*dst)[1] = ptr[1];
      (*dst)[2] = ptr[0];
      (*dst)[3] = ptr[3];
    }
  }
  void floatRow(Vec<S,N> *dst, uint8_t *ptr, int len, int chan, int sz) {
    if (chan >= N) {
      for (int i=0; i<len; i++, dst++, ptr+=sz)
	for (int j = 0; j < N; j++)
	  (*dst)[j] = (S)((float *)ptr)[j];
    }
  }
};

inline void putPixel(Gray8 src, uint8_t *ptr) {
  *ptr = src;
}

inline void putPixel(Gray16 src, uint8_t *ptr) {
  *(uint16_t *)ptr = src;
}

inline void putPixel(Rgb8& src, uint8_t *ptr) {
  ptr[0] = src.B();
  ptr[1] = src.G();
  ptr[2] = src.R();
}

inline void putPixel(Rgba8& src, uint8_t *ptr) {
  ptr[0] = src.B();
  ptr[1] = src.G();
  ptr[2] = src.R();
  ptr[3] = src.A();
}

inline void putPixel(Hsv8& src, uint8_t *ptr) {
  ptr[0] = src.H();
  ptr[1] = src.S();
  ptr[2] = src.V();
}

inline void writePixel(const Gray8 src, FILE *outf) {
  fprintf(outf, "%02X", src);
}

inline void writePixel(const Gray16 src, FILE *outf) {
  fprintf(outf, "%04X", src);
}

inline void writePixel(const Rgb8& src, FILE *outf) {
  fprintf(outf, "%02X%02X%02X", src.R(), src.G(), src.B());
}

inline void writePixel(const Rgb16& src, FILE *outf) {
  fprintf(outf, "%04X%04X%04X", src.R(), src.G(), src.B());
}

inline void writePixel(const Rgba8& src, FILE *outf) {
  fprintf(outf, "%02X%02X%02X%02X", src.R(), src.G(), src.B(), src.A());
}

inline void writePixel(const Hsv8& src, FILE *outf) {
  fprintf(outf, "%02X%02X%02X", src.H(), src.S(), src.V());
}

inline void writeAs(Rgba8 *ptr, const Rgb8& val) {
#ifndef SL_SYNTH
  ptr->rgba = 0xff000000u | val.rgb;
#else
  ptr->set(val.R(), val.G(), val.B(), 255u);
#endif
}

template <typename T> T abs(T x) {
  return x>=0 ? x : -x;
}

template <typename T> RgbV<T> abs(const RgbV<T> x) {
  return RgbV<T>(abs(x.R()), abs(x.G()), abs(x.B()));
}

inline Rgb8 scaleFx(const Rgb8 x, const uint8_t k) {
  uint32_t r, g, b;
  r = (258*(uint32_t)x.R()*(uint32_t)k)>>16;
  g = (258*(uint32_t)x.G()*(uint32_t)k)>>16;
  b = (258*(uint32_t)x.B()*(uint32_t)k)>>16;
  return Rgb8(r,g,b);
}

inline Rgba8 scaleFx(const Rgba8 x, const uint8_t k) {
  uint32_t a;
  a = (258*(uint32_t)x.A()*(uint32_t)k)>>16;
  return Rgba8(x.R(),x.G(),x.B(),a);
}

inline Hsv8 scaleFx(const Hsv8 x, const uint8_t k) {
  uint32_t v;
  v = (258*(uint32_t)x.V()*(uint32_t)k)>>16;
  return Hsv8(x.H(),x.S(),v);
}

inline Rgb8 blendFx(const Rgb8 x, const Rgb8 y, const uint8_t k) {
  uint32_t r, g, b;
  uint32_t xk = 255-k;
  uint32_t yk = k;
  r = (258*((uint32_t)x.R()*xk + (uint32_t)y.R()*yk))>>16;
  g = (258*((uint32_t)x.G()*xk + (uint32_t)y.G()*yk))>>16;
  b = (258*((uint32_t)x.B()*xk + (uint32_t)y.B()*yk))>>16;
  return Rgb8(r,g,b);
}

inline Rgb8 absdiff(const Rgb8 x, const Rgb8 y) {
  uint8_t r = (x.R() >= y.R()) ? x.R()-y.R() : y.R()-x.R();
  uint8_t g = (x.B() >= y.B()) ? x.B()-y.B() : y.B()-x.B();
  uint8_t b = (x.G() >= y.G()) ? x.G()-y.G() : y.G()-x.G();
  return Rgb8(r,g,b);
}

inline Gray absdiff(const Gray x, const Gray y) {
  return (x >= y) ? (x - y) : (y - x);
}

inline Rgb8 minpx(const Rgb8 x, const Rgb8 y) {
  if ((x.R()+x.B()+x.G())<=(y.R()+y.B()+y.G()))
    return x;
  else
    return y;
}

inline Gray minpx(const Gray x, const Gray y) {
  return (x <= y) ? x : y;
}

inline Rgb8 maxpx(const Rgb8 x, const Rgb8 y) {
  if ((x.R()+x.B()+x.G())<=(y.R()+y.B()+y.G()))
    return x;
  else
    return y;
}

inline Gray maxpx(const Gray x, const Gray y) {
  return (x >= y) ? x : y;
}

inline Rgb8 compose(const Rgb8 x, const Rgba8 y) {
  uint32_t r, g, b;
  uint8_t k = y.A();
  uint32_t xk = 255-k;
  uint32_t yk = k;
  r = (258*((uint32_t)x.R()*xk + (uint32_t)y.R()*yk))>>16;
  g = (258*((uint32_t)x.G()*xk + (uint32_t)y.G()*yk))>>16;
  b = (258*((uint32_t)x.B()*xk + (uint32_t)y.B()*yk))>>16;
  return Rgb8(r,g,b);
}

inline void compose(Rgba8 *ptr, const Rgba8& val) {
#ifndef SL_SYNTH
  uint8_t u,v;
  uint8_t aA = val.rgba>>24;
  uint8_t aB = ptr->rgba>>24;
  aB = ((uint16_t)aB * (255-aA))/255;
  uint32_t rgba = aA + aB;
  u = val.rgba>>16;
  v = ptr->rgba>>16;
  rgba = (rgba << 8) + (uint8_t)(((uint16_t)u*aA + (uint16_t)v*aB)/255);
  u = val.rgba>>8;
  v = ptr->rgba>>8;
  rgba = (rgba << 8) + (uint8_t)(((uint16_t)u*aA + (uint16_t)v*aB)/255);
  u = val.rgba;
  v = ptr->rgba;
  rgba = (rgba << 8) + (uint8_t)(((uint16_t)u*aA + (uint16_t)v*aB)/255);
  ptr->rgba = rgba;
#else
  uint8_t aA = val.A();
  uint8_t aB = ptr->A();
  aB = ((uint16_t)aB * (255-aA))/255;
  ptr->set(((uint16_t)val.R()*aA + (uint16_t)ptr->B()*aB)/255,
	   ((uint16_t)val.G()*aA + (uint16_t)ptr->G()*aB)/255,
	   ((uint16_t)val.B()*aA + (uint16_t)ptr->B()*aB)/255,
	   aA + aB);
#endif
}

class AnySxParam {
 public:
  virtual ~AnySxParam() { }
};

class StartSxParam : public AnySxParam {
 public:
  int value;
  StartSxParam(int start) : value(start) {}
};

inline StartSxParam *Start(int start) { return new StartSxParam(start); }

class StopSxParam : public AnySxParam {
 public:
  int value;
  StopSxParam(int stop) : value(stop) {}
};

inline StopSxParam *Stop(int stop) { return new StopSxParam(stop); }

template <typename T> class Buffer {
 private:
  bool external;
  size_t allocated, filled;
  unsigned refcnt;
 public:
  T *data;

  Buffer(size_t size) {
    external = false;
    allocated = filled = size;
    data = new T[allocated];
    refcnt = 1;
  }
  Buffer(T *data, size_t size) {
    external = true;
    allocated = filled = size;
    this->data = data;
    refcnt = 1;
  }
  ~Buffer() {
    if (!external)
      delete[] data;
    //fflush(stdout);
    //printf("delete\n");
  }

  size_t getAllocatedBytes() { return allocated*sizeof(T); }

  void copyTo(Buffer<T> *other) {
    other->resize(filled);
    memcpy(other->data, data, filled*sizeof(T));
  }

  void incref() {
    refcnt++;
  }
  void decref() {
    refcnt--;
    if (refcnt == 0)
      delete this;
  }

  void resize(size_t size) {
    if (size > allocated) {
      allocated = filled = size;
      delete[] data;
      data = new T[allocated];
    } else
      filled = size;
  }

  void set(T value) {
    for (int i = 0; i < filled; i++)
      data[i] = value;
  }

  T minimal() const {
    T res = std::numeric_limits<T>::max();
    for (uint16_t idx = 0; idx < filled; idx++) {
      T val = data[idx];
      if (val < res)
	res = val;
    }
    return res;
  }

  T maximal() const {
    T res = std::numeric_limits<T>::min();
    for (uint16_t idx = 0; idx < filled; idx++) {
      T val = data[idx];
      if (val > res)
	res = val;
    }
    return res;
  }


  T sum() const {
    T res = (T)0;
    for (uint16_t idx = 0; idx < filled; idx++)
      res += data[idx];
    return res;
  }
};

template <typename T> class BufferView {
 protected:
  Buffer<T> *buffer;
  int height, width; // height,width of underlying image
  int anchorx, anchory; // top-left of origin of view (may be negative)
  unsigned refcnt;

  virtual T *map(int y, int x) const = 0;

 public:
  BufferView() {
    buffer = NULL;
    anchorx = anchory = 0;
    refcnt = 1;
  }
  BufferView(Buffer<T> *buffer, int rows, int cols)
   : buffer(buffer), height(rows), width(cols) {
    buffer->incref();
    anchorx = anchory = 0;
    refcnt = 1;
  }
  BufferView(int rows, int cols)
    : height(rows), width(cols) {
    buffer = new Buffer<T>(height * width);
    anchorx = anchory = 0;
    refcnt = 1;
  }
  BufferView(const BufferView<T>& other) {
    height = other.height;
    width = other.width;
    anchorx = other.anchorx;
    anchory = other.anchory;
    buffer = other.buffer;
    buffer->incref();
    refcnt = 1;
    puts("copy");
  }
  virtual ~BufferView() { if (buffer != NULL) buffer->decref(); }

  void incref() {
    refcnt++;
  }
  void decref() {
    refcnt--;
    if (refcnt == 0)
      delete this;
  }

  Buffer<T> *getBuffer() { return buffer; }

  void setBuffer(Buffer<T> *buffer2, int rows, int cols) {
    if (buffer != NULL) buffer->decref();
    buffer = buffer2;
    buffer2->incref();
    height = rows;
    width = cols;
  }

  void resize(int rows, int cols) {
    height = rows;
    width = cols;
    buffer->resize(height * width);
  }

  int Height() { return height; }
  int Width() { return width; }

  // anchor methods
  int getX() const { return anchorx; }
  int getY() const { return anchory; }
  void setX(int x) { anchorx = x; }
  void setY(int y) { anchory = y; }
  void incrX(int dx=1) {
    anchorx += dx;
    if (anchorx >= width)
      anchorx -= width;
  }
  void incrY(int dy=1) {
    anchory += dy;
    if (anchory >= height)
      anchory -= height;
  }

  // access methods
  T& operator()(int y, int x)  {
    return *map(y,x);
  }
  const T& operator()(int y, int x) const {
    return *map(y,x);
  }
  void set(int y, int x, T val) {
    *map(y,x) = val;
  }
};

template <typename T> class WrapView : public BufferView<T> {
 protected:
  T *map(int y, int x) const {
    x += this->anchorx;
    if (x < 0) x += this->width;
    else if (x >= this->width) x -= this->width;
    y += this->anchory;
    if (y < 0) y += this->height;
    else if (y >= this->height) y -= this->height;
    return this->buffer->data + (y * this->width + x);
  }

 public:
};

template <typename T> class WinWrapView : public BufferView<T> {
 public:
  int dx,dy; // x/y wrap distance
 protected:
  T *map(int y, int x) const {
    x += this->anchorx;
    if (x < 0) x += dx;
    else if (x >= this->width) x -= dx;
    y += this->anchory;
    if (y < 0) y += dy;
    else if (y >= this->height) y -= dy;
    return this->buffer->data + (y * this->width + x);
  }

 public:
   WinWrapView(int dy, int dx) : dy(dy), dx(dx) {}
};

template <typename T> class PadView : public BufferView<T> {
 private:
  T pad;
  T *padPtr;
 protected:
  T *map(int y, int x) const {
    x += this->anchorx;
    if (x < 0) return padPtr;
    else if (x >= this->width) return padPtr;
    y += this->anchory;
    if (y < 0) return padPtr;
    else if (y >= this->height) return padPtr;
    return this->buffer->data + (y * this->width + x);
  }

 public:
  PadView(T value) : pad(value) { padPtr = &pad; }
};

template <typename T> class RepeatView : public BufferView<T> {
 protected:
  T *map(int y, int x) const {
    x += this->anchorx;
    if (x < 0) x = 0;
    else if (x >= this->width) x = this->width-1;
    y += this->anchory;
    if (y < 0) y = 0;
    else if (y >= this->height) y = this->height-1;
    return this->buffer->data + (y * this->width + x);
  }

 public:
};

template <typename T, int N, int M> class SubMat {
 private:
  BufferView<T> *view;
 public:
 SubMat(BufferView<T> *view) : view(view) { view->incref(); }
  SubMat(const SubMat<T,N,M>& other) {
   view = other.view;
   view->incref();
 }
 ~SubMat() { view->decref(); }

 int getX() const { return view->getX(); }
 int getY() const { return view->getY(); }

  T& operator()(int y, int x)  {
    return (*view)(y,x);
  }
  const T& operator()(int y, int x) const {
    return (*view)(y,x);
  }

  T minimal() const {
    T res = std::numeric_limits<T>::max();
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++) {
	T val = (*view)(y,x);
	if (val < res)
	  res = val;
      }
    }
    return res;
  }

  T maximal() const {
    T res = std::numeric_limits<T>::min();
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++) {
	T val = (*view)(y,x);
	if (val > res)
	  res = val;
      }
    }
    return res;
  }

  T min() const {
    T res = std::numeric_limits<typename ArrayInfo<T>::elmtype>::max();
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++)
	vecmin(res, (*view)(y,x));
    }
    return res;
  }

  T max() const {
    T res = std::numeric_limits<typename ArrayInfo<T>::elmtype>::min();
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++)
	res = vecmax(res, (*view)(y,x));
    }
    return res;
  }

  T sum() const {
    T res = (T)0;
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++) {
	T val = (*view)(y,x);
	res += val;
      }
    }
    return res;
  }

  T average() const {
    typename UpSize<T>::type res(0,0,0);
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++) {
	T val = (*view)(y,x);
	res += val;
      }
    }
    return T(res.R()/(N*M), res.G()/(N*M), res.B()/(N*M));
  }

  void dump() {
    for (int y = 0; y < N; y++)
      for (int x = 0; x < M; x++)
	printf(" %d", (*view)(y,x));
    printf("\n");
  }
};

template <typename T, typename U, typename V, int N>
void product(const Vec<T,N>& v, const Vec<U,N>& k, Vec<V,N>& p) {
  for (int i = 0; i < N; i++)
    p[i] = (V)v[i] * (V)k[i];
}

template <typename T, typename U, typename V, int N, int M>
  void product(const SubMat<T,N,M>& mat, const U& k, Mat<V,N,M>& p) {
  for (int y = 0; y < N; y++)
    for (int x = 0; x < M; x++)
      p(y,x) = (V)mat(y,x) * k;
}

template <typename T, typename U, typename V, int N, int M>
void product(const SubMat<T,N,M>& mat, const Mat<U,N,M>& k, Mat<V,N,M>& p) {
  for (int y = 0; y < N; y++)
    for (int x = 0; x < M; x++)
      p(y,x) = (V)mat(y,x) * (V)k(y,x);
#if 0
  if (mat.getX()==19 && mat.getY()==8) {
    V acc=0;
    for (int y = 0; y < N; y++) {
      for (int x = 0; x < M; x++)
	printf("%3d ", mat(y,x));
      printf("| ");
      for (int x = 0; x < M; x++)
	printf("%3d ", k(y,x));
      printf("| ");
      for (int x = 0; x < M; x++) {
	printf("%5d ", p(y,x));
	acc+=p(y,x);
      }
      printf("\n");
    }
    printf("=%d\n",acc);
  }
#endif
}

template <typename T, typename U, typename V, int N, int M, int O>
void product(const SubMat<Vec<T,O>,N,M> mat, const Vol<U,N,M,O>& k, Vol<V,N,M,O>& p) {
  for (int y = 0; y < N; y++)
    for (int x = 0; x < M; x++)
      for (int z = 0; z < O; z++)
	p(y,x,z) = (V)mat(y,x)[z] * (V)k(y,x,z);
#if 0
  if (mat.getX()==3 && mat.getY()==2) {
    V acc=0,row;
    for (int z = 0; z < O; z++) {
      for (int y = 0; y < N; y++) {
	for (int x = 0; x < M; x++)
	  printf("%3d ", mat(y,x)[z]);
	printf("| ");
	for (int x = 0; x < M; x++)
	  printf("%3d ", k(y,x,z));
	printf("| ");
	row=0;
	for (int x = 0; x < M; x++) {
	  printf("%5d ", p(y,x,z));
	  row+=p(y,x,z);
	}
	printf("=%d\n",row);
	acc+=row;
      }
    }
    printf("=%d\n",acc);
  }
#endif
}

template <typename T> class Img {
 private:
  CvMatImport<T> cvImport;
  Coord height, width;
  MetaDataRef meta;
  Buffer<T> *buffer;
  uint32_t ptr, size;
 public:
  Img() {
    buffer = NULL;
  }
  Img(Coord rows, Coord cols, Buffer<T> *buf) {
    height = rows;
    width = cols;
    buffer = buf;
    buf->incref();
  }
  Img(const Img& other) {
    height = other.height;
    width = other.width;
    meta = other.meta;
    buffer = other.buffer;
    if (buffer != NULL) buffer->incref();
    size = other.size;
    reset();
  }
  ~Img() {
    if (buffer != NULL) buffer->decref();
  }

  Img& operator=(const Img& other) {
    height = other.height;
    width = other.width;
    meta = other.meta;
    buffer = other.buffer;
    if (buffer != NULL) buffer->incref();
    size = other.size;
    reset();
    return *this;
  }

  void copyTo(Img<T>& other) {
    if (buffer != NULL) {
      other.resize(height, width);
      buffer->copyTo(other.buffer);
    }
    other.meta = meta;
  }

  const MetaDataRef Attributes() const { return meta; }
  void setAttributes(const MetaDataRef other) { meta = other; }
  void clearAttributes() { meta.clear(); }

  Coord Height() const { return height; }
  Coord Width() const { return width; }

  Buffer<T> *getBuffer() { return buffer; }
  const Buffer<T> *getBuffer() const { return buffer; }

  BufferView<T> *PaddedView(T padding) const {
    auto view = new PadView<T>(padding);
    view->setBuffer(buffer, height, width);
    return view;
  }

  void resize(Coord rows, Coord cols) {
    height = rows;
    width = cols;
    size = rows*cols;
    if (buffer != NULL)
      buffer->resize(size);
    else
      buffer = new Buffer<T>(size);
    reset();
  }
  void resize(Size size) { resize(size.rows, size.cols); }

  void reset() {
    ptr = 0;
  }

  inline uint32_t Pos() { return ptr; }
  inline T *Ptr() { return buffer->data+ptr; }
  inline T *Ptr(uint32_t idx) { return buffer->data+idx; }
  inline T *RowPtr(Coord row) { return buffer->data+row*width; }
  inline const T *Ptr(uint32_t idx) const { return buffer->data+idx; }
  inline const T *RowPtr(Coord row) const { return buffer->data+row*width; }

  inline T& next() {
    T *point = buffer->data+ptr;
    ptr++;
    if (ptr == size) ptr = 0;
    return *point;
  }

  void clear(T val) {
    ptr = 0;
    while (ptr < size) {
      *(buffer->data+ptr) = val;
      ptr++;
    }
    ptr = 0;
  }
  void clear() { clear((T) 0); }

  void fillrect(const Box& box, T val) {
    Coord boxwidth = box.right-box.left+1;
    for (Coord y = box.top; y <= box.bottom; y++) {
      ptr = y * width + box.left;
      for (Coord x = 0; x < boxwidth; x++) {
	*(buffer->data+ptr) = val;
	ptr++;
      }
    }
  }

  void drawrect(const Box& box, T val) {
    Coord boxwidth = box.right-box.left+1;
    for (Coord y = box.top; y <= box.bottom; y++) {
      ptr = y * width + box.left;
      if (y == box.top || y == box.bottom) {
	for (Coord x = 0; x < boxwidth; x++) {
	  *(buffer->data+ptr) = val;
	  ptr++;
	}
      } else {
	*(buffer->data+ptr) = val;
	ptr += box.right - box.left;
	*(buffer->data+ptr) = val;
      }
    }
  }

  void serialize(const char *filename, bool exitOnError=true) {
    FILE *outf = fopen(filename, "wb");
    if (outf != NULL) {
      reset();
      for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
	  writePixel(next(), outf);
	  fprintf(outf,"\n");
	}
      }
      fclose(outf);
    } else {
      fprintf(stderr, "Unable to open output %s\n", filename);
      if (exitOnError)
	exit(1);
    }
    reset();
  }
};

template <typename T> class Blob {
 private:
  MetaDataRef meta;
  Buffer<T> *buffer;
  uint32_t ptr, size;
 public:
  Blob() {
    buffer = NULL;
  }
  Blob(const Blob& other) {
    meta = other.meta;
    buffer = other.buffer;
    if (buffer != NULL) buffer->incref();
    size = other.size;
    reset();
  }
  ~Blob() {
    if (buffer != NULL) buffer->decref();
  }

  Blob& operator=(const Blob& other) {
    meta = other.meta;
    buffer = other.buffer;
    if (buffer != NULL) buffer->incref();
    size = other.size;
    reset();
    return *this;
  }

  void copyTo(Blob<T>& other) {
    if (buffer != NULL) {
      other.resize(size);
      buffer->copyTo(other.buffer);
    }
    other.meta = meta;
  }

  const MetaDataRef Attributes() const { return meta; }
  void setAttributes(const MetaDataRef other) { meta = other; }
  void clearAttributes() { meta.clear(); }

  uint32_t Size() const { return size; }

  Buffer<T> *getBuffer() { return buffer; }

  void resize(uint32_t size) {
    if (buffer != NULL)
      buffer->resize(size);
    else
      buffer = new Buffer<T>(size);
    this->size = size;
    reset();
  }

  void reset() {
    ptr = 0;
  }

  inline uint32_t Pos() { return ptr; }
  inline T *Ptr() { return buffer->data+ptr; }
  inline T *Ptr(uint32_t idx) { return buffer->data+idx; }
  inline const T *Ptr(uint32_t idx) const { return buffer->data+idx; }

  inline T& next() {
    T *point = buffer->data+ptr;
    ptr++;
    if (ptr == size) ptr = 0;
    return *point;
  }

  void clear(T val) {
    ptr = 0;
    while (ptr < size) {
      *(buffer->data+ptr) = val;
      ptr++;
    }
    ptr = 0;
  }
  void clear() { clear((T) 0); }

  void set(T *data, uint32_t bytes) {
    resize(bytes);
    memcpy(buffer->data, data, bytes);
  }
};

template <typename T> struct is_img {
  static constexpr bool value = false;
};

template <typename T> struct is_img<Img<T>> {
  static constexpr bool value = true;
};

#endif
