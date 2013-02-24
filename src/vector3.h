#ifndef VECTOR3_H
#define VECTOR3_H

#include "gear2d.h"
#include <cmath>

#define rad2deg(X)  (X*180/M_PI)
#define deg2rad(X)  (X*M_PI/180)

using namespace gear2d;
using std::sqrt;

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
        length_ = sqrt(x*x + y*y + z*z);
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
    
    // arithmetic assignment operators
    
    vector3& operator=(float scalar) {
      has_set = true;
      x_ = scalar;
      y_ = scalar;
      z_ = scalar;
      return *this;
    }
    
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
    
    vector3& setopposite() {
      x_ = -x_;
      y_ = -y_;
      z_ = -z_;
      return *this;
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
    
    float operator*(const vector3& other) const {
      return x_*other.x_ + y_*other.y_ + z_*other.z_;
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
    
    // non-trivial operations
    
    float angle(vector3& other) {
      if (!length() || !other.length())
        throw divisionbyzero();
      
      return rad2deg(acos(((*this)*other)/(length_*other.length_)));
    }
    
    vector3 unitvec() {
      return (*this)/length();
    }
    
    vector3& setunitvec() {
      (*this) /= length();
      return *this;
    }
    
    // my projection over the other vector
    vector3 proj(vector3& other) const {
      if (!other.length())
        throw divisionbyzero();
      
      vector3 v;
      float scalar = ((*this)*other)/(other.length_*other.length_);
      x_ = other.x_*scalar;
      y_ = other.y_*scalar;
      z_ = other.z_*scalar;
      return v;
    }
    
    // my projection over the other vector [assignment]
    vector3& setproj(vector3& other) {
      if (!other.length())
        throw divisionbyzero();
      
      float scalar = ((*this)*other)/(other.length_*other.length_);
      x_ = other.x_*scalar;
      y_ = other.y_*scalar;
      z_ = other.z_*scalar;
      return *this;
    }
    
    // my rejection over the other vector
    vector3 rej(vector3& other) const {
      if (!other.length())
        throw divisionbyzero();
      
      vector3 v;
      float scalar = ((*this)*other)/(other.length_*other.length_);
      v.x_ = x_ - other.x_*scalar;
      v.y_ = y_ - other.y_*scalar;
      v.z_ = z_ - other.z_*scalar;
      return v;
    }
    
    // my rejection over the other vector [assignment]
    vector3& setrej(vector3& other) {
      if (!other.length())
        throw divisionbyzero();
      
      float scalar = ((*this)*other)/(other.length_*other.length_);
      x_ -= other.x_*scalar;
      y_ -= other.y_*scalar;
      z_ -= other.z_*scalar;
      return *this;
    }
    
    // my scalar projection over the other vector
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
    
    // this cross other [assignment]
    vector3& setcross(const vector3& other) {
      vector3 v;
      v.x_ = y_*other.z_ - z_*other.y_;
      v.y_ = z_*other.x_ - x_*other.z_;
      v.z_ = x_*other.y_ - y_*other.x_;
      x_ = v.x_;
      y_ = v.y_;
      z_ = v.z_;
      return *this;
    }
    
    // other cross this
    vector3 oppositecross(const vector3& other) const {
      vector3 v;
      v.x_ = z_*other.y_ - y_*other.z_;
      v.y_ = x_*other.z_ - z_*other.x_;
      v.z_ = y_*other.x_ - x_*other.y_;
      return v;
    }
    
    // other cross this [assignment]
    vector3& setoppositecross(const vector3& other) {
      vector3 v;
      v.x_ = z_*other.y_ - y_*other.z_;
      v.y_ = x_*other.z_ - z_*other.x_;
      v.z_ = y_*other.x_ - x_*other.y_;
      x_ = v.x_;
      y_ = v.y_;
      z_ = v.z_;
      return *this;
    }
    
    // rotates this around other by angle degrees
    vector3 rotate(float angle, const vector3& other) const {
      vector3 v;
      // TODO
      return v;
    }
    
    // rotates this around other by angle degrees [assignment]
    vector3& setrotate(float angle, const vector3& other) {
      // TODO
      return *this;
    }
};

#endif
