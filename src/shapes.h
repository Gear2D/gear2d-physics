
#ifndef SHAPES_H
#define SHAPES_H

#include "gear2d.h"

using namespace std;
using namespace gear2d;

// every new shape class implementation must add lines in the
// marked points of this file.

// MARK ONE: declaration of all shapes
class rectangle;
class circle;

// shape base class
class shape {
  protected:
    component::base* owner;
    string name;
    gear2d::link<float> x0, y0;
    gear2d::link<float> x, y;
    
  public:
    shape(component::base* owner, object::signature & sig, const string& name)
    : owner(owner), name("collider." + name + ".") {
      // init x0, y0
      x0 = owner->fetch<float>("x");
      y0 = owner->fetch<float>("y");
      
      // init x
      owner->write(this->name + "x", eval<float>(sig[this->name + "x"]));
      x = owner->fetch<float>(this->name + "x");
      
      // init y
      owner->write(this->name + "y", eval<float>(sig[this->name + "y"]));
      y = owner->fetch<float>(this->name + "y");
    }
    
    // calls the collision detection function according to the type of shape
    bool checkcollision(timediff dt, shape* other) {
      string other_type = other->type();
      
      // MARK TWO: routing function
      if (other_type == "rectangle")
        return collidesrectangle(dt, (rectangle*)other);
      else if (other_type == "circle")
        return collidescircle(dt, (circle*)other);
      
      return false;
    }
    
    // MARK THREE: abstract functions
    virtual string type() const = 0;
    virtual bool collidesrectangle(timediff dt, rectangle* other) = 0;
    virtual bool collidescircle(timediff dt, circle* other) = 0;
};

class rectangle : public shape {
  private:
    gear2d::link<float> w, h, theta;
    
  public:
    // FRIENDSHIP MARK
    friend class circle;
    
    rectangle(component::base* owner, object::signature & sig, const string& name)
    : shape(owner, sig, name) {
      // init w
      owner->write(this->name + "w", eval<float>(sig[this->name + "w"]));
      w = owner->fetch<float>(this->name + "w");
      
      // init h
      owner->write(this->name + "h", eval<float>(sig[this->name + "h"]));
      h = owner->fetch<float>(this->name + "h");
      
      // init theta
      owner->write(this->name + "theta", eval<float>(sig[this->name + "theta"]));
      theta = owner->fetch<float>(this->name + "theta");
      
      if (!w || !h)
        throw evil("Trying to create rectangle without width and/or height inside rectangle shape class");
    }
    
    string type() const { return "rectangle"; }
    
    // IMPLEMENTATION MARK: abstract functions implementation
    bool collidesrectangle(timediff dt, rectangle* other) {
      return false;
    }
    bool collidescircle(timediff dt, circle* other) {
      return false;
    }
};

class circle : public shape {
  private:
    gear2d::link<float> r;
    
  public:
    // FRIENDSHIP MARK
    friend class rectangle;
    
    circle(component::base* owner, object::signature & sig, const string& name)
    : shape(owner, sig, name) {
      // init r
      owner->write(this->name + "r", eval<float>(sig[this->name + "r"]));
      r = owner->fetch<float>(this->name + "r");
      
      if (!r)
        throw evil("Trying to create circle without radius inside circle shape class");
    }
    
    string type() const { return "circle"; }
    
    // IMPLEMENTATION MARK: abstract functions implementation
    bool collidesrectangle(timediff dt, rectangle* other) {
      return false;
    }
    bool collidescircle(timediff dt, circle* other) {
      return false;
    }
};

#endif
