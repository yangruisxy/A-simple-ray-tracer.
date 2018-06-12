#include "triangle.h"
bool triangle::hit(const ray &r, float tmin, float tmax, hit_record &rec) const{
	float b[3];
	float t;
	const vec3 &e1 = v1 - v0;
	const vec3 &e2 = v2 - v0;
	const vec3 &q = cross(unit_vector(r.direction()), e2);
	const float a = dot(e1, q);
	//std::cout << a << std::endl;
	if((dot(normal, r.direction())>= 0) || (abs(a) <= 0))
		return false;
	
	const vec3 &s = (r.origin() - v0) / a;
	//std::cout << s << std::endl;
	const vec3 &w = cross(s, e1);
	//std::cout << w << std::endl;
	b[0] = dot(s, q);
	b[1] = dot(w, unit_vector(r.direction()));
	b[2] = 1.0f - b[0] - b[1];
	if((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f))
		return false;
	t = dot(e2, w);
	if(t >= 0.0f){
		rec.t = t;
		rec.p = r.point_at_parameter(rec.t);
		rec.normal = normal;
		rec.m_ptr = material;
		return true;
	}
	else{
		return false;
	}
}

bool triangle::bounding_box(float t0, float t1, aabb &box) const{
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for(int c = 0; c < 3; c++){
		min[c] = std::min(std::min(v0[c], v1[c]), v2[c]);
		max[c] = std::max(std::max(v0[c], v1[c]), v2[c]);
	}
	box = aabb(min, max);
	return true;
}

