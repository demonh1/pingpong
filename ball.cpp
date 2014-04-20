#include "ball.h"
#include <random>
#include <functional>


const int Ball::width = 1024;
const int Ball::height = 768;
const float Ball::PI = 3.14126f;


int Ball::random(int i) {


std::uniform_int_distribution<int> distr(0, 99);
std::mt19937 engine; 
auto generator = std::bind(distr, engine);
return generator()%i+1;
}
