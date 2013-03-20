
#ifndef SHAPES_H
#define SHAPES_H

#include "gear2d.h"

using namespace gear2d;

class shape {
  protected:
    gear2d::link<float> x0, y0;
    gear2d::link<float> x, y;
    
  public:
    shape(component::base* owner, object::signature & sig, const std::string& name) {
      // init x0, y0
      x0 = owner->fetch<float>("x");
      y0 = owner->fetch<float>("y");
      
      // init x
      owner->write(name + "x", eval<float>(sig[name + "x"]));
      x = owner->fetch<float>(name + "x");
      
      // init y
      owner->write(name + "y", eval<float>(sig[name + "y"]));
      y = owner->fetch<float>(name + "y");
    }
};

class rectangle : public shape {
  private:
    gear2d::link<float> w, h;
    
  public:
    rectangle(component::base* owner, object::signature & sig, const std::string& name)
    : shape(owner, sig, name) {
      // init w
      owner->write(name + "w", eval<float>(sig[name + "w"]));
      w = owner->fetch<float>(name + "w");
      
      // init h
      owner->write(name + "h", eval<float>(sig[name + "h"]));
      h = owner->fetch<float>(name + "h");
    }
};

class circle : public shape {
  private:
    gear2d::link<float> r;
    
  public:
    circle(component::base* owner, object::signature & sig, const std::string& name)
    : shape(owner, sig, name) {
      // init r
      owner->write(name + "r", eval<float>(sig[name + "r"]));
      r = owner->fetch<float>(name + "r");
    }
};

#endif
