#pragma once
#include <cmath>

class Vector3 {
public:
    float x, y, z;
    
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    

    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    


    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    
    Vector3 Normalized() const {
        float length = Length();
        if (length < 0.0001f) {
            return Vector3(0, 0, 0);
        }
        return Vector3(x / length, y / length, z / length);
    }
    
    void Normalize() {
        float length = Length();
        if (length < 0.0001f) return;
        x /= length;
        y /= length;
        z /= length;
    }
    
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float SquaredLength() const {
        return x * x + y * y + z * z;
    }
};

inline Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
} 