#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>

namespace tracer {
  class image;

  class image_column {
  public:
    // constructors and destructors
    image_column(int _col, int _height, color *_data);

    // operators
    color& operator [] (int i);

    // data members
    int col, height;
    color *data;
  };

  class image {
  public:
    // constructors and destructors
    image(int width, int height);

    // methods
    color& getpixel(int x, int y) const;
    setpixel(int x, int y, color c);
    image resize(int width, int height) const;
    save(std::string filename);
    saveTiff(std::string filename);

    // operators
    image_column& operator [] (int i);

  protected:
    // data members
    color *data;
  };
}

#endif
