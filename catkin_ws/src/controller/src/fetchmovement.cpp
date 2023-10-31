//these headers are included to make the reobot funciton the way we like 
#include <cmath>
#include "fetchmovement.h"
#include "math.h"
#include <cstdlib>





FetchMovement::FetchMovement()
{
    myfile.open("pure_pursuit_log.txt");
}

FetchMovement::~FetchMovement()
{
    myfile.close();
}

void FetchMovement::purePursuit2(double r, double x, double b, double &lin, double &ang, double &s)
{
    
//r = range 
// s = state 
     double GR = r - Target;
    double GRmultifowrad = GR * 1.5;
   double GRmultibackward = GR * 1.5;
 double GOAL = GR*x/r;
bool prio = false;

    // Calculating the curvature of the path, gamma
    double gam = (2*std::abs(GOAL))/std::pow(GR, 2);

    
    // Check Distance. If in allowable range, don't move.
    if(r => CloseDIST && r <= Target)
    {
      //Checks if the QR tracker has found
      if(s == 3.0)
      {
        
        lin = 0;
        ang = 0;
        
      }
    }

    // IF GR >= 0 -> MOVE FORWARDS
    else if(GR >= 0 && s == 3.0)
    {
        

        // If the angle is relatively small, go full steam ahead
        if((-fowangchange <= b && b <= fowangchange) && s == 3.0)
        {
            
            lin = GRmultifowrad;
            if(lin > V_MAX)
            {
                lin = V_MAX;
            }
            else if(lin < V_MIN)
            {
                lin = V_MIN;
            }
            else (lin == V_MIN){
		ang = 0;
            }
            
        }

        // If bearing error is between 0 and PI, rotate clockwise 
       
        else if(fowangchange < b && b <= M_PI)
        {
            

            // Prioritise Linear Velocity
            if(prio == false)
            {
                lin = GR;
                if(lin > V_MAX)
                {
                    lin = V_MAX;
                }
                else if(lin < V_MIN)
                {
                    lin = V_MIN;
                }

                // Calc Angular Velocity using Gamma Proportionality
                ang = -(gam*lin);
                if(ang < -W_MAX)
                {
                    ang = -W_MAX;
                }
            }

            // Prioritise Angular Velocity
            else
            {
                ang = -b/2; //this will cahnge depending 

                if(-W_MAX> ang)
                {
                    ang = -W_MAX;
                }
                lin = std::abs(ang)/gam;
                if(GRmultifowrad < lin)
                {
                    lin = GRmultifowrad;
                    if(V_MAX <= lin )
                    {
                        lin = V_MAX;
                    }
                    else if(lin <= V_MIN)
                    {
                        lin = V_MIN;
                    }
                }
            }

            
        }

        // If bearing error is between 0  and -PI, rotate anticlockwise 
                else if((b < -fowangchange) && (b >= -M_PI))
        {
            

            // Prioritise Linear Velocity
            if(prio == false)
            {
                lin = GR;
                if(V_MAX< lin )
                {
                    lin = V_MAX;
                }
                else if(lin < V_MIN)
                {
                    lin = V_MIN;
                }

                // Calc Angular Velocity using Gamma Proportionality
                ang = (gam*lin);
                if(W_MAX< ang )
                {
                    ang = W_MAX;
                }
            }

            // Prioritise Angular Velocity
            else
            {
                ang = -b/2;
//this will cahnge depending

                if(ang > W_MAX)
                {
                    ang = W_MAX;
                }
                lin = std::abs(ang)/gam;
                if(lin > GRmultifowrad)
                {
                    lin = GRmultifowrad;
                    if(lin > V_MAX)
                    {
                        lin = V_MAX;
                    }
                    else if(lin < V_MIN)
                    {
                        lin = V_MIN;
                    }
                }
            }

            
        }
    }
    

        // If bearing is between 0 and PI, rot anit

	else if((b < -Revangchange) && (b >= -M_PI))
        {
            
            lin = GRmultibackward;
            if(lin< -V_MAX)
            {
                lin = -V_MAX;
            }
            else if(lin > -V_MIN)
            {
                lin = -V_MIN;
            }

            ang = -(lin*gam);
            if(ang> W_MAX)
            {
                ang = W_MAX;
            }
        

        // If bearing error is between 0 and -PI, rot clockwise 
       
        else if((Revangchange < b) && (b <= M_PI))
        {
            
            lin = GRmultibackward;
            if(lin < -V_MAX)
            {
                lin = -V_MAX;
            }
            else if(lin > -V_MIN)
            {
                lin = -V_MIN;
            }

            ang = lin*gam;
            if(ang < -W_MAX)
            {
                ang = -W_MAX;
            }
            
        }
    }

    
}

double FetchMovement::normaliseAngle(double theta)
{
    if((2 * M_PI) < theta)
    {
        thetavar = theta - (2 * M_PI);
		theata = thetavar;
    }
    else if(theta < 0)
    {
        thetavar = theta + (2 * M_PI);
		theata = thetavar;
    }
    return theta;
}

void FetchMovement::rpyFromPose(geometry_msgs::PoseStamped pose, double &fetchRoll, double &fetchPitch, double &fetchYaw)
{
    // Calc Roll, Pitch, Yaw of a given Pose
    // SOURCE: https://answers.ros.org/question/238083/how-to-get-yaw-from-quaternion-values/ 
     double z = pose.pose.orientation.z;
   double y = pose.pose.orientation.y;
  double x = pose.pose.orientation.x;
double w = pose.pose.orientation.w;
    
   
    
    // Create instance of Quaternion
    tf::Quaternion q(x, y, z, w);
    // Create 3x3 Rotation Matrix of Pose from Quaternion
    tf::Matrix3x3 m(q);
    // Extract Roll, Pitch and Yaw from 3x3 Rotation Matrix
    m.getRPY(fetchRoll, fetchPitch, fetchYaw);
}



