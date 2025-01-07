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


void Bow::trackArrow() {

	float angleRadians = angleDegrees * PI / 180;

	initialVelocity = velocity;

	float v_x = initialVelocity * std::cos(angleRadians);
	float v_y = initialVelocity * std::sin(angleRadians) - gravity * time;

	//std::cout << "INITVEL: " << initialVelocity << " GRAV: " << gravity << " TIME: " << time << std::endl;
	//std::cout << "VX: " << v_x << " VY: " << v_y << std::endl;

	arrowDest.x += v_x * timeStep;
	arrowDest.y += v_y * timeStep;
	


	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Time (s): " << time
		<< "\tDistance (m): " << arrowDest.x
		<< "\tHeight (m): " << arrowDest.y << "\n";
	


	time += timeStep;

}


void Bow::printResults() {
	std::cout << "Bow elastic potential: " << elasticPotential << " J\n";
	std::cout << "Arrow velocity: " << velocity << " m/s\n";
	std::cout << "Force required to pull the string: " << forceRequired << " N\n";
	std::cout << "The arrow travels: " << arrowDistance << " m\n";
}