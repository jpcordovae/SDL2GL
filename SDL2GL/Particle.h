#pragma once
#include <armadillo.hpp>
#include <memory>

class Particle
{
public:
	typedef std::shared_ptr<Particle> particlePtr;
	Particle();
	Particle(double _m, arma::vec3 _position, arma::vec3 _velocity);
	virtual ~Particle();
private:
	double m; // mass
	arma::vec3 x; // position
	arma::vec3 v; // velocity
};

typedef Particle::particlePtr particlePtr;
