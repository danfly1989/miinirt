#ifndef RT_H
# define RT_H

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
} t_vec3;

typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 dir;
} t_ray;

typedef struct s_sphere
{
	t_vec3 center;
	double radius;
} t_sphere;

t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	scalar_mult(t_vec3 v, double d);
double	vec_dot(t_vec3 a, t_vec3 b);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
double	vec_length(t_vec3 v);
t_vec3	vec_normalize(t_vec3 v);

#endif
