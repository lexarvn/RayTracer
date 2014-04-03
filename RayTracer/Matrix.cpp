#include "Matrix.hpp"

Matrix& Matrix::operator+= (const Matrix & right_op)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            x[i][j] += right_op.x[i][j];
    return *this;
}
Matrix& Matrix::operator-= (const Matrix & right_op)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            x[i][j] -= right_op.x[i][j];
    return *this;
}
Matrix& Matrix::operator*= (double right_op)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            x[i][j] *= right_op;
    return *this;
}
Matrix& Matrix::operator*= (const Matrix & right_op)
{
    Matrix ret = *this;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            double sum = 0;
            for (int k = 0; k < 4; k++)
                sum += ret.x[i][k] * right_op.x[k][j] ;
            x[i][j] = sum;
        }
    return *this;
}

void Matrix::invert()
{
    double det = determinant();
    Matrix inverse;
    inverse.x[0][0] =  det3(x[1][1], x[1][2], x[1][3],
                            x[2][1], x[2][2], x[2][3],
                            x[3][1], x[3][2], x[3][3]) / det;
    
    inverse.x[0][1] = -det3(x[0][1], x[0][2], x[0][3],
                            x[2][1], x[2][2], x[2][3],
                            x[3][1], x[3][2], x[3][3]) / det;
    
    inverse.x[0][2] =  det3(x[0][1], x[0][2], x[0][3],
                            x[1][1], x[1][2], x[1][3],
                            x[3][1], x[3][2], x[3][3]) / det;
    
    inverse.x[0][3] = -det3(x[0][1], x[0][2], x[0][3],
                            x[1][1], x[1][2], x[1][3],
                            x[2][1], x[2][2], x[2][3]) / det;
    
    
    inverse.x[1][0] = -det3(x[1][0], x[1][2], x[1][3],
                            x[2][0], x[2][2], x[2][3],
                            x[3][0], x[3][2], x[3][3]) / det;
    
    inverse.x[1][1] =  det3(x[0][0], x[0][2], x[0][3],
                            x[2][0], x[2][2], x[2][3],
                            x[3][0], x[3][2], x[3][3]) / det;
    
    inverse.x[1][2] = -det3(x[0][0], x[0][2], x[0][3],
                            x[1][0], x[1][2], x[1][3],
                            x[3][0], x[3][2], x[3][3]) / det;
    
    inverse.x[1][3] =  det3(x[0][0], x[0][2], x[0][3],
                            x[1][0], x[1][2], x[1][3],
                            x[2][0], x[2][2], x[2][3]) / det;
    
    
    inverse.x[2][0] =  det3(x[1][0], x[1][1], x[1][3],
                            x[2][0], x[2][1], x[2][3],
                            x[3][0], x[3][1], x[3][3]) / det;
    
    inverse.x[2][1] = -det3(x[0][0], x[0][1], x[0][3],
                            x[2][0], x[2][1], x[2][3],
                            x[3][0], x[3][1], x[3][3]) / det;
    
    inverse.x[2][2] =  det3(x[0][0], x[0][1], x[0][3],
                            x[1][0], x[1][1], x[1][3],
                            x[3][0], x[3][1], x[3][3]) / det;
    
    inverse.x[2][3] = -det3(x[0][0], x[0][1], x[0][3],
                            x[1][0], x[1][1], x[1][3],
                            x[2][0], x[2][1], x[2][3]) / det;
    
    
    inverse.x[3][0] = -det3(x[1][0], x[1][1], x[1][2],
                            x[2][0], x[2][1], x[2][2],
                            x[3][0], x[3][1], x[3][2]) / det;
    
    inverse.x[3][1] =  det3(x[0][0], x[0][1], x[0][2],
                            x[2][0], x[2][1], x[2][2],
                            x[3][0], x[3][1], x[3][2]) / det;
    
    inverse.x[3][2] = -det3(x[0][0], x[0][1], x[0][2],
                            x[1][0], x[1][1], x[1][2],
                            x[3][0], x[3][1], x[3][2]) / det;
    
    inverse.x[3][3] =  det3(x[0][0], x[0][1], x[0][2],
                            x[1][0], x[1][1], x[1][2],
                            x[2][0], x[2][1], x[2][2]) / det;
    
    *this = inverse;
}


Matrix Matrix::getInverse()const
{
    auto ret = Matrix(*this);
    ret.invert();
    return ret;
}

void Matrix::transpose()
{
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
    {
        double temp = x[i][j];
        x[j][i] = temp;
    }
}

Matrix Matrix::getTranspose()const
{
    Matrix ret = *this;
    ret.transpose();
    return ret;
}

double Matrix::determinant()
{
    double det;
    det = x[0][0] * det3(x[1][1], x[1][2], x[1][3],
                         x[2][1], x[2][2], x[2][3],
                         x[3][1], x[3][2], x[3][3]);
    
    det-= x[0][1] * det3(x[1][0], x[1][2], x[1][3],
                         x[2][0], x[2][2], x[2][3],
                         x[3][0], x[3][2], x[3][3]);
    
    det+= x[0][2] * det3(x[1][0], x[1][1], x[1][3],
                         x[2][0], x[2][1], x[2][3],
                         x[3][0], x[3][1], x[3][3]);
    
    det-= x[0][3] * det3(x[1][0], x[1][1], x[1][2],
                         x[2][0], x[2][1], x[2][2],
                         x[3][0], x[3][1], x[3][2]);
    return det;
}

// transform a vector by matrix
Vector operator* (const Matrix & left_op, const Vector & right_op)
{
    Vector ret;
    ret.x = right_op.x * left_op.x[0][0] + right_op.y * left_op.x[0][1] + right_op.z * left_op.x[0][2] + left_op.x[0][3];
    ret.y = right_op.x * left_op.x[1][0] + right_op.y * left_op.x[1][1] + right_op.z * left_op.x[1][2] + left_op.x[1][3];
    ret.z = right_op.x * left_op.x[2][0] + right_op.y * left_op.x[2][1] + right_op.z * left_op.x[2][2] + left_op.x[2][3] ;
    double temp = right_op.x * left_op.x[3][0] + right_op.y * left_op.x[3][1] + right_op.z * left_op.x[3][2] + left_op.x[3][3];
    ret *= 1/temp;
    return ret;
}

Vector transformLoc(const Matrix& left_op, const Vector& right_op)
{
    return (left_op * right_op);
}

Vector transformVec(const Matrix& left_op, const Vector& right_op)
{
    Vector ret;
    ret.x = right_op.x * left_op.x[0][0] + right_op.y * left_op.x[0][1] + right_op.z * left_op.x[0][2] ;
    ret.y = right_op.x * left_op.x[1][0] + right_op.y * left_op.x[1][1] + right_op.z * left_op.x[1][2] ;
    ret.z = right_op.x * left_op.x[2][0] + right_op.y * left_op.x[2][1] + right_op.z * left_op.x[2][2] ;
    return ret;
}

Matrix zeroMatrix()
{
    Matrix ret;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret.x[i][j] = 0.0;
    return ret;
}

Matrix identityMatrix()
{
    Matrix ret;
    ret.x[0][0] = 1.0;
    ret.x[0][1] = 0.0;
    ret.x[0][2] = 0.0;
    ret.x[0][3] = 0.0;
    ret.x[1][0] = 0.0;
    ret.x[1][1] = 1.0;
    ret.x[1][2] = 0.0;
    ret.x[1][3] = 0.0;
    ret.x[2][0] = 0.0;
    ret.x[2][1] = 0.0;
    ret.x[2][2] = 1.0;
    ret.x[2][3] = 0.0;
    ret.x[3][0] = 0.0;
    ret.x[3][1] = 0.0;
    ret.x[3][2] = 0.0;
    ret.x[3][3] = 1.0;
    return ret;
}

Matrix translate (double _x, double _y, double _z)
{
    Matrix ret = identityMatrix();
    ret.x[0][3] = _x;
    ret.x[1][3] = _y;
    ret.x[2][3] = _z;
    return ret;
}

// angle is in radians
Matrix rotateX (double angle)
{
    Matrix ret = identityMatrix();
    double cosine = cos(angle);
    double sine = sin(angle);
    ret.x[1][1] = cosine;
    ret.x[1][2] = -sine;
    ret.x[2][1] = sine;
    ret.x[2][2] = cosine;
    return ret;
}

// angle is in radians
Matrix rotateY (double angle)
{
    Matrix ret = identityMatrix();
    double cosine = cos(angle);
    double sine = sin(angle);
    ret.x[0][0] = cosine;
    ret.x[0][2] = sine;
    ret.x[2][0] = -sine;
    ret.x[2][2] = cosine;
    return ret;
}

// angle is in radians
Matrix rotateZ (double angle)
{
    Matrix ret = identityMatrix();
    double cosine = cos(angle);
    double sine = sin(angle);
    ret.x[0][0] = cosine;
    ret.x[0][1] = -sine;
    ret.x[1][0] = sine;
    ret.x[1][1] = cosine;
    return ret;
}

// rotation is in radians about an arbitrary axis
Matrix rotate(const Vector& axis, double angle)
{
    Vector _axis = axis;
    _axis.normalize();
    Matrix ret;

    double x = _axis.x;
    double y = _axis.y;
    double z = _axis.z;
    double cosine = cos(angle);
    double sine = sin(angle);
    double t = 1 - cosine;
    ret.x[0][0] = t * x * x + cosine;
    ret.x[0][1] = t * x * y - sine * y;
    ret.x[0][2] = t * x * z + sine * y;
    ret.x[0][3] = 0.0;
    ret.x[1][0] = t * x * y + sine * z;
    ret.x[1][1] = t * y * y + cosine;
    ret.x[1][2] = t * y * z - sine * x;
    ret.x[1][3] = 0.0;
    ret.x[2][0] = t * x * z - sine * y;
    ret.x[2][1] = t * y * z + sine * x;
    ret.x[2][2] = t * z * z + cosine;
    ret.x[2][3] = 0.0;
    ret.x[3][0] = 0.0;
    ret.x[3][1] = 0.0;
    ret.x[3][2] = 0.0;
    ret.x[3][3] = 1.0;
    return ret;
}

Matrix scale(double _x, double _y, double _z)
{
    Matrix ret = zeroMatrix();
    ret.x[0][0] = _x;
    ret.x[1][1] = _y;
    ret.x[2][2] = _z;
    ret.x[3][3] = 1.0;
    return ret;
}

// creates viewing Matrix that so the ey is at xyz origin looking down the -z axis
Matrix viewMatrix(const Vector& eye, const Vector& gaze, const Vector& up)
{
    Matrix ret = identityMatrix();

    // create an orthoganal basis from parameters
    Vector w = -gaze;
    w.normalize();
    Vector u = up.cross(w);
    u.normalize();
    Vector v = w.cross(u);
    v.normalize();

    // rotate orthoganal basis to xyz basis
    ret.x[0][0] = u.x;
    ret.x[0][1] = u.y;
    ret.x[0][2] = u.z;
    ret.x[1][0] = v.x;
    ret.x[1][1] = v.y;
    ret.x[1][2] = v.z;
    ret.x[2][0] = w.x;
    ret.x[2][1] = w.y;
    ret.x[2][2] = w.z;

    // translate eye to xyz origin
    Matrix move = identityMatrix();
    move.x[0][3] = -(eye.x);
    move.x[1][3] = -(eye.y);
    move.x[2][3] = -(eye.z);
    ret = ret * move;
    return ret;
}

Matrix operator+ (const Matrix & left_op, const Matrix & right_op)
{
    auto ret = Matrix(left_op);
    ret += right_op;
    return ret;
}

Matrix operator- (const Matrix & left_op, const Matrix & right_op)
{
    auto ret = Matrix(left_op);
    ret -= right_op;
    return ret;
}

Matrix operator* (const Matrix & left_op, const Matrix & right_op)
{
    auto ret = Matrix(left_op);
    ret *= right_op;
    return ret;
}

Matrix operator* (const Matrix & left_op, double right_op)
{
    auto ret = Matrix(left_op);
    ret *= right_op;
    return ret;
}

std::ostream & operator<< (std::ostream & out, const Matrix & right_op)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            out << std::setprecision(3) << std::setw(10) << right_op.x[i][j];
        out << std::endl;
    }
    return out;
}

