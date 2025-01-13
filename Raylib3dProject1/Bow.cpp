#include "Bow.h"


Bow::Bow() {


}


Bow::~Bow() {

}


float Bow::calculateRange() {
	float angleRadians = angleDegrees * PI / 180;

	float range = (std::pow(velocity, 2) * std::sin(2 * angleRadians)) / gravity;

	return arrowDistance = range;
}


void Bow::trackArrow(float yawRadians) {

	float angleRadians = angleDegrees * PI / 180.f;

	initialVelocity = velocity;

	v_x = initialVelocity * cos(angleRadians) * sin(yawRadians); // X uses cost * sin
	v_z = initialVelocity * cos(angleRadians) * cos(yawRadians); // Z uses cos * cos
	v_y = initialVelocity * sin(angleRadians) - (0.5f / gravity * time * time); // y uses sin

	arrowDest.x += v_x * timeStep;
	arrowDest.y += v_y * timeStep;
	arrowDest.z += v_z * timeStep;

	
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Time (s): " << time
		<< "\tX Dist: " << arrowDest.x
		<< "\tY Dist: " << arrowDest.y
		<< "\tZ Dist: " << arrowDest.z << "\n";
	

	time += timeStep;

}


void Bow::printResults() {
	std::cout << "Bow elastic potential: " << elasticPotential << " J\n";
	std::cout << "Arrow velocity: " << velocity << " m/s\n";
	std::cout << "Force required to pull the string: " << forceRequired << " N\n";
	std::cout << "The arrow travels: " << arrowDistance << " m\n";

}