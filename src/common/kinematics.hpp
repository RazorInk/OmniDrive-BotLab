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

	/**
	* @brief  Maps from world space (i.e., ground velocity and rotation) to "kiwi"
	*         space (i.e., three wheel tangential vel in m/s) 
	* @param  {vx, vy, wz}: robot x/y ground velocity in m/s and robot rotation in
	*         rad/s
	* @retval {va, vb, vc}: wheel tangential velocities in m/s
	* @note   Different from Chin-Wei's dynamics doc, which uses wheel rotation
  *         in rad/s.
	* @note   Can be used for distances instead of velocities, since all terms
	*         have '/s' on their units, which can be dropped.
	*/
	KiwiVels forwardKinematicsLocal(float vx, float vy, float wz);

	/**
	* @brief  Maps from "kiwi" space (i.e., three wheel tangential vel in m/s) to 
	*         world space (i.e., ground velocity and rotation)
	* @param  va, vb, vc: wheel tangential velocities in m/s
	* @retval {vx, vy, wz}: robot x/y ground velocity in m/s and robot rotation in
	*         rad/s
	* @note   Different from Chin-Wei's dynamics doc, which uses wheel rotation
  *         in rad/s.
	* @note   Can be used for distances instead of velocities, since all terms
	*         have '/s' on their units, which can be dropped.
	*/
	CartesianVels inverseKinematicsLocal(float &va, float &vb, float &vc);

	private:
		lcm::LCM lcmInstance_;
		float wheelRad_meters_;
		float botRad_meters_;
};