#include "raylib.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>


class Bow
{

public:
	Bow();
	~Bow();

	const float gravity = 9.8f;
	float stiffness{ 165 };
	float displacement{ };
	const float maxDisplacement{ 1.5f };
	float mass{ .02 };
	float angleDegrees{ 45 };
	const float timeStep = .2f;

	float velocity{};
	float elasticPotential{};
	float forceRequired{};
	float arrowDistance{};
	float time = 0.0f;

	float initialVelocity{};
	float angle{};
	float horizontal{};
	float vertical{};

	Vector3 arrowDest{};
	Vector3 bowDest{};


	float bowElasticPotential() {return elasticPotential = 0.5f * (stiffness * std::pow(displacement, 2));}
	float arrowVelocity() { return velocity = std::sqrt((stiffness * std::pow(displacement, 2)) / mass); }
	float stringForceRequired() { return forceRequired = stiffness * displacement; }
	float calculateRange();
	float calculateTotalDistance() { return sqrt(horizontal * horizontal + vertical * vertical); }
	void trackArrow(float yawRadians);
	void printResults();


private:


};

