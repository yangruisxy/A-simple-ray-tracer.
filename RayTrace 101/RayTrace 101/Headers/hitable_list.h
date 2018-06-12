#ifndef hitable_list_h
#define hitable_list_h
#include "hitable.h"
class hitable_list: public hitable{
public:
	hitable_list(){}
	hitable_list(hitable **l, int n){list = l; size = n;}
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	hitable **list;
	int size;
};

#endif /* hitable_list_h */
