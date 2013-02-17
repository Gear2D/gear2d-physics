#ifndef VECTOR3_H
#define VECTOR3_H

#include "gear2d.h"
#include <cmath>

using namespace gear2d;
using std::sqrt;

class vector3 {
  private:
    float x;
    float y;
    float z;
    float lenght;
    
  public:
    vector3(float x = 0, float y = 0, float x = 0)
    : x(x), y(y), z(z)
    {
      
      float a = 1.0f + lenght;
    }
    
    void write(component::base & com, const std::string & prefix) {
      com.write<float>(prefix + ".x", x);
      com.write<float>(prefix + ".y", y);
      com.write<float>(prefix + ".z", z);
    }
    
    void read(component::base & com, const std::string & prefix) {
      com.read<float>(prefix + ".x", x);
      com.read<float>(prefix + ".y", y);
      com.read<float>(prefix + ".z", z);
    }
    
    
    /* static methods */
    static vector3 from(component::base & com, const std::string & prefix) {
      vector3 v;
      v.read(com, prefix);
      return v;
    }
    
    void bolas() {
      
    }
    
};



#endif