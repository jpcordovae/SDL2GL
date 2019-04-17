#include "Particle.h"



Particle::Particle()
{
}

Particle::Particle(double _m, arma::vec3 _position, arma::vec3 _velocity) :m(_m), x(_position), v(_velocity)
{
		
}

Particle::~Particle()
{
}
