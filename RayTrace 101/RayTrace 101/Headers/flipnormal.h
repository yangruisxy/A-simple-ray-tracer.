#ifndef flipnormal_h
#define flipnormal_h
#include "hitable.h"
class flipnormal: public hitable{
public:
	flipnormal(hitable *p) : ptr(p){}
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const{
		if(ptr -> hit(r, tmin, tmax, rec)){
			rec.normal = -rec.normal;
			return true;
		}
		return false;
	}
	virtual bool bounding_box(float t0, float t1, aabb &box) const{
		return ptr->bounding_box(t0, t1, box);
	}
	hitable *ptr;
};

#endif
