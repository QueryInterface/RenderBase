#pragma once

template <class T>
struct Rect {
    T Left;
    T Right;
    T Top;
    T Bottom;
};

template <class T>
struct Vector1 {
    Vector1() {}
    Vector1(T x_) : x(x_) {}
    T* data() const {return (T*)this;}
    T x;
};

template <class T>
struct Vector2 {
    Vector2() {}
    Vector2(T x_, T y_) : x(x_), y(y_) {}
    T* data() const {return (T*)this;}
    T x;
    T y;
};

template <class T>
struct Vector3 {
    Vector3() {}
    Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    T* data() const {return (T*)this;}
    T x;
    T y;
    T z;

    Vector3 operator+ (const Vector3& arg) {return Vector3(x + arg.x, y+arg.y, z+arg.z);}
    Vector3 operator+=(const Vector3& arg) {x += arg.x; y+= arg.y; z+= arg.z; return *this;}
};
template <class T>
bool    operator==(const Vector3<T>& arg1, const Vector3<T>& arg2) {return arg1.x == arg2.x && arg1.y == arg2.y && arg1.z == arg2.z;}

template <class T>
struct Vector4 {
    Vector4() {}
    Vector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
    T* data() const {return (T*)this;}
    T x;
    T y;
    T z;
    T w;
};