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

	float v_x = initialVelocity * cos(angleRadians) * sin(yawRadians); // X uses sin
	float v_z = initialVelocity * cos(angleRadians) * cos(yawRadians); // Z uses cos
	float v_y = initialVelocity * sin(angleRadians) - gravity * time;

	//std::cout << "INITVEL: " << initialVelocity << " GRAV: " << gravity << " TIME: " << time << std::endl;
	//std::cout << "VX: " << v_x << " VY: " << v_y << std::endl;
	float dirZ = std::sin(yawRadians);

	arrowDest.x += v_x * timeStep;
	arrowDest.y += v_y * timeStep;
	arrowDest.z += v_z * timeStep;

	Vector3 rayStart = arrowDest;
	Vector3 rayDirection = {
		v_x,
		v_y,
		v_z
	};

	rayEnd = Vector3Add(rayStart, Vector3Scale(Vector3Normalize(rayDirection), 40.f));


	DrawLine3D(rayStart, rayEnd, RED);

	std::cout << "RAYSTART: " << rayStart.z << " RayEnd: " << rayEnd.z << std::endl;

	/*
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Time (s): " << time
		<< "\tX Dist: " << arrowDest.x
		<< "\tY Dist: " << arrowDest.y
		<< "\tZ Dist: " << arrowDest.z << "\n";
	*/


	time += timeStep;

}


void Bow::printResults() {
	std::cout << "Bow elastic potential: " << elasticPotential << " J\n";
	std::cout << "Arrow velocity: " << velocity << " m/s\n";
	std::cout << "Force required to pull the string: " << forceRequired << " N\n";
	std::cout << "The arrow travels: " << arrowDistance << " m\n";
}