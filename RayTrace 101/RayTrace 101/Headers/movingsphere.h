#ifndef movingsphere_h
#define movingsphere_h
#include "hitable.h"
class movingsphere : public hitable{
public:
	movingsphere(){}
	movingsphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m):center0(cen0),
	center1(cen1), time0(t0), time1(t1), radius(r), m_ptr(m){}
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	vec3 center(float time) const;
	vec3 center0, center1;
	float time0, time1;
	float radius;
	material *m_ptr;
};

vec3 movingsphere::center(float time) const{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool movingsphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if(discriminant > 0)
	{
		float t = (-b - sqrt(discriminant)) / a;
		if(t < t_max && t > t_min){
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
			rec.m_ptr = m_ptr;
			return true;
		}
		t = (-b + sqrt(discriminant)) / a;
		if(t < t_max && t > t_min){
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
			rec.m_ptr = m_ptr;
			return true;
		}
	}
	return false;
}

bool movingsphere::bounding_box(float t0, float t1, aabb &box) const{
	aabb box0 = aabb(center(time0) - vec3(radius, radius, radius), center(time0) + vec3(radius, radius, radius));
	aabb box1 = aabb(center(time1) - vec3(radius, radius, radius), center(time1) + vec3(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}
#endif /* movingsphere_h */
