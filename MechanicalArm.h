//
//  MechanicalArm.hpp
//  M_Arm
//
//  Created by Arvin_JIN on 2018/10/13.
//  Copyright © 2018年 Arvin_JIN. All rights reserved.
//

#ifndef MechanicalArm_h
#define MechanicalArm_h

#define DOT_MAX 100
#define THETA_MAX 2000
#define AXIS_MAX 170
#define PI 3.1415926535897

typedef struct RobotArm
{
    int LengthA, LengthB;    //LengthA=BigArm  LengthB=SmallArm
    int theta_s1, theta_s2;    //Gap angle:s1=start  s2=end
    int GAP;
    int speed;
    double Rmin, Rmax;
    
} RobotArm;

typedef struct TargetPoint
{
    int x, y, r;
    int camera_x, camera_y;
    
    double OpWidth, ArWidth;   //Operation Width,Arrive Width
    
} TargetPoint;

typedef struct ThetaList
{
    double r;
    double theta;
} ThetaList;

typedef struct TurnTheta
{
    double theta_a;
    double theta_b;
    double r;
    
    int Departure;
    
} TurnTheta;
#endif /* MechanicalArm_hpp */

void init();

void arm_init(RobotArm &arm, double (&OpWidth)[AXIS_MAX]);

void theta_init(RobotArm &arm, ThetaList (&theta1)[THETA_MAX], ThetaList (&theta2)[THETA_MAX], TurnTheta (&turn)[AXIS_MAX][AXIS_MAX]);

void departure_evaluation(RobotArm &arm, TurnTheta (&turn)[AXIS_MAX][AXIS_MAX]);

void order_evaluation();

void read_in(RobotArm &arm, TargetPoint (&dot)[DOT_MAX], int Dot_Num, double (&OpWidth)[AXIS_MAX]);

void test(TurnTheta (&turn)[AXIS_MAX][AXIS_MAX]);

