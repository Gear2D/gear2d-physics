
#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include <cmath>
#include <iostream>
#include "gear2d.h"

#undef  rad2deg
#undef  deg2rad
#define rad2deg(X)  (X*180/M_PI)
#define deg2rad(X)  (X*M_PI/180)

namespace gear2d {
  class evildivision : public evil {
    public:
      evildivision(const std::string what) throw() :
      evil("Division by zero inside \"" + what + "\"") {}
  };
}

class vector3 {
  public:
    // gear2d component communication
    void write(gear2d::component::base & com, const std::string & prefix = "") const {
      std::string my_prefix = prefix;
      if (prefix.size())
        my_prefix += ".";
      com.write<float>(my_prefix + "x", x_);
      com.write<float>(my_prefix + "y", y_);
      com.write<float>(my_prefix + "z", z_);
    }
    void read(gear2d::component::base & com, const std::string & prefix = "") {
      std::string my_prefix = prefix;
      if (prefix.size())
        my_prefix += ".";
      com.read<float>(my_prefix + "x", x_);
      com.read<float>(my_prefix + "y", y_);
      com.read<float>(my_prefix + "z", z_);
    }
    static vector3 from(gear2d::component::base & com, const std::string & prefix = "") {
      vector3 v;
      v.read(com, prefix);
      return v;
    }
    
  private:
    float x_;
    float y_;
    float z_;
    float length_;
    bool has_set;
    
  public:
    vector3(float x = 0, float y = 0, float z = 0)
    : x_(x), y_(y), z_(z), has_set(true)
    {
    }
    
    // getters
    float x() const {
      return x_;
    }
    float y() const {
      return y_;
    }
    float z() const {
      return z_;
    }
    float length() {
      if (has_set) {
        has_set = false;
        length_ = std::sqrt(x_*x_ + y_*y_ + z_*z_);
      }
      return length_;
    }
    
    // setters
    void setx(float x) {
      has_set = true;
      x_ = x;
    }
    void sety(float y) {
      has_set = true;
      y_ = y;
    }
    void setz(float z) {
      has_set = true;
      z_ = z;
    }
    void addx(float dx) {
      has_set = true;
      x_ += dx;
    }
    void addy(float dy) {
      has_set = true;
      y_ += dy;
    }
    void addz(float dz) {
      has_set = true;
      z_ += dz;
    }
    vector3& operator=(float scalar) {
      has_set = true;
      x_ = scalar;
      y_ = scalar;
      z_ = scalar;
      return *this;
    }
    
    // arithmetic assignment operators
    vector3& operator+=(const vector3& other) {
      has_set = true;
      x_ += other.x_;
      y_ += other.y_;
      z_ += other.z_;
      return *this;
    }
    vector3& operator-=(const vector3& other) {
      has_set = true;
      x_ -= other.x_;
      y_ -= other.y_;
      z_ -= other.z_;
      return *this;
    }
    vector3& operator*=(float scalar) {
      has_set = true;
      x_ *= scalar;
      y_ *= scalar;
      z_ *= scalar;
      return *this;
    }
    vector3& operator/=(float scalar) {
      if (!scalar)
        throw gear2d::evildivision("vector3& vector3::operator/=(float scalar)");
      
      has_set = true;
      x_ /= scalar;
      y_ /= scalar;
      z_ /= scalar;
      return *this;
    }
    void setopposite() {
      x_ = -x_;
      y_ = -y_;
      z_ = -z_;
    }
    
    // arithmetic operators
    vector3 operator+(const vector3& other) const {
      vector3 v;
      v.x_ = x_ + other.x_;
      v.y_ = y_ + other.y_;
      v.z_ = z_ + other.z_;
      return v;
    }
    vector3 operator-(const vector3& other) const {
      vector3 v;
      v.x_ = x_ - other.x_;
      v.y_ = y_ - other.y_;
      v.z_ = z_ - other.z_;
      return v;
    }
    vector3 operator*(float scalar) const {
      vector3 v;
      v.x_ = x_*scalar;
      v.y_ = y_*scalar;
      v.z_ = z_*scalar;
      return v;
    }
    vector3 operator/(float scalar) const {
      if (!scalar)
        throw gear2d::evildivision("vector3 vector3::operator/(float scalar) const");
      
      vector3 v;
      v.x_ = x_/scalar;
      v.y_ = y_/scalar;
      v.z_ = z_/scalar;
      return v;
    }
    vector3 operator-() const {
      vector3 v;
      v.x_ = -x_;
      v.y_ = -y_;
      v.z_ = -z_;
      return v;
    }
    
    // logical operators
    bool operator==(const vector3& other) const {
      return (x_ == other.x_ && y_ == other.y_ && z_ == other.z_);
    }
    bool operator!=(const vector3& other) const {
      return (x_ != other.x_ || y_ != other.y_ || z_ != other.z_);
    }
    bool operator>(const vector3& other) const {
      return (((vector3&)*this).length() > ((vector3&)other).length());
    }
    bool operator<(const vector3& other) const {
      return (((vector3&)*this).length() < ((vector3&)other).length());
    }
    bool operator>=(const vector3& other) const {
      return (((vector3&)*this).length() >= ((vector3&)other).length());
    }
    bool operator<=(const vector3& other) const {
      return (((vector3&)*this).length() <= ((vector3&)other).length());
    }
    
    // this dot other
    float dot(const vector3& other) const {
      return x_*other.x_ + y_*other.y_ + z_*other.z_;
    }
    
    // angle in degrees between this and other
    float angle(const vector3& other) const {
      if (!((vector3&)*this).length() || !((vector3&)other).length())
        throw gear2d::evildivision("float vector3::angle(const vector3& other) const");
      
      return rad2deg(std::acos(dot(other)/(this->length_*other.length_)));
    }
    
    // this unit vector
    vector3 unitvec() const {
      if (!((vector3&)*this).length())
        throw gear2d::evildivision("vector3 vector3::unitvec() const");
      
      vector3 v;
      v.x_ = x_/this->length_;
      v.y_ = y_/this->length_;
      v.z_ = z_/this->length_;
      return v;
    }
    void setunitvec() {
      if (!length())
        throw gear2d::evildivision("void vector3::setunitvec()");
      
      x_ /= length_;
      y_ /= length_;
      z_ /= length_;
    }
    
    // this projection over other
    vector3 proj(const vector3& other) const {
      if (!((vector3&)other).length())
        throw gear2d::evildivision("vector3 vector3::proj(const vector3& other) const");
      
      vector3 v;
      float scalar = dot(other)/(other.length_*other.length_);
      v.x_ = other.x_*scalar;
      v.y_ = other.y_*scalar;
      v.z_ = other.z_*scalar;
      return v;
    }
    void setproj(const vector3& other) {
      if (!((vector3&)other).length())
        throw gear2d::evildivision("void vector3::setproj(const vector3& other)");
      
      float scalar = dot(other)/(other.length_*other.length_);
      x_ = other.x_*scalar;
      y_ = other.y_*scalar;
      z_ = other.z_*scalar;
    }
    
    // this rejection over other
    vector3 rej(const vector3& other) const {
      if (!((vector3&)other).length())
        throw gear2d::evildivision("vector3 vector3::rej(const vector3& other) const");
      
      vector3 v;
      float scalar = dot(other)/(other.length_*other.length_);
      v.x_ = x_ - other.x_*scalar;
      v.y_ = y_ - other.y_*scalar;
      v.z_ = z_ - other.z_*scalar;
      return v;
    }
    void setrej(const vector3& other) {
      if (!((vector3&)other).length())
        throw gear2d::evildivision("void vector3::setrej(const vector3& other)");
      
      float scalar = dot(other)/(other.length_*other.length_);
      x_ -= other.x_*scalar;
      y_ -= other.y_*scalar;
      z_ -= other.z_*scalar;
    }
    
    // this scalar projection over other
    float scalarproj(const vector3& other) const {
      if (!((vector3&)other).length())
        throw gear2d::evildivision("float vector3::scalarproj(const vector3& other) const");
      
      return (x_*other.x_ + y_*other.y_ + z_*other.z_)/other.length_;
    }
    
    // this cross other
    vector3 cross(const vector3& other) const {
      vector3 v;
      v.x_ = y_*other.z_ - z_*other.y_;
      v.y_ = z_*other.x_ - x_*other.z_;
      v.z_ = x_*other.y_ - y_*other.x_;
      return v;
    }
    void setcross(const vector3& other) {
      vector3 v;
      v.x_ = y_*other.z_ - z_*other.y_;
      v.y_ = z_*other.x_ - x_*other.z_;
      v.z_ = x_*other.y_ - y_*other.x_;
      x_ = v.x_;
      y_ = v.y_;
      z_ = v.z_;
    }
    
    // rotates this around other by angle degrees
    vector3 rotate(float angle, const vector3& other) const {
      vector3 v;
      vector3 u = other.unitvec();
      float sint = std::sin(deg2rad(angle));
      float cost = std::cos(deg2rad(angle));
      v.x_ = x_*(u.x_*u.x_*(1 - cost) +      cost)  +  y_*(u.x_*u.y_*(1 - cost) - u.z_*sint)  +  z_*(u.x_*u.z_*(1 - cost) + u.y_*sint);
      v.y_ = x_*(u.x_*u.y_*(1 - cost) + u.z_*sint)  +  y_*(u.y_*u.y_*(1 - cost) +      cost)  +  z_*(u.y_*u.z_*(1 - cost) - u.x_*sint);
      v.z_ = x_*(u.x_*u.z_*(1 - cost) - u.y_*sint)  +  y_*(u.y_*u.z_*(1 - cost) + u.x_*sint)  +  z_*(u.z_*u.z_*(1 - cost) +      cost);
      return v;
    }
    void setrotate(float angle, const vector3& other) {
      vector3 v;
      vector3 u = other.unitvec();
      float sint = sin(deg2rad(angle));
      float cost = cos(deg2rad(angle));
      v.x_ = x_*(u.x_*u.x_*(1 - cost) +      cost)  +  y_*(u.x_*u.y_*(1 - cost) - u.z_*sint)  +  z_*(u.x_*u.z_*(1 - cost) + u.y_*sint);
      v.y_ = x_*(u.x_*u.y_*(1 - cost) + u.z_*sint)  +  y_*(u.y_*u.y_*(1 - cost) +      cost)  +  z_*(u.y_*u.z_*(1 - cost) - u.x_*sint);
      v.z_ = x_*(u.x_*u.z_*(1 - cost) - u.y_*sint)  +  y_*(u.y_*u.z_*(1 - cost) + u.x_*sint)  +  z_*(u.z_*u.z_*(1 - cost) +      cost);
      x_ = v.x_;
      y_ = v.y_;
      z_ = v.z_;
    }
    
    // shows the vector values
    void show() {
      std::cout << "vector3 at " << this << ": x = " << x_ << ", y = " << y_;
      std::cout << ", z = " << z_ << ", length = " << length() << std::endl;
    }
};

float det2(const vector3& a, const vector3& b) {
  return a.x()*b.y() - a.y()*b.x();
}

float det3(const vector3& a, const vector3& b, const vector3& c) {
  return
      a.x()*b.y()*c.z() + b.x()*c.y()*a.z() + c.x()*a.y()*b.z()
    - a.z()*b.y()*c.x() - b.z()*c.y()*a.x() - c.z()*a.y()*b.x()
  ;
}

bool linearsystem2(
  const vector3& a, const vector3& b, const vector3& c,
  float& x, float& y
) {
  float D = det2(a, b);
  if (!D) {
    x = 0;
    y = 0;
    return false;
  }
  
  float Dx = det2(c, b);
  float Dy = det2(a, c);
  
  x = Dx/D;
  y = Dy/D;
  
  return true;
}

bool linearsystem3(
  const vector3& a, const vector3& b, const vector3& c, const vector3& d,
  float& x, float& y, float& z
) {
  float D = det3(a, b, c);
  if (!D) {
    x = 0;
    y = 0;
    z = 0;
    return false;
  }
  
  float Dx = det3(d, b, c);
  float Dy = det3(a, d, c);
  float Dz = det3(a, b, d);
  
  x = Dx/D;
  y = Dy/D;
  z = Dz/D;
  
  return true;
}

#endif
