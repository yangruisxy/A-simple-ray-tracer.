#include "sphere.h"
bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if(discriminant > 0){
		float t = (-b - sqrt(b*b-a*c)) / a;
		if(t < t_max && t > t_min){
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.m_ptr = mat;
			return true;
		}
		t = (-b + sqrt(b*b-a*c)) / a;
		if(t < t_max && t > t_min){
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			get_sphere_uv((rec.p - center) / radius , rec.u, rec.v);
			rec.m_ptr = mat;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(float t0,  float t1, aabb &box) const{
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}
