#include "computepose.h"

#include "calibration/calibrationprocess.h"
#include "geometrictransformation.h"

void computePose(
        const Mat &cameraMatrix,
        const Mat &distortionCoefficients,
        const vector<Point3f> &objectPoints,
        const vector<Point2f> &imagePoints,
        Mat &t,
        Mat &rotVec,
        double &thetaX,
        double &thetaY)
{
    //ricava la posa di un oggetto a partire da punti 3d e punti 2d. Risolve la matrice proiettiva
    solvePnP(objectPoints, imagePoints, cameraMatrix, distortionCoefficients, rotVec, t);

    double tx = t.at<double>(0);
    double ty = t.at<double>(1);
    double tz = t.at<double>(2);
    double distance = norm(t, NORM_L2);
    thetaY = -atan2(tx, tz);
    thetaX = atan2(ty, distance);
}
