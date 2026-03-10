/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:21:08 by daflynn           #+#    #+#             */
/*   Updated: 2026/03/10 19:26:01 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <mlx.h>

#define WIDTH 800
#define HEIGHT 600


typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 dir;
}	t_ray;

typedef struct s_sphere
{
	t_vec3 center;
	double radius;	
} t_sphere;

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

t_vec3	scalar_mult(t_vec3 v, double d)
{
	t_vec3	result;

	result.x = v.x * d;
	result.y = v.y * d;
	result.z = v.z * d;

	return (result);
}

double	vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}


t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

double vec_length(t_vec3 v)
{
	return (sqrt(vec_dot(v, v)));
}

t_vec3 vec_normalize(t_vec3 v)
{
	double	len;

	len = vec_length(v);
	if (len == 0)
		return ((t_vec3){0, 0, 0});
	return (scalar_mult(v, 1.0 / len));
}

t_vec3	ray_at(t_ray ray, double t)
{
	return (vec_add(ray.origin, scalar_mult(ray.dir, t)));
}

/*convert normal to rgb*/
int normal_to_color(t_vec3 n)
{
	int r = (int)(((n.x + 1.0) * 0.5) * 255);
	int g = (int)(((n.y + 1.0) * 0.5) * 255);
	int b = (int)(((n.z + 1.0) * 0.5) * 255);
	return (r << 16 | g << 8 | b);

}

double hit_sphere(t_sphere s, t_ray r)
{
    t_vec3 oc = vec_sub(r.origin, s.center);
    double a = vec_dot(r.dir, r.dir);
    double b = 2.0 * vec_dot(oc, r.dir);
    double c = vec_dot(oc, oc) - s.radius * s.radius;
    double disc = b * b - 4 * a * c;
    
    if (disc < 0)
        return -1.0;
    
    double sqrt_disc = sqrt(disc);
    double t1 = (-b - sqrt_disc) / (2.0 * a);
    double t2 = (-b + sqrt_disc) / (2.0 * a);
    
    // Find the smallest positive t
    if (t1 > 0 && t2 > 0)
        return (t1 < t2) ? t1 : t2;
    else if (t1 > 0)
        return t1;
    else if (t2 > 0)
        return t2;
    else
        return -1.0;
}


int main(void)
{
    void *mlx = mlx_init();
    void *win = mlx_new_window(mlx, WIDTH, HEIGHT, "Ray Tracer");
    void *img = mlx_new_image(mlx, WIDTH, HEIGHT);
   int bpp, size_line, endian;
int *data = (int *)mlx_get_data_addr(img, &bpp, &size_line, &endian);
    t_sphere sphere = { {0, 0, -5}, 1.0 };
    t_vec3 camera = {0, 0, 0};

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            double u = (x - WIDTH / 2.0) / (WIDTH / 2.0);
            double v = (HEIGHT / 2.0 - y) / (HEIGHT / 2.0);

            t_ray ray = { camera, vec_normalize((t_vec3){u, v, -1.0}) };
            double t = hit_sphere(sphere, ray);
            int color;

            if (t > 0)
            {
                t_vec3 point = ray_at(ray, t);
                t_vec3 normal = vec_normalize(vec_sub(point, sphere.center));
                color = normal_to_color(normal);
            }
            else
                color = 0x000000;

            data[y * (size_line / 4) + x] = color;
        }
    }

    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    return 0;
}