#ifndef sphere_h
#define sphere_h
#include "hitable.h"
class sphere: public hitable{
public:
	sphere(){}
	sphere(vec3 cen, float r, material *m): center(cen), radius(r), mat(m){}
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	vec3 center;
	float radius;
	material *mat;
};
#endif /* sphere_h */
