
#ifndef SHAPES_H
#define SHAPES_H

#include "gear2d.h"
#include "linearalgebra.h"

using namespace std;
using namespace gear2d;

// every new shape class implementation must add lines in the
// marked points of this file.

// CLASS LISTING MARK
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
      
      // FUNCTION ROUTING MARK
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
    // ABSTRACT FUNCTIONS MARK
    virtual string type() const = 0;
    virtual bool collidesrectangle(rectangle*, const vector3&, const vector3&) = 0;
    virtual bool collidescircle(circle*, const vector3&, const vector3&) = 0;
};

// CLASS DECLARATION MARK

class rectangle : public shape {
  private:
    // in this class, x and y are the position of the left upper corner of
    // the rectangle, when theta is equals to zero
    
    // width and height
    gear2d::link<float> w, h;
    
  public:
    // FRIENDSHIP MARK
    friend class circle;
    
    rectangle(component::base* owner, object::signature & sig, const string& name);
    
  private:
    string type() const;
    
    // ABSTRACT FUNCTIONS MARK
    bool collidesrectangle(rectangle* other, const vector3& this_pos, const vector3& other_pos);
    bool collidescircle(circle* other, const vector3& this_pos, const vector3& other_pos);
};

class circle : public shape {
  private:
    // in this class, x and y are the position of the center of the circle
    
    // radius
    gear2d::link<float> r;
    
  public:
    // FRIENDSHIP MARK
    friend class rectangle;
    
    circle(component::base* owner, object::signature & sig, const string& name);
    
  public:
    static bool linesegcollision(const vector3& v0, const vector3& v, const vector3& center, float radius);
    
  private:
    string type() const;
    
    // ABSTRACT FUNCTIONS MARK
    bool collidesrectangle(rectangle* other, const vector3& this_pos, const vector3& other_pos);
    bool collidescircle(circle* other, const vector3& this_pos, const vector3& other_pos);
};

// =============================================================================
// rectangle class implementation
// =============================================================================

rectangle::rectangle(component::base* owner, object::signature & sig, const string& name)
: shape(owner, sig, name) {
  // init w
  owner->write(this->name + "w", eval<float>(sig[this->name + "w"]));
  w = owner->fetch<float>(this->name + "w");
  
  // init h
  owner->write(this->name + "h", eval<float>(sig[this->name + "h"]));
  h = owner->fetch<float>(this->name + "h");
  
  if (w <= 0 || h <= 0)
    throw evil("Trying to create rectangle without width and/or height inside rectangle shape class");
}

string rectangle::type() const { return "rectangle"; }

// RECTANGLE ABSTRACT FUNCTIONS IMPLEMENTATION MARK

bool rectangle::collidesrectangle(rectangle* other, const vector3& this_pos, const vector3& other_pos) {
  // De Morgan of:
  // first rect totally right the second rect OR
  // second rect totally right the first rect OR
  // first rect totally below the second rect OR
  // second rect totally below the first rect
  return (
     this_pos.x() <= other_pos.x() + other->w &&
    other_pos.x() <=  this_pos.x() +        w &&
     this_pos.y() <= other_pos.y() + other->h &&
    other_pos.y() <=  this_pos.y() +        h
  );
}

bool rectangle::collidescircle(circle* other, const vector3& this_pos, const vector3& other_pos) {
  // other_pos represents the center of the circle. this condition checks
  // if this point is inside the rectangle, which means that
  // collision happens.
  if (
    other_pos.x() >= this_pos.x() && other_pos.x() < this_pos.x() + w &&
    other_pos.y() >= this_pos.y() && other_pos.y() < this_pos.y() + h
  )
    return true;
  
  // collision happens if any line segment of the rectangle collides with
  // the circle.
  vector3 horizontal = vector3(w, 0);
  vector3 vertical = vector3(0, h);
  vector3 upper_left = this_pos;
  vector3 upper_right = this_pos + horizontal;
  vector3 lower_left = this_pos + vertical;
  return (
    circle::linesegcollision(upper_left, horizontal, other_pos, other->r) ||
    circle::linesegcollision(upper_left, vertical, other_pos, other->r) ||
    circle::linesegcollision(upper_right, vertical, other_pos, other->r) ||
    circle::linesegcollision(lower_left, horizontal, other_pos, other->r)
  );
}

// =============================================================================
// circle class implementation
// =============================================================================

circle::circle(component::base* owner, object::signature & sig, const string& name)
: shape(owner, sig, name) {
  // init r
  owner->write(this->name + "r", eval<float>(sig[this->name + "r"]));
  r = owner->fetch<float>(this->name + "r");
  
  if (r <= 0)
    throw evil("Trying to create circle without radius inside circle shape class");
}

bool circle::linesegcollision(const vector3& v0, const vector3& v, const vector3& center, float radius) {
  // point of the line segment closest to the center of the circle
  vector3 closest;
  
  // the scalar projection of the position vector of the circle, relative to
  // the begining of the line segment, over the line segment
  float scalar_proj = (center - v0).scalarproj(v);
  
  // in this case, the closest point is the begining of the line segment
  if (scalar_proj < 0)
    closest = v0;
  // in this case, the closest point is the end of the line segment
  else if (scalar_proj > ((vector3&)v).length())
    closest = v0 + v;
  // in this case, the closest point is the begining of the line segment plus
  // the projection of the position vector of the circle, relative to the
  // begining of the line segment, over the line segment
  else
    closest = v0 + v.unitvec()*scalar_proj;
  
  // intersection happens if the shortest distance between the center and the
  // line segment is less or equal to the radius
  return ((center - closest).length() <= radius);
}

string circle::type() const { return "circle"; }

// CIRCLE ABSTRACT FUNCTIONS IMPLEMENTATION MARK

bool circle::collidesrectangle(rectangle* other, const vector3& this_pos, const vector3& other_pos) {
  // this_pos represents the center of the circle. this condition checks
  // if this point is inside the rectangle, which means that
  // collision happens.
  if (
    this_pos.x() >= other_pos.x() && this_pos.x() < other_pos.x() + other->w &&
    this_pos.y() >= other_pos.y() && this_pos.y() < other_pos.y() + other->h
  )
    return true;
  
  // collision happens if any line segment of the rectangle collides with
  // the circle.
  vector3 horizontal = vector3(other->w, 0);
  vector3 vertical = vector3(0, other->h);
  vector3 upper_left = other_pos;
  vector3 upper_right = other_pos + horizontal;
  vector3 lower_left = other_pos + vertical;
  return (
    circle::linesegcollision(upper_left, horizontal, this_pos, r) ||
    circle::linesegcollision(upper_left, vertical, this_pos, r) ||
    circle::linesegcollision(upper_right, vertical, this_pos, r) ||
    circle::linesegcollision(lower_left, horizontal, this_pos, r)
  );
}

bool circle::collidescircle(circle* other, const vector3& this_pos, const vector3& other_pos) {
  // collision happens if distance center-to-center is less or equal than the sum of the radii
  return ((this_pos - other_pos).length() <= r + other->r);
}

#endif
