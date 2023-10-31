#ifndef FETCHMOVEMENT_H

#define FETCHMOVEMENT_H

#define V_MAX 0.5
#define V_MIN 0.05
#define W_MAX 0.5

#include "geometry_msgs/PoseArray.h"
#include <cmath>
#include <iostream>
#include "geometry_msgs/Pose2D.h"
#include "tf/transform_datatypes.h"
#include "nav_msgs/Odometry.h"
#include <fstream>



#define fowangchange 2*M_PI/180
#define Revangchange 2*M_PI/180

#define G_MAX (V_MAX * W_MAX)

#define Target 1.5 //cahnge this to make distance differnet 
#define CloseDIST (TARGET_DIST - 0.05)




class FetchMovement
{
public:
    
    FetchMovement();
    
    ~FetchMovement();
//r = range b = bearing 

   

void rpyFromPose(geometry_msgs::PoseStamped pose, double &fetchRoll, double &fetchPitch, double &fetchYaw);

void purePursuit2(double r, double x, double b, double &lin, double &ang, double &state);

   double normaliseAngle(double theta);

private:
    std::ofstream myfile;


};

 

