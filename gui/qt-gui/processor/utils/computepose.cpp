#include "computepose.h"

#include "calibration/calibrationprocess.h"

ComputePose::ComputePose()
{

}

void ComputePose::setComponent(Mat K, Mat &distCoeff)
{
    this->cameraMatrix = K;
    this->distortionCoefficients = distCoeff;
    this->objectPoints = {Point3f(0.0, 0.0, 0.0), Point3f(0.0, 1.0, 0.0), Point3f(1.0, 0.0, 0.0), Point3f(1.0, 1.0, 0.0)};
}

void ComputePose::computePose(Mat &frame, vector<Point2f> imagePointsVec)
{
    //estimate pattern pose
    Mat rotVec, R, t;
    //ricava la posa di un oggetto a partire da punti 3d e punti 2d. Risolve la matrice proiettiva
    solvePnP(objectPoints, imagePointsVec, cameraMatrix, distortionCoefficients, rotVec, t);
    Rodrigues(rotVec, R); //trasforma il vettore 3*1 in una matrice di rotazione 3*3

    vector<Point2f> reprojImagePoints;
    //in output il vettore di punti immagine riproiettati
    projectPoints(objectPoints, rotVec, t, cameraMatrix, distortionCoefficients, reprojImagePoints);

    Mat reprojectionMat, Ext;
    //mi mettono nel sistema di riferimento della telecamera
    hconcat(R, t, Ext);
    reprojectionMat = cameraMatrix * Ext; // matrice 4*3, matrice di riproiezione

    Mat Himg2scene, Hscene2img;
    hconcat(reprojectionMat(Range(0,3), Range(0,2)), reprojectionMat.col(3), Hscene2img);
    Himg2scene = Hscene2img.inv();

    Matx31d vx = reprojectionMat.col(0);
    Matx31d vy = reprojectionMat.col(1);
    Matx31d vz = reprojectionMat.col(2);
    Matx31d o = reprojectionMat.col(3);

    vx(0) /= vx(2); //punti dell'immagine. la terza variabile è il piano Z 0
    vx(1) /= vx(2);
    vx(2) = 1.0;

    vy(0) /= vy(2);
    vy(1) /= vy(2);
    vy(2) = 1.0;

    vz(0) /= vz(2);
    vz(1) /= vz(2);
    vz(2) = 1.0;

    o(0) /= o(2);
    o(1) /= o(2);
    o(2) = 1.0;

    vector<Point3f> scene_axis_point;
    vector<Point2f> projected_axis_point;
    scene_axis_point.push_back(Point3f(0.5, 0, 0));
    scene_axis_point.push_back(Point3f(0, 0.5, 0));
    scene_axis_point.push_back(Point3f(0, 0, 0.5));

    projectPoints(scene_axis_point, rotVec, t, cameraMatrix, Mat::zeros(1, 5, CV_64FC1), projected_axis_point);

    arrowedLine(frame, Point2f(o(0), o(1)), projected_axis_point[0], Scalar(255, 0, 0), 2);
    arrowedLine(frame, Point2f(o(0), o(1)), projected_axis_point[1], Scalar(0, 255, 0), 2);
    arrowedLine(frame, Point2f(o(0), o(1)), projected_axis_point[2], Scalar(0, 0, 255), 2);

    putText(frame, "X", projected_axis_point[0], 1, 2, Scalar(255, 0, 0));
    putText(frame, "Y", projected_axis_point[1], 1, 2, Scalar(0, 255, 0));
    putText(frame, "Z", projected_axis_point[2], 1, 2, Scalar(0, 0, 255));
}
