
//all of the library that is included 

#include "fetchmovement.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/PoseArray.h"
#include "math.h"
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/Int8.h"
#include <sstream>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "tf/transform_datatypes.h"


geometry_msgs::PoseStamped pose;		// Allows for pose to be calculated
geometry_msgs::PoseStamped fetchPose;		// Information on fetch robot pose 
sensor_msgs::LaserScan base_scan;		// Information from laser scan


int qr = 15;                                    // Width of the QR Code 15
int dist_scale = 0.3;                           // Scale for distance
long double s = 0.0;
const double dist_scale_constant = qr*dist_scale;    // setup the qr code constant 
                                                                                                                                          double i;
double close_span = 5;
double small_angle = 0;
bool detect = false;
bool follow = false;

void ros_callback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
  pose.pose.position.x = msg.get()->pose.position.x;
  pose.pose.position.y = msg.get()->pose.position.y;
  pose.pose.position.z = msg.get()->pose.position.z;
  pose.pose.orientation = msg.get()->pose.orientation;
}


void ros_callback2(const std_msgs::Int8::ConstPtr& s)                                                  
{
    s = s.get()->data;
}

double quaternionToYaw(geometry_msgs::Quaternion q)                                                     // Internet research helped
{
    return std::atan2(2.0f * (q.w * q.z + q.x * q.y), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
}

void odomCallback(const nav_msgs::Odometry& r)                                                          // Odometry	
{
    fetchPose.pose = r.pose.pose;
}

void laserCallback(const sensor_msgs::LaserScan& msg)
{
    //base_scan.ranges = msg.ranges;
    base_scan = msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/visp_auto_tracker/object_position", 1000, ros_callback);
  ros::Subscriber sub2 = n.subscribe("/visp_auto_tracker/status", 1000, ros_callback2);
  ros::Subscriber fetch_odom = n.subscribe("/odom", 1000, odomCallback);
  ros::Subscriber fetch_basescan = n.subscribe("/base_scan", 1000, laserCallback);
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel1", 1000);

  ros::Rate loop_rate(10);
  double something = abs(10);

  FetchMovement fetchM;       
  double lastX = 0;             
  double lastYaw = 0;           
  double previous_status = 0;    

  while (ros::ok())
  {
    // Variable to contol fetch linear + angular velocity
    double lin = 0;
    double ang = 0;
    double roll, pitch, yaw;
    fetchM.rpyFromPose(pose, roll, pitch, yaw);
    double fetchRoll, fetchPitch, fetchYaw;
    fetchM.rpyFromPose(fetchPose, fetchRoll, fetchPitch, fetchYaw);
    geometry_msgs::Twist msg2;
    double angle = atan2(pose.pose.position.x, pose.pose.position.z);
    double cord_x_distance = DIST_SCALE*pose.pose.position.x;
    double cord_z_distance = DIST_SCALE*pose.pose.position.z;
    double vision = std::sqrt(std::pow(cord_x_distance, 2) + std::pow(cord_z_distance, 2));
    //Calculations to make everything nwork 


    // QR code detected
    if((s == 0 || s == 1) && Detect == false)
    {
        lin = 0;
        ang = 0;

    } else {

        Detect = true;
    }
 

    for (unsigned int i=0; i < base_scan.ranges.size(); i++) {  

        if (base_scan.ranges.at(i) < close_range){
            close_range = base_scan.ranges.at(i);
            small_angle = i*base_scan.angle_increment*180/M_PI;
        }
    }



