#include <iostream>
#include "gear2d.h"

#include "vector3.h"
#include "shapes.h"

using namespace gear2d;
using namespace std;

class collider : public component::base {
  private:
    class interaction {
      private:
        int update_timestamp;
        
      public:
        collider* com1;
        string shape1;
        
        collider* com2;
        string shape2;
        
        interaction() : update_timestamp(-1) {}
        
        // operator implemented for the set template class comparison
        bool operator<(const interaction& other) const {
          // an interaction is equal to another if both have the same shapes
          if (((com1 == other.com1 && shape1 == other.shape1) &&
               (com2 == other.com2 && shape2 == other.shape2)) ||
              ((com1 == other.com2 && shape1 == other.shape2) &&
               (com2 == other.com1 && shape2 == other.shape1)))
            return false;
          
          // interactions between a shape and itself shouldn't be stored
          if ((com1       == com2       && shape1       == shape2      ) ||
              (other.com1 == other.com2 && other.shape1 == other.shape2))
            return false;
          
          return true;
        }
        
        // call shape methods to check collision between them
        void checkcollision(timediff dt, int frame_timestamp) {
          // avoiding collision interaction check more than once by frame
          if (update_timestamp == frame_timestamp)
            return;
          update_timestamp = frame_timestamp;
          
          
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
    
    virtual void update(timediff dt) {
      int frame_timestamp;//TODO retrive "begin" variable from run function of gear2d...
      
      // avoiding collider component update more than once by frame
      if (update_timestamp == frame_timestamp)
        return;
      update_timestamp = frame_timestamp;
      
      // update while new collision interactions were created inside the global update loop
      shapes_changed = true;
      while (shapes_changed) {
        shapes_changed = false;
        globalupdate(dt, frame_timestamp);
      }
    }
    
    // check all collision interactions
    void globalupdate(timediff dt, int frame_timestamp) {
      for (set<interaction>::iterator it = interactions.begin(); it != interactions.end(); ++it)
        ((interaction&)*it).checkcollision(dt, frame_timestamp);
    }
};

// static vars
map< collider*, map<string, shape*> > collider::shapes;
set<collider::interaction> collider::interactions;
int collider::update_timestamp = -1;
bool collider::shapes_changed;

// the build function
g2dcomponent(collider)
