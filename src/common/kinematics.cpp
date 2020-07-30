#include <common/kinematics.hpp>

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
Kinematics::KiwiVels Kinematics::forwardKinematicsLocal(float vx, float vy, float wz) {
	float va = (-vy - botRad_meters_*wz);
	float vb = (SIN120*vx - COS120*vy - botRad_meters_*wz);
	float vc = (SIN240*vx - COS240*vy - botRad_meters_*wz);
	return {va, vb, vc};
}

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
Kinematics::CartesianVels Kinematics::inverseKinematicsLocal(float &va, float &vb, float &vc) {
	// std::cout << "ROOTTHREE = " << ROOTTHREE << std::endl;
	// std::cout << "wheelRad_meters_ = " << wheelRad_meters_ << std::endl;
	// std::cout << "va, vb, vc = " << va << ", " << vb << ", " << vc << std::endl;
	float vx = (1/ROOTTHREE)*(vb - vc);
	float vy = ((-2/3)*va + (1/3)*(vb + vc));
	float wz = (1/(3*botRad_meters_))*(-va - vb - vc);
	return {vx, vy, wz};
}