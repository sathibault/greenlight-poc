#ifndef IMG_DLIB_H
#define IMG_DLIB_H

////////// Img<Rgb8> dlib image interface

#ifdef _MSC_VER
__pragma(pack(push, 1)) typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char pad;
} sxrgb_pixel; __pragma(pack(pop))
#elif defined(__GNUC__)
typedef  struct __attribute__((packed)) {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char pad;
} sxrgb_pixel;
#endif


namespace dlib {
  template <>
  struct image_traits<Img<Rgb8>> {
    typedef sxrgb_pixel pixel_type;
  };

  template <>
  struct pixel_traits<sxrgb_pixel> {
        constexpr static bool rgb  = true;
        constexpr static bool rgb_alpha  = false;
        constexpr static bool grayscale = false;
        constexpr static bool hsi = false;
        constexpr static bool lab = false;
        enum { num = 3};
        typedef unsigned char basic_pixel_type;
        static basic_pixel_type min() { return 0;}
        static basic_pixel_type max() { return 255;}
        constexpr static bool is_unsigned = true;
        constexpr static bool has_alpha = false;
    };

  template <>
  struct image_traits<Img<Gray8>> {
    typedef unsigned char pixel_type;
  };
}

inline long num_rows(const Img<Rgb8>& img) { return img.Height(); }
inline long num_columns(const Img<Rgb8>& img) { return img.Width(); }
inline void *image_data(Img<Rgb8>& img) { return img.Ptr(0); }
inline const void *image_data(const Img<Rgb8>& img) { return img.Ptr(0); }
inline long width_step(const Img<Rgb8>& img) { return img.Width()*sizeof(Rgb8); }
inline void set_image_size(Img<Rgb>& img, long rows, long cols) { img.resize(rows, cols); }

inline long num_rows(const Img<Gray8>& img) { return img.Height(); }
inline long num_columns(const Img<Gray8>& img) { return img.Width(); }
inline void *image_data(Img<Gray8>& img) { return img.Ptr(0); }
inline const void *image_data(const Img<Gray8>& img) { return img.Ptr(0); }
inline long width_step(const Img<Gray8>& img) { return img.Width(); }
inline void set_image_size(Img<Gray8>& img, long rows, long cols) { img.resize(rows, cols); }

#endif
