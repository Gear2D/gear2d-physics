#include <iostream>
#include "gear2d.h"

#include "shapes.h"

using namespace gear2d;
using namespace std;

class collider : public component::base {
  private:
    // container to hold a pair of collision check
    struct interaction {
      public:
        static bool interactions_changed;
        
        int update_timestamp;
        shape* shape1;
        shape* shape2;
        
        interaction(shape* shape1, shape* shape2)
        : update_timestamp(-1), shape1(shape1), shape2(shape2)
        {
          interactions_changed = true;
        }
        
        // operator implemented for the set template class comparison
        bool operator<(const interaction& other) const {
          // an interaction is equal to another if both have the same shapes
          return (!((shape1 == other.shape1 && shape2 == other.shape2) ||
                    (shape1 == other.shape2 && shape2 == other.shape1)));
        }
        
        // checks collision between the pair of shapes
        void checkcollision(timediff dt) {
          bool collides = shape1->checkcollision(dt, shape2);
          
          // triggers the collision event
          
        }
    };
    
    static set<collider*> colliders;
    static set<interaction> interactions;
    static int update_timestamp;
    
    set<shape*> shapes;
    
  public:
    // constructor and destructor
    collider() {
      colliders.insert(this);
    }
    ~collider() {
      // frees all collider shapes
      while (shapes.size()) {
        freeshape(*shapes.begin());
        shapes.erase(shapes.begin());
      }
      
      colliders.erase(this);
    }
    
    virtual gear2d::component::family family() { return "collider"; }
    
    virtual gear2d::component::type type() { return "collider"; }
    
    virtual std::string depends() { return "kinematics/kinematic2d"; }
    
    // setup phase, to initialize paramters and other stuff
    virtual void setup(object::signature & sig) {
      set<string> shape_names;
      split(shape_names, sig["collider.shapes"], ' ');
      
      while (shape_names.size()) {
        loadshape(sig, *shape_names.begin());
        shape_names.erase(shape_names.begin());
      }
    }
    
    // looks for a shape type and calls the correct constructor
    void loadshape(object::signature & sig, const string& shape_name) {
      string type = sig["collider." + shape_name + ".type"];
      
      if (type == "rectangle")
        shapes.insert(new rectangle(this, sig, shape_name));
      else if (type == "circle")
        shapes.insert(new circle(this, sig, shape_name));
      else
        throw evil("Unknown geometrical shape type creation inside collider component");
    }
    
    // frees all interactions related to a shape and the shape itself
    void freeshape(shape* sh) {
      set<interaction>::iterator it = interactions.begin(), ittmp;
      while (it != interactions.end()) {
        ittmp = it;
        ++it;
        if (ittmp->shape1 == sh || ittmp->shape2 == sh)
          interactions.erase(ittmp);
      }
      delete sh;
    }
    
    // runs the big loop to check collision between alls pairs of interaction
    virtual void update(timediff dt, int begin) {
      // avoiding collider component update more than once by frame
      if (update_timestamp == begin)
        return;
      update_timestamp = begin;
      
      // globalupdate function is called until it doesn't create any interaction
      do {
        interaction::interactions_changed = false;
        globalupdate(dt, begin);
      } while (interaction::interactions_changed);
    }
    
    // check all collision interactions
    void globalupdate(timediff dt, int begin) {
      for (set<interaction>::iterator it = interactions.begin(); it != interactions.end(); ++it) {
        interaction& tmp = (interaction&)*it;
        
        // avoiding collision interaction check more than once by frame
        if (tmp.update_timestamp == begin)
          continue;
        tmp.update_timestamp = begin;
        
        // checks collision
        tmp.checkcollision(dt);
      }
    }
};

// static vars

bool collider::interaction::interactions_changed = false;

set<collider*> collider::colliders;
set<collider::interaction> collider::interactions;
int collider::update_timestamp = -1;

// the build function
g2dcomponent(collider)
