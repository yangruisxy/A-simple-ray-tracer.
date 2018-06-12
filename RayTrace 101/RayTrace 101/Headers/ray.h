#ifndef ray_h
#define ray_h
#include "vec3.h"
class ray{
public:
	ray(){}
	ray(const vec3 &ori, const vec3 &dir, float ti = 0.0){
		o = ori;
		d = dir;
		_time = ti;
	}
	vec3 origin() const {return o;}
	vec3 direction() const {return d;}
	float time() const{return _time;}
	vec3 point_at_parameter(float t) const{return o + t * d;}
	
	vec3 o;
	vec3 d;
	float _time;
};

#endif
