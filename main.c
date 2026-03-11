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
#include "rt.h"

#define WIDTH 800
#define HEIGHT 600

t_vec3	ray_at(t_ray ray, double t)
{
	return (vec_add(ray.origin, scalar_mult(ray.dir, t)));
}

/*convert normal to rgb*/
int	normal_to_color(t_vec3 n)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((n.x + 1.0) * 0.5) * 255);
	g = (int)(((n.y + 1.0) * 0.5) * 255);
	b = (int)(((n.z + 1.0) * 0.5) * 255);
	return (r << 16 | g << 8 | b);
}

double	hit_sphere(t_sphere s, t_ray r)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	disc;
	double	sqrt_disc;
	double	t1;
	double	t2;

	oc = vec_sub(r.origin, s.center);
	a = vec_dot(r.dir, r.dir);
	b = 2.0 * vec_dot(oc, r.dir);
	c = vec_dot(oc, oc) - s.radius * s.radius;
	disc = b * b - 4 * a * c;
	if (disc < 0)
		return (-1.0);
	sqrt_disc = sqrt(disc);
	t1 = (-b - sqrt_disc) / (2.0 * a);
	t2 = (-b + sqrt_disc) / (2.0 * a);
	if (t1 > 0 && t2 > 0)
		return ((t1 < t2) ? t1 : t2);
	else if (t1 > 0)
		return (t1);
	else if (t2 > 0)
		return (t2);
	return (-1.0);
}

int	main(void)
{
	void	*mlx;
	void	*win;
	void	*img;
	int		bpp;
	int		size_line;
	int		endian;
	int		*data;
	t_sphere	sphere;
	t_vec3	camera;
	int		x;
	int		y;
	double	u;
	double	v;
	t_ray	ray;
	double	t;
	int		color;
	t_vec3	point;
	t_vec3	normal;

	mlx = mlx_init();
	win = mlx_new_window(mlx, WIDTH, HEIGHT, "Ray Tracer");
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	data = (int *)mlx_get_data_addr(img, &bpp, &size_line, &endian);
	sphere = (t_sphere){{0, 0, -5}, 1.0};
	camera = (t_vec3){0, 0, 0};
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			u = (x - WIDTH / 2.0) / (WIDTH / 2.0);
			v = (HEIGHT / 2.0 - y) / (HEIGHT / 2.0);
			ray = (t_ray){camera, vec_normalize((t_vec3){u, v, -1.0})};
			t = hit_sphere(sphere, ray);
			if (t > 0)
			{
				point = ray_at(ray, t);
				normal = vec_normalize(vec_sub(point, sphere.center));
				color = normal_to_color(normal);
			}
			else
				color = 0x000000;
			data[y * (size_line / 4) + x] = color;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx, win, img, 0, 0);
	mlx_loop(mlx);
	return (0);
}
