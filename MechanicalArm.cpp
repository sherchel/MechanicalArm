#include"stdafx.h"

#include<iostream>
#include<cstdlib>
#include<math.h>
using namespace std;

#define DOT_MAX 100
#define THETA_MAX 2000
#define AXIS_MAX 170
#define PI 3.1415926535897

typedef struct RobotArm
{
	int LengthA, LengthB;				//LengthA=BigArm  LengthB=SmallArm
	int theta_s1, theta_s2;				//Gap angle:s1=start  s2=end
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

RobotArm arm;
TargetPoint dot[DOT_MAX];
ThetaList theta1[THETA_MAX], theta2[THETA_MAX];
TurnTheta turn[AXIS_MAX][AXIS_MAX];

int Dot_Num = 0;
double OpWidth[AXIS_MAX];			//Operation Width

void init();
void arm_init();
void theta_init();
void departure_evaluation();
void order_evaluation();
void read_in();
void test();


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
	arm.speed = -1;
	arm.Rmin = sqrt(arm.LengthA*arm.LengthA + arm.LengthB*arm.LengthB - 2 * arm.LengthA*arm.LengthB*cos(arm.theta_s1/180*PI));
	arm.Rmax = sqrt(arm.LengthA*arm.LengthA + arm.LengthB*arm.LengthB - 2 * arm.LengthA*arm.LengthB*cos((180 - arm.theta_s2)/180*PI));

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
	for (int i = arm.Rmin*10; i <arm.Rmax*10; i++) {
		r = i / 10;
		theta1[i].r = r;
		theta1[i].theta = acos((arm.LengthA*arm.LengthA + r*r - arm.LengthB*arm.LengthB) / (2 * arm.LengthA*r))/PI*180;
		theta2[i].r = r;
		theta2[i].theta = acos((arm.LengthA*arm.LengthA + arm.LengthB * arm.LengthB - r*r) / (2 * arm.LengthA*arm.LengthB))/PI*180;
	}

	//Arm's Rotation Angle at Present Point
	for (int x = 0; x < AXIS_MAX; x++)
		for (int y = 0; y < AXIS_MAX; y++) {

			turn[x][y].r = sqrt((x + arm.Rmin)*(x + arm.Rmin) + y*y);

			rou = (int)(turn[x][y].r * 10);
			if (rou<arm.Rmin * 10 || rou>arm.Rmax * 10)  break;

			if (y >= 0) {
				turn[x][y].theta_a = 90 - theta1[rou].theta - (atan(y / (x + arm.Rmin))/PI*180);
				turn[x][y].theta_b = theta2[rou].theta - turn[x][y].theta_a;
			}
			else {  //***
				turn[x][y].theta_b = -90 + theta1[rou].theta + theta2[rou].theta - (atan(-y / (x + arm.Rmin))/PI*180);
				turn[x][y].theta_a = theta1[rou].theta + theta2[rou].theta - turn[x][y].theta_b;
			}
			//cout << x << "  " << y << "  " << turn[x][y].theta_a << "  "<<turn[x][y].theta_b<<endl;
		}

	return;
}

//Departure Solution Evaluation of Object Release
void departure_evaluation()
{
	int min,sign;
	int now;
	
	for (int x = 0; x < AXIS_MAX; x++)
		for (int y = 0; y < AXIS_MAX; y++) {
			if (turn[x][y].r<arm.Rmin || turn[x][y].r>arm.Rmax)  break;

			min = 1000;
			sign = 0;
			for (int k = 0; k < AXIS_MAX; k++) {
				now = abs(turn[x][y].theta_a - turn[0][k].theta_a) + abs(turn[x][y].theta_b - turn[0][k].theta_b);
				if (now < min) { min = now; sign = k; }
			}
			turn[x][y].Departure = sign;
			//cout << x << " " << y << " " << sign<<" "<<min<<endl;
		}

	return;
}

//Order of Dot Clamping Evaluation
void order_evaluation()
{
	int tnow=0;
	//dot's displacement=tnow*arm.speed

	bool dotsign[DOT_MAX];
	for (int i = 0; i < Dot_Num; i++)
		dotsign[i] = false;


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

void test()
{
	for (int x = 0; x < AXIS_MAX; x++)
		for (int y = 0; y < AXIS_MAX; y++) {
			cout << x << "  " << y<<"  ";
			cout << turn[x][y].theta_a<<"  "<<turn[x][y].theta_b<< endl;

		}

	return;
}

int main()
{
	init();
	//read_in();

	//test();
	int aaa;
	cin >> aaa;

	return 0;
}