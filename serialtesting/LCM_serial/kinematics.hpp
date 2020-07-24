#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <cmath>

#define SIN120 0.86603f
#define SIN240 -SIN120
#define COS120 -0.5f
#define COS240 COS120

#define ROOTTHREE 1.73205f

class Kinematics {
	public:

	struct KiwiVels {float va; float vb; float vc;};
	struct CartesianVels {float vx; float vy; float wz;};

	Kinematics(float wheelRad_meters, float botRad_meters) : 
		wheelRad_meters_(wheelRad_meters), 
		botRad_meters_(botRad_meters) {}

	KiwiVels forwardKinematicsLocal(float vx, float vy, float wz) {
		float va = (1/wheelRad_meters_)*(-vy - botRad_meters_*wz);
		float vb = (1/wheelRad_meters_)*(SIN120*vx - COS120*vy - botRad_meters_*wz);
		float vc = (1/wheelRad_meters_)*(SIN240*vx - COS240*vy - botRad_meters_*wz);
		return {va, vb, vc};
	}

	CartesianVels inverseKinematicsLocal(float va, float vb, float vc) {
		float vx = wheelRad_meters_*(1/ROOTTHREE)*(vb - vc);
		float vy = wheelRad_meters_*((-2/3)*va + (1/3)*(vb + vc));
		float wz = wheelRad_meters_*(1/(3*botRad_meters_))*(-va - vb - vc);
	}

	private:
		lcm::LCM lcmInstance_;
		float wheelRad_meters_;
		float botRad_meters_;
};