#include <iostream>
#include "gear2d.h"

#include "vector3.h"
#include "shapes.h"

using namespace gear2d;
using namespace std;

class collider : public component::base {
  private:
    list<shape*> shapes;
    
  public:
    // constructor and destructor
    collider() { }
    virtual ~collider() { }
    
    // greeter family
    virtual gear2d::component::family family() { return "collider"; }
    
    // helloperson type
    virtual gear2d::component::type type() { return "collider"; }
    
    virtual std::string depends() { return "spatial/space2d"; }
    
    // setup phase, to initialize paramters and other stuff
    virtual void setup(object::signature & sig) {
      set<string> shape_names;
      split(shape_names, sig["collider.shapes"], ' ');
      while (shape_names.size()) {
        const string& shape_name = *shape_names.begin();
        loadshape(sig, "collider." + shape_name + ".");
        shape_names.erase(shape_name);
      }
    }
    
    void loadshape(object::signature & sig, const string& shape_name) {
      string type = sig[shape_name + "type"];
      if (type == "rectangle")
        shapes.push_back(new rectangle(this, sig, shape_name));
      else if (type == "circle")
        shapes.push_back(new circle(this, sig, shape_name));
    }
    
    virtual void update(timediff dt) {
    }
};

// the build function
g2dcomponent(collider)
