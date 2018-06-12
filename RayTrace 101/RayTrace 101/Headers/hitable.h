#ifndef hitable_h
#define hitable_h
#include "ray.h"
#include "aabb.h"

static void get_sphere_uv(const vec3 &p, float &u, float &v){
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / ( 2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}

class material;
struct hit_record{
	float t;
	vec3 normal;
	vec3 p;
	material *m_ptr;
	float u;
	float v;
};

class hitable{
public:
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb &box) const = 0;
};
#endif /* hitable_h */
