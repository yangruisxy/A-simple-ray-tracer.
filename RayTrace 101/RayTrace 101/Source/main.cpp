#include <iostream>
#include <fstream>
#include "sphere.h"
#include "aabb.h"
#include "bvh.h"
#include "movingsphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "rect.h"
#include "image_texture.h"
#include "flipnormal.h"
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"
#include "box.h"
#include "translate.h"
#include "rotation.h"
#include "scale.h"
#include "constant_medium.h"
#include <float.h>
#include "triangle.h"
vec3 color(const ray &r, hitable *world, int depth){
	hit_record rec;
	if(world->hit(r, 0.001, MAXFLOAT, rec)){
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.m_ptr->emitted(rec.u, rec.v, rec.p);
		if(depth < 50 && rec.m_ptr->scatter(r, rec, attenuation, scattered)){
			return emitted + attenuation * color(scattered, world, depth + 1);
		}
		else{
			return emitted;
		}
		
	}
	else{
		return vec3(0, 0, 0);
	}
}

hitable *randomScene(){
	int n = 500;
	hitable** list = new hitable*[n+1];
	texture *checker = new checker_texture(new constant_texture(vec3(0, 0, 0)), new constant_texture(vec3(1, 1, 1)));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lamebertian(checker));
	int i = 1;
	for(int a = -10; a < 10; a++){
		for(int b = -10; b < 10; b++){
			float choose_mat = drand48();
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
			if((center - vec3(4.0, 0, 2.0)).length() > 0.9){
				if(choose_mat < 0.8){
					list[i++] = new movingsphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2,
												 new lamebertian(new constant_texture(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48()))));
				}
				else if(choose_mat < 0.95){
					list[i++] = new sphere(center, 0.2, new mental(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
				}else{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lamebertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new mental(vec3(0.7, 0.6, 0.5), 0.0));
	//return new hitable_list(list, i);
	return new bvh_node(list, i, 0.0, 1.0);
}

hitable *two_spheres(){
	texture *pertext = new noise_texture(10.0f);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lamebertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lamebertian(pertext));
	return new hitable_list(list, 2);
}

hitable *earth(){
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, 0, 0), 2, new lamebertian(new image_texture(tex_data, nx, ny)));
	return new hitable_list(list, 1);
}

hitable *simple_light(){
	texture *pertext = new noise_texture(4);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lamebertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lamebertian(pertext));
	//list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	//list[2] = new xy_rect(3, 5, 1, 3, 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[2] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 3);
}

hitable *cornell_box(){
	hitable **list = new hitable*[8];
	int i = 0;
	material *red =new lamebertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lamebertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lamebertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flipnormal(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flipnormal(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flipnormal(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *cornell_box_scale(){
	hitable **list = new hitable*[8];
	int i = 0;
	material *red =new lamebertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lamebertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lamebertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flipnormal(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flipnormal(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flipnormal(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new scale(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), 1, 1.5, 1), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new scale(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 1.5, 1, 1), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *cornell_box_with_fog(){
	hitable **list = new hitable*[8];
	int i = 0;
	material *red =new lamebertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lamebertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lamebertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[i++] = new flipnormal(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flipnormal(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flipnormal(new xy_rect(0, 555, 0, 555, 555, white));
	hitable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	hitable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
	return new hitable_list(list, i);
}

hitable *cornell_balls(){
	hitable **list = new hitable*[9];
	int i = 0;
	material *red = new lamebertian( new constant_texture(vec3(0.65, 0.05, 0.05)) );
	material *white = new lamebertian( new constant_texture(vec3(0.73, 0.73, 0.73)) );
	material *green = new lamebertian( new constant_texture(vec3(0.12, 0.45, 0.15)) );
	material *light = new diffuse_light( new constant_texture(vec3(5, 5, 5)) );
	list[i++] = new flipnormal(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flipnormal(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flipnormal(new xy_rect(0, 555, 0, 555, 555, white));
	hitable *boundary = new sphere(vec3(160, 100, 145), 100, new dielectric(1.5));
	list[i++] = boundary;
	list[i++] = new constant_medium(boundary, 0.1, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *cornell_final(){
	hitable **list = new hitable*[30];
	hitable **boxlist = new hitable*[10000];
	texture *pertext = new noise_texture(0.1);
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat =  new lamebertian(new image_texture(tex_data, nx, ny));
	int i = 0;
	material *red = new lamebertian( new constant_texture(vec3(0.65, 0.05, 0.05)) );
	material *white = new lamebertian( new constant_texture(vec3(0.73, 0.73, 0.73)) );
	material *green = new lamebertian( new constant_texture(vec3(0.12, 0.45, 0.15)) );
	material *light = new diffuse_light( new constant_texture(vec3(7, 7, 7)) );
	list[i++] = new flipnormal(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(123, 423, 147, 412, 554, light);
	list[i++] = new flipnormal(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flipnormal(new xy_rect(0, 555, 0, 555, 555, white));
	hitable *boundary = new sphere(vec3(160, 50, 145), 50, mat);
	list[i++] = boundary;
	hitable *boundary2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), new dielectric(1.5)), -18), vec3(130, 0, 65));
	list[i++] = boundary2;
	list[i++] = new constant_medium(boundary2, 0.5, new constant_texture(vec3(0.5, 0.7, 0.1)));
	return new hitable_list(list, i);
}

hitable *final(){
	int nb = 20;
	hitable **list = new hitable*[30];
	hitable **boxlist = new hitable*[10000];
	hitable **boxlist2 = new hitable*[10000];
	material *white = new lamebertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *ground = new lamebertian(new constant_texture(vec3(0.6, 0.4, 0.68)));
	int b = 0;
	for(int i = 0; i < nb; i++){
		for(int j = 0; j < nb; j++){
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float z1 = z0 + w;
			float y1 = 100 * (drand48() + 0.01);
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	material *light =  new diffuse_light( new constant_texture(vec3(7, 7, 7)));
	list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
	vec3 center(400, 400, 200);
	list[l++] = new movingsphere(center, center+vec3(30, 0, 0), 0, 1, 50, new lamebertian(new constant_texture(vec3(0.5, 0.7, 0.9))));
	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
	list[l++] = new sphere(vec3(0, 150, 145), 50, new mental(vec3(0.4, 0.6, 0.8), 10.0));
	hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.5, new constant_texture(vec3(0.2, 0.4, 0.9)));
	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
	list[l++] = new constant_medium(boundary, 0.001, new constant_texture(vec3(1.0, 1.0, 1.0)));
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lamebertian(new image_texture(tex_data, nx, ny));
	list[l++] = new sphere(vec3(400, 200, 400), 100, mat);
	texture *pertext = new noise_texture(0.1);
	list[l++] = new sphere(vec3(220, 280, 300), 80, new lamebertian(pertext));
	int ns = 1000;
	for(int j = 0; j < ns; j++){
		boxlist2[j] = new sphere(vec3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);
	}
	list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
	return new hitable_list(list, l);
}

hitable *simpleTriangle(){
	hitable **list= new hitable*[1];
	list[0] = new triangle(vec3(0, 0, 0), vec3(5, 0, 0), vec3(0, 5	, 0), new diffuse_light(new constant_texture(vec3(0.5, 0.7, 0.8))));
	return new hitable_list(list, 1);
}

int main()
{
	int nx = 800;
	int ny = 800;
	int ns = 100;
	std::ofstream oFile;
	oFile.open("final.ppm",std::ios::binary);
	oFile << "P3" << "\n" << nx << " " << ny << "\n255\n";
	//vec3 lookfrom(278, 278, -800);
	//vec3 lookat(278, 278, 0);
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 40, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
	//hitable *world = randomScene();
	//hitable *world = two_spheres();
	//hitable *world = earth();
	//hitable *world = simple_light();
	//hitable *world = cornell_box();
	//hitable *world = cornell_box_scale();
	//hitable *world = cornell_box_with_fog();
	//hitable *world = cornell_final();
	//hitable *world = cornell_balls();
	//hitable *world = final();
	hitable *world = simpleTriangle();
	for(int j = ny - 1; j >= 0; j--){
		for(int i = 0;  i < nx; i++){
			vec3 col(0, 0, 0);
				for(int s = 0; s < ns; s++){
					float u = float(i + drand48()) / float(nx);
					float v = float(j + drand48()) / float(ny);
					ray r = cam.get_ray(u,v);
					col += color(r, world, 0);
				}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());
			oFile << ir << " " << ig << " " << ib << " " << "\n";
		}
	}
}
