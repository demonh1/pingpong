#ifndef ball_h
#define ball_h

#include <cmath>
#include <cstdlib>

typedef struct 
{
	float ballX;
	float ballY;
	float ballDx; 
	float ballDy;
	float ballDirection;
	float ballAngle;
	float ballStepx;
	float ballStepy;
	float ballDelta;
} ball_t;

class Ball {
private:
	ball_t ball;

	static const int width;
	static const int height;
	static const float PI;
	// ctr
	Ball() {}
	Ball(const Ball&) = delete;
	// operator=
	Ball& operator=(const Ball&) = delete;

protected:
	int random(int i);

	void Run(ball_t* ball) 
	{
		ball->ballDelta = 0;
		ball->ballX = width/2;
		ball->ballY = height/2;
		ball->ballDx = width/40.96; //25
		ball->ballDy = height/30.72; //25

		ball->ballDirection = random(7);
		ball->ballAngle = random(360)*(PI/180);
		if (ball->ballAngle == 0) ball->ballAngle = 1;


		ball->ballStepx = ball->ballDirection*cos(ball->ballAngle);
		ball->ballStepy = ball->ballDirection*sin(ball->ballAngle);
	}
public:
	static Ball* Instance() 
	{
		static Ball ball;
		return &ball;
	}
	void StartBall() { Run(&ball); }
	ball_t* getBallPos() { return &ball; }

};

#endif
