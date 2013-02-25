
#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include "gear2d.h"

#define rad2deg(X)  (X*180/M_PI)
#define deg2rad(X)  (X*M_PI/180)

using namespace gear2d;

class vector3 {
  public:
    class divisionbyzero {};
    
  private:
    float x_;
    float y_;
    float z_;
    float length_;
    bool has_set;
    
  public:
    vector3(float x = 0, float y = 0, float x = 0)
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
        throw divisionbyzero();
      
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
        throw divisionbyzero();
      
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
    bool operator>(vector3& other) {
      return (length() > other.length());
    }
    bool operator<(vector3& other) {
      return (length() < other.length());
    }
    bool operator>=(vector3& other) {
      return (length() >= other.length());
    }
    bool operator<=(vector3& other) {
      return (length() <= other.length());
    }
    
    // this dot other
    float dot(const vector3& other) const {
      return x_*other.x_ + y_*other.y_ + z_*other.z_;
    }
    
    // angle in degrees between this and other
    float angle(vector3& other) {
      if (!length() || !other.length())
        throw divisionbyzero();
      
      return rad2deg(std::acos(dot(other)/(length_*other.length_)));
    }
    
    // this unit vector
    vector3 unitvec() {
      if (!length())
        throw divisionbyzero();
      
      vector3 v;
      v.x_ = x_/length_;
      v.y_ = y_/length_;
      v.z_ = z_/length_;
      return v;
    }
    void setunitvec() {
      if (!length())
        throw divisionbyzero();
      
      x_ /= length_;
      y_ /= length_;
      z_ /= length_;
    }
    
    // this projection over other
    vector3 proj(vector3& other) const {
      if (!other.length())
        throw divisionbyzero();
      
      vector3 v;
      float scalar = dot(other)/(other.length_*other.length_);
      x_ = other.x_*scalar;
      y_ = other.y_*scalar;
      z_ = other.z_*scalar;
      return v;
    }
    void setproj(vector3& other) {
      if (!other.length())
        throw divisionbyzero();
      
      float scalar = dot(other)/(other.length_*other.length_);
      x_ = other.x_*scalar;
      y_ = other.y_*scalar;
      z_ = other.z_*scalar;
    }
    
    // this rejection over other
    vector3 rej(vector3& other) const {
      if (!other.length())
        throw divisionbyzero();
      
      vector3 v;
      float scalar = dot(other)/(other.length_*other.length_);
      v.x_ = x_ - other.x_*scalar;
      v.y_ = y_ - other.y_*scalar;
      v.z_ = z_ - other.z_*scalar;
      return v;
    }
    void setrej(vector3& other) {
      if (!other.length())
        throw divisionbyzero();
      
      float scalar = dot(other)/(other.length_*other.length_);
      x_ -= other.x_*scalar;
      y_ -= other.y_*scalar;
      z_ -= other.z_*scalar;
    }
    
    // this scalar projection over other
    float scalarproj(vector3& other) const {
      if (!other.length())
        throw divisionbyzero();
      
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
    
    // other cross this
    vector3 oppositecross(const vector3& other) const {
      vector3 v;
      v.x_ = z_*other.y_ - y_*other.z_;
      v.y_ = x_*other.z_ - z_*other.x_;
      v.z_ = y_*other.x_ - x_*other.y_;
      return v;
    }
    void setoppositecross(const vector3& other) {
      vector3 v;
      v.x_ = z_*other.y_ - y_*other.z_;
      v.y_ = x_*other.z_ - z_*other.x_;
      v.z_ = y_*other.x_ - x_*other.y_;
      x_ = v.x_;
      y_ = v.y_;
      z_ = v.z_;
    }
    
    // rotates this around other by angle degrees
    vector3 rotate(float angle, vector3& other) const {
      vector3 v;
      vector3 u = other.unitvec();
      float sint = std::sin(deg2rad(angle));
      float cost = std::cos(deg2rad(angle));
      v.x_ = x_*(u.x_*u.x_*(1 - cost) +      cost)  +  y_*(u.x_*u.y_*(1 - cost) - u.z_*sint)  +  z_*(u.x_*u.z_*(1 - cost) + u.y_*sint);
      v.y_ = x_*(u.x_*u.y_*(1 - cost) + u.z_*sint)  +  y_*(u.y_*u.y_*(1 - cost) +      cost)  +  z_*(u.y_*u.z_*(1 - cost) - u.x_*sint);
      v.z_ = x_*(u.x_*u.z_*(1 - cost) - u.y_*sint)  +  y_*(u.y_*u.z_*(1 - cost) + u.x_*sint)  +  z_*(u.z_*u.z_*(1 - cost) +      cost);
      return v;
    }
    void setrotate(float angle, vector3& other) {
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
    
    // gear2d component communication
    void write(component::base & com, const std::string & prefix = "") const {
      std::string my_prefix = prefix;
      if (prefix.size())
        my_prefix += ".";
      com.write<float>(my_prefix + "x", x);
      com.write<float>(my_prefix + "y", y);
      com.write<float>(my_prefix + "z", z);
    }
    void read(component::base & com, const std::string & prefix = "") const {
      std::string my_prefix = prefix;
      if (prefix.size())
        my_prefix += ".";
      com.read<float>(my_prefix + "x", x);
      com.read<float>(my_prefix + "y", y);
      com.read<float>(my_prefix + "z", z);
    }
    static vector3 from(component::base & com, const std::string & prefix = "") {
      vector3 v;
      v.read(com, prefix);
      return v;
    }
};

#endif
