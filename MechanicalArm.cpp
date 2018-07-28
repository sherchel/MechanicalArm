#include"stdafx.h"

#include<iostream>
#include<cstdlib>
#include<math.h>
using namespace std;

#define DOT_MAX 100
#define THETA_MAX 2000
#define AXIS_MAX 100

typedef struct RobotArm
{
	int LengthA, LengthB;				//LengthA=BigArm  LengthB=SmallArm
	int theta_s1, theta_s2;				//Gap angle:s1=start  s2=end
	int GAP;
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

RobotArm arm;
TargetPoint dot[DOT_MAX];
ThetaList theta1[THETA_MAX], theta2[THETA_MAX];
TurnTheta turn[AXIS_MAX][AXIS_MAX];

int Dot_Num = 0;
double OpWidth[AXIS_MAX];			//Operation Width



void init()
{
	arm_init();
	theta_init();

	departure_evaluation();

	return;
}

void arm_init()
{
	arm.LengthA = 100;
	arm.LengthB = 70;
	arm.theta_s1 = 0;
	arm.theta_s2 = 0;
	arm.GAP = 50;
	arm.Rmin = sqrt(arm.LengthA*arm.LengthA + arm.LengthB*arm.LengthB - 2 * arm.LengthA*arm.LengthB*cos(arm.theta_s1 / 180));
	arm.Rmax = sqrt(arm.LengthA*arm.LengthA + arm.LengthB*arm.LengthB - 2 * arm.LengthA*arm.LengthB*cos((180 - arm.theta_s2) / 180));

	for (int x = 0; x < AXIS_MAX; x++)
		OpWidth[x] = sqrt(arm.Rmax*arm.Rmax - (x + arm.Rmin)*(x + arm.Rmin));

	return;
}

//Preset Theta Calculation of possible Dot Position
void theta_init()
{
	double r;
	int rou=0;

	//Triangle's Interior Angle of possible Arm's Triangle Form
	for (int i = 0; i <= 1700; i++) {
		r = i / 10;
		theta1[i].r = r;
		theta1[i].theta = acos((arm.LengthA*arm.LengthA + r*r - arm.LengthB*arm.LengthB) / (2 * arm.LengthA*r));
		theta2[i].r = r;
		theta2[i].theta = acos((arm.LengthA*arm.LengthA + arm.LengthB + arm.LengthB - r*r) / (2 * arm.LengthA*arm.LengthB));
	}

	//Arm's Rotation Angle at Present Point
	for (int x = 0; x < AXIS_MAX; x++)
		for (int y = 0; y < AXIS_MAX; y++) {
			turn[x][y].r = sqrt((x + arm.Rmin)*(x + arm.Rmin) / y);
			if (y > 0) {
				rou = (int)(turn[x][y].r * 10);
				turn[x][y].theta_a = 90 - theta1[rou].theta - atan(y / (x + arm.Rmin));
				turn[x][y].theta_b = theta2[rou].theta - turn[x][y].theta_a;
			}
			else {
				turn[x][y].theta_b = -90 + theta1[rou].theta + theta2[rou].theta - atan(-y / (x + arm.Rmin));
				turn[x][y].theta_a = theta1[rou].theta + theta2[rou].theta - turn[x][y].theta_b;
			}
		}

	return;
}

//Departure Solution Evaluation of Object Release
void departure_evaluation()
{
	int min;
	
	for (int x = 0; x < AXIS_MAX; x++)
		for (int y = 0; y < AXIS_MAX; y++) {
			min = 1000;
			for (int k = 0; k < AXIS_MAX; k++)

		}

}

//Dot Data Read In
void read_in()
{
	cout << "Dot_Num=";
	cin >>Dot_Num;

	cout << endl << "camera_x   camera_y";

	for (int i = 0; i < Dot_Num; i++) {
		cin >> dot[i].camera_x >> dot[i].camera_y;

		dot[i].x = dot[i].camera_x;
		dot[i].y = dot[i].camera_y + arm.GAP;

		dot[i].OpWidth = 2 * OpWidth[dot[i].x];
		dot[i].ArWidth = arm.GAP - OpWidth[dot[i].x] + dot[i].camera_y;
	}

	return;
}

int main()
{
	init();
	read_in();

	return 0;
}