
#ifndef SHAPES_H
#define SHAPES_H

#include "gear2d.h"
#include "linearalgebra.h"

using namespace std;
using namespace gear2d;

// every new shape class implementation must add lines in the
// marked points of this file.

// MARK ONE: declaration of all shapes
class rectangle;
class circle;

// shape base class
class shape {
  private:
    typedef bool (shape::*checkcallback)(shape*, const vector3&, const vector3&);
    
  protected:
    component::base* owner;
    string name;
    
    // object position
    gear2d::link<float> x0, y0;
    
    // shape position. relativity varies depending on geometric shape
    gear2d::link<float> x, y;
    
    // temporary storage for checkcollision function
    float xaccel, yaccel, xspeed, yspeed;
    
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
    
  private:
    // function to add next object position to shape position
    vector3 getpos(timediff dt) const {
      return vector3(
        x + x0 + xspeed*dt + xaccel*dt*dt*0.5,
        y + y0 + yspeed*dt + yaccel*dt*dt*0.5
      );
    }
    
  public:
    // uses interpolation to check collision over interpolation steps.
    // calls the collision detection function according to the type of shape.
    bool checkcollision(timediff dt, shape* other) {
      int interpolation_steps = 1;  //TODO change for scene const parameter
      timediff step = dt/interpolation_steps;
      string other_type = other->type();
      checkcallback ccb;
      
      // MARK TWO: routing function
      if (other_type == "rectangle")
        ccb = (checkcallback)&shape::collidesrectangle;
      else if (other_type == "circle")
        ccb = (checkcallback)&shape::collidescircle;
      
      // iterates over all steps to check collision
      owner->read<float>("x.accel", xaccel); owner->read<float>("y.accel", yaccel);
      owner->read<float>("x.speed", xspeed); owner->read<float>("y.speed", yspeed);
      for (timediff local_dt = 0; local_dt <= dt; local_dt += step) {
        if ((this->*ccb)(other, getpos(local_dt), other->getpos(local_dt)))
          return true;
      }
      
      return false;
    }
    
  private:
    // MARK THREE: abstract functions
    virtual string type() const = 0;
    virtual bool collidesrectangle(rectangle*, const vector3&, const vector3&) = 0;
    virtual bool collidescircle(circle*, const vector3&, const vector3&) = 0;
};

class rectangle : public shape {
  private:
    // in this class, x and y are the position of the left upper corner of
    // the rectangle, when theta is equals to zero
    
    // width and height
    gear2d::link<float> w, h;
    
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
      
      if (!w || !h)
        throw evil("Trying to create rectangle without width and/or height inside rectangle shape class");
    }
    
  private:
    string type() const { return "rectangle"; }
    
    // IMPLEMENTATION MARK: abstract functions implementation
    bool collidesrectangle(rectangle* other, const vector3& this_pos, const vector3& other_pos) {
      return (
         this_pos.x() <= other_pos.x() + other->w &&
        other_pos.x() <=  this_pos.x() +        w &&
         this_pos.y() <= other_pos.y() + other->h &&
        other_pos.y() <=  this_pos.y() +        h
      );
    }
    bool collidescircle(circle* other, const vector3& this_pos, const vector3& other_pos) {
      return false;
    }
};

class circle : public shape {
  private:
    // in this class, x and y are the position of the center of the circle
    
    // radius
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
    
  private:
    string type() const { return "circle"; }
    
    // IMPLEMENTATION MARK: abstract functions implementation
    bool collidesrectangle(rectangle* other, const vector3& this_pos, const vector3& other_pos) {
      return false;
    }
    bool collidescircle(circle* other, const vector3& this_pos, const vector3& other_pos) {
      return ((this_pos - other_pos).length() <= r + other->r);
    }
};

#endif
