#ifndef scale_h
#define scale_h
#include "hitable.h"
class scale: public hitable{
public:
	scale(hitable *p, float sx, float sy, float sz);
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const{
		box = bbox;
		return hasbox;
	}
	hitable *ptr;
	float xScale, yScale, zScale;
	bool hasbox;
	aabb bbox;
};

scale::scale(hitable *p, float sx, float sy, float sz): ptr(p){
	xScale = sx;
	yScale = sy;
	zScale = sz;
	hasbox = ptr->bounding_box(0, 1, bbox);
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++){
			for(int k = 0; k < 2; k++){
				float x = xScale * (i * bbox.max().x() + (1 - i) * bbox.min().x());
				float y = yScale * (j * bbox.max().y() + (1 - j) * bbox.min().y());
				float z = zScale * (k * bbox.max().z() + (1 - k) * bbox.min().z());
				vec3 tester(x, y, z);
				for(int c = 0; c < 3; c++){
					if(tester[c] > max[c])
						max[c] = tester[c];
					if(tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox = aabb(min,max);
}

bool scale::hit(const ray &r, float tmin, float tmax, hit_record &rec) const{
	float invX = 1.0 / xScale;
	float invY = 1.0 / yScale;
	float invZ = 1.0 / zScale;
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = r.origin()[0] * invX;
	origin[1] = r.origin()[1] * invY;
	origin[2] = r.origin()[2] * invZ;
	direction[0] = r.direction()[0] * invX;
	direction[1] = r.direction()[1] * invY;
	direction[2] = r.direction()[2] * invZ;
	ray scaled_ray(origin, direction, r.time());
	if(ptr->hit(scaled_ray, tmin, tmax, rec)){
		vec3 p = rec.p;
		vec3 normal = rec.normal;
		p[0] = rec.p[0] * xScale;
		p[1] = rec.p[1] * yScale;
		p[2] = rec.p[2] * zScale;
		normal[0] = rec.normal[0] * invX;
		normal[1] = rec.normal[1] * invY;
		normal[2] = rec.normal[2] * invZ;
		normal = unit_vector(normal);
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else
		return false;
}

#endif /* scale_h */
