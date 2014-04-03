#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"
#include <iostream>
#include <iomanip>

struct Matrix
{
    Matrix(){}
    void invert();
    void transpose();
    Matrix getInverse()const;
    Matrix getTranspose()const;
    Matrix & operator+= (const Matrix& right_op);
    Matrix & operator-= (const Matrix& right_op);
    Matrix & operator*= (const Matrix& right_op);
    Matrix & operator*= (double right_op);
    
    double determinant();
    double x[4][4];
};

// 3x3 matrix determinant — helper function
inline double det3 (double a, double b, double c,
                    double d, double e, double f,
                    double g, double h, double i)
{ return a*e*i + d*h*c + g*b*f - g*e*c - d*b*i - a*h*f; }

Matrix operator- (const Matrix& left_op, const Matrix& right_op);
Matrix operator+ (const Matrix& left_op, const Matrix& right_op);
Matrix operator* (const Matrix& left_op, const Matrix& right_op);
Vector operator* (const Matrix& left_op, const Vector& right_op);
Matrix operator* (const Matrix & left_op, double right_op);
Vector transformLoc(const Matrix& left_op, const Vector& right_op);
Vector transformVec(const Matrix& left_op, const Vector& right_op);
Matrix zeroMatrix();
Matrix identityMatrix();
Matrix translate(double _x, double _y, double _z) ;
Matrix scale(double _x, double _y, double _z);
Matrix rotate(const Vector & axis, double angle);
Matrix rotateX(double angle); //
Matrix rotateY(double angle); // More efficient than arbitrary axis
Matrix rotateZ(double angle); //
Matrix viewMatrix(const Vector& eye, const Vector & gaze,
                         const Vector& up);
std::ostream & operator<< (std::ostream& out, const Matrix& right_op);

#endif