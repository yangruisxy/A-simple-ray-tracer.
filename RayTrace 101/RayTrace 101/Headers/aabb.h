#ifndef aabb_h
#define aabb_h
#include "ray.h"
#include "hitable.h"
inline float ffmin(float a, float b){return a < b ? a : b;}
inline float ffmax(float a, float b){return a > b ? a : b;}
class aabb{
public:
	aabb() {}
	aabb(const vec3 &a, const vec3 &b){_min = a; _max = b;}
	vec3 min() const {return _min;}
	vec3 max() const {return _max;}
	
	inline bool hit(const ray &r, float t_min, float t_max) const{
		for(int a = 0; a < 3; ++a){
			float t0 = ffmin((_min[a] - r.origin()[a])/ r.direction()[a],
							 (_max[a] - r.origin()[a]) / r.direction()[a]);
			float t1 = ffmax((_min[a] - r.origin()[a])/ r.direction()[a],
							 (_max[a] - r.origin()[a]) / r.direction()[a]);
			t_min = ffmax(t0, t_min);
			t_max = ffmin(t1, t_max);
			if(t_max < t_min){
				return false;
			}
		}
		return true;
	}
	vec3 _min;
	vec3 _max;
};

inline aabb surrounding_box(aabb box0, aabb box1){
	vec3 small(ffmin(box0.min().x(), box1.min().x()), ffmin(box0.min().y(), box1.min().y()), ffmin(box0.min().z(), box1.min().z()));
	vec3 big(ffmax(box0.max().x(), box1.max().x()), ffmax(box0.max().y(), box1.max().y()), ffmax(box0.max().z(), box1.max().z()));
	return aabb(small, big);
}
#endif /* aabb_h */
