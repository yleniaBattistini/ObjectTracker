#include "geometrictransformation.h"
#include <math.h>
#include <opencv2/calib3d.hpp>

Mat rot2euler(const Mat &rotationMatrix)
{
    Mat euler(3, 1, CV_64F);
    double m00 = rotationMatrix.at<double>(0, 0);
    double m10 = rotationMatrix.at<double>(1, 0);
    double m11 = rotationMatrix.at<double>(1, 1);
    double m12 = rotationMatrix.at<double>(1, 2);
    double m20 = rotationMatrix.at<double>(2, 0);
    double m21 = rotationMatrix.at<double>(2, 1);
    double m22 = rotationMatrix.at<double>(2, 2);
    double x, y, z;
    // Assuming the angles are in radians.
    if (m20 < 1)
    {
        if (m20 > -1)
        {
            x = atan2(m21, m22);
            y = asin(-m20);
            z = atan2(m10, m00);
        }
        else //m20 = -1
        {
            //Not a unique solution: x - z = atan2(-m12,m11)
            x = 0;
            y = CV_PI / 2;
            z = -atan2(-m12, m11);
        }
    }
    else //m20 = +1
    {
        //Not a unique solution: x + z = atan2(-m12,m11)
        x = 0;
        y = -CV_PI / 2;
        z = atan2(-m12, m11);
    }
    euler.at<double>(0) = x;
    euler.at<double>(1) = y;
    euler.at<double>(2) = z;
    return euler;
}

Mat euler2rot(const Mat &euler)
{
    Mat rotationMatrix(3,3,CV_64F);

    double x = euler.at<double>(0);
    double y = euler.at<double>(1);
    double z = euler.at<double>(2);

    // Assuming the angles are in radians.
    double ch = cos(z);
    double sh = sin(z);
    double ca = cos(y);
    double sa = sin(y);
    double cb = cos(x);
    double sb = sin(x);

    double m00, m01, m02, m10, m11, m12, m20, m21, m22;

    m00 = ch * ca;
    m01 = sh*sb - ch*sa*cb;
    m02 = ch*sa*sb + sh*cb;
    m10 = sa;
    m11 = ca*cb;
    m12 = -ca*sb;
    m20 = -sh*ca;
    m21 = sh*sa*cb + ch*sb;
    m22 = -sh*sa*sb + ch*cb;

    rotationMatrix.at<double>(0,0) = m00;
    rotationMatrix.at<double>(0,1) = m01;
    rotationMatrix.at<double>(0,2) = m02;
    rotationMatrix.at<double>(1,0) = m10;
    rotationMatrix.at<double>(1,1) = m11;
    rotationMatrix.at<double>(1,2) = m12;
    rotationMatrix.at<double>(2,0) = m20;
    rotationMatrix.at<double>(2,1) = m21;
    rotationMatrix.at<double>(2,2) = m22;

    return rotationMatrix;
}

Mat rotBetweenVectors(const Mat &v1, const Mat &v2)
{
    Mat direction = v1.cross(v2);
    double sin = norm(direction, NORM_L2);
    Mat rotationVector = direction / sin * asin(sin);
    Mat rotationMatrix;
    Rodrigues(rotationVector, rotationMatrix);
    return rotationMatrix;
}

Mat rotationAroundX(double theta)
{
    double s = sin(theta);
    double c = cos(theta);
    return (Mat_<double>(3, 3)
          << 1, 0, 0,
             0, c, -s,
             0, s, c);
}

Mat rotationAroundY(double theta)
{
    double s = sin(theta);
    double c = cos(theta);
    return (Mat_<double>(3, 3)
          << c, 0, s,
             0, 1, 0,
             -s, 0, c);
}

double linearInterpolation(double a, double b, double percentage)
{
    return a + percentage * (b - a);
}
