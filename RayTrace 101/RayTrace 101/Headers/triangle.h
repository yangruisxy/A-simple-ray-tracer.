#ifndef triangle_h
#define triangle_h
#include "hitable.h"
#include "float.h"
class triangle: public hitable{
public:
	triangle(const vec3 &a, const vec3 &b, const vec3 &c, material *mat): material(mat), v0(a), v1(b), v2(c){
		normal = cross(v1 - v0, v2 - v0);
		normal = unit_vector(normal);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &res) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	vec3 v0, v1, v2, normal;
	material *material;
};



#endif /* triangle_h */
