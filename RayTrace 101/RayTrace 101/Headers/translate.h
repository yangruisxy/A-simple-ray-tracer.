#ifndef translate_h
#define translate_h
#include "hitable.h"
class translate : public hitable{
public:
	translate(hitable *p, const vec3 &displacement): ptr(p), offset(displacement){}
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	hitable *ptr;
	vec3 offset;
};

bool translate::hit(const ray &r, float tmin, float tmax, hit_record &rec) const{
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if(ptr->hit(moved_r, tmin, tmax, rec)){
		rec.p += offset;
		return true;
	}
	else
		return false;
}

bool translate::bounding_box(float t0, float t1, aabb &box) const{
	if(ptr->bounding_box(t0, t1, box)){
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	else
		return false;
}


#endif /* translate_h */
