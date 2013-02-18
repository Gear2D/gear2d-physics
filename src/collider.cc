#include <iostream>
#include "gear2d.h"

#include "vector3.h"

using namespace gear2d;
using namespace std;

class collider : public component::base {
  private:
    
    vector3 v;
    
  public:
    // constructor and destructor
    collider() { }
    virtual ~collider() { }
    
    void writepos(const vector3 & v) {
      write<float>("x", v.x);
      write<float>("y", v.y);
      write<float>("z", v.z);
    }
    
    void readpos(vector3 & v) {
      read<float>("x", v.x);
      read<float>("y", v.y);
      read<float>("z", v.z);
    }
    
    
    // greeter family
    virtual gear2d::component::family family() { return "collider"; }
    
    // helloperson type
    virtual gear2d::component::type type() { return "collider"; }
    
    virtual std::string depends() { return "spatial"; }
    
    // setup phase, to initialize paramters and other stuff
    virtual void setup(object::signature & sig) {
      v.x = v.y = v.z = 0;
      write("speed", v);
      
      float h = v.x + v.y;
    }           
    
    
    virtual void update(timediff dt) {
    }
};

// the build function
g2dcomponent(collider)
