#include <iostream>
#include "gear2d.h"

#include "vector3.h"
#include "shapes.h"

using namespace gear2d;
using namespace std;

class collider : public component::base {
  private:
    // container to hold a pair of collision check
    struct interaction {
      public:
        int update_timestamp;
        shape* shape1;
        shape* shape2;
        
        interaction(shape* shape1, shape* shape2)
        : update_timestamp(-1), shape1(shape1), shape2(shape2) {}
        
        // operator implemented for the set template class comparison
        bool operator<(const interaction& other) const {
          // an interaction is equal to another if both have the same shapes.
          // interactions between a shape and itself shouldn't be stored.
          if ((shape1 == other.shape1 && shape2 == other.shape2) ||
              (shape1 == other.shape2 && shape2 == other.shape1) ||
              (shape1 == shape2 || other.shape1 == other.shape2))
            return false;
          
          return true;
        }
    };
    
    static map< collider*, map<string, shape*> > shapes;
    static set<interaction> interactions;
    static int update_timestamp;
    static bool shapes_changed;
    
  public:
    // constructor and destructor
    collider() {}
    ~collider() {
      while (shapes[this].size()) {
        delete shapes[this].begin()->second;
        shapes[this].erase(shapes[this].begin());
      }
      shapes.erase(this);
    }
    
    virtual gear2d::component::family family() { return "collider"; }
    
    virtual gear2d::component::type type() { return "collider"; }
    
    virtual std::string depends() { return "spatial/space2d"; }
    
    // setup phase, to initialize paramters and other stuff
    virtual void setup(object::signature & sig) {
      set<string> shape_names;
      split(shape_names, sig["collider.shapes"], ' ');
      
      while (shape_names.size()) {
        loadshape(sig, *shape_names.begin());
        shape_names.erase(shape_names.begin());
      }
    }
    
    void loadshape(object::signature & sig, const string& shape_name) {
      string full_name = "collider." + shape_name + ".";
      string type = sig[full_name + "type"];
      
      if (type == "rectangle")
        shapes[this][shape_name] = new rectangle(this, sig, full_name);
      else if (type == "circle")
        shapes[this][shape_name] = new circle(this, sig, full_name);
      else
        throw evil("Unknown geometrical shape type creation inside collider component");
      
      shapes_changed = true;
    }
    
    virtual void update(timediff dt, int begin) {
      // avoiding collider component update more than once by frame
      if (update_timestamp == begin)
        return;
      update_timestamp = begin;
      
      // update while new collision interactions were created inside the global update loop
      shapes_changed = true;
      while (shapes_changed) {
        shapes_changed = false;
        globalupdate(dt, begin);
      }
    }
    
    // check all collision interactions
    void globalupdate(timediff dt, int begin) {
      for (set<interaction>::iterator it = interactions.begin(); it != interactions.end(); ++it) {
        interaction& tmp = (interaction&)*it;
        
        // avoiding collision interaction check more than once by frame
        if (tmp.update_timestamp == begin)
          continue;
        tmp.update_timestamp = begin;
        
        
      }
    }
};

// static vars
map< collider*, map<string, shape*> > collider::shapes;
set<collider::interaction> collider::interactions;
int collider::update_timestamp = -1;
bool collider::shapes_changed;

// the build function
g2dcomponent(collider)
