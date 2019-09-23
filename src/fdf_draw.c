/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkuhic <hkuhic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/11 17:46:27 by jaleman           #+#    #+#             */
/*   Updated: 2019/09/23 15:39:11 by hkuhic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	put_pixel(t_fdf *fdf, int y, int z, double uvector)
{
	int		pos;

	if ((y > 0 && z > 0) && (y < WIN_WIDTH && z < WIN_HEIGHT))
	{
		pos = (y * 4) + (z * WIN_WIDTH * 4);
		fdf->image.data[pos] = fdf->color.red + uvector;
		fdf->image.data[pos + 1] = fdf->color.green + uvector;
		fdf->image.data[pos + 2] = fdf->color.blue + uvector;
		fdf->image.data[pos + 3] = 0x7F + uvector;
	}
}

static void	draw_lines(t_fdf *fdf)
{
	double y;
	double z;
	double delta_y;
	double delta_z;
	double uvector;

	y = fdf->map.y0;
	z = fdf->map.z0;
	delta_y = fdf->map.y1 - fdf->map.y0;
	delta_z = fdf->map.z1 - fdf->map.z0;
	uvector = sqrt((pow(delta_y, 2)) + (pow(delta_z, 2)));
	delta_y /= uvector;
	delta_z /= uvector;
	while (uvector > 0)
	{
		put_pixel(fdf, y, z, uvector);
		y += delta_y;
		z += delta_z;
		uvector -= 1;
	}
}

static void	draw_horizontal(t_fdf *fdf, int y, int z)
{
	int		yt;
	int		zt;

	yt = y - fdf->map.width / 2;
	zt = z - fdf->map.height / 2;
	fdf->map.y0 = fdf->map.angle_y * (yt - zt) * fdf->map.zoom;
	fdf->map.z0 = fdf->map.angle_z * (yt + zt) * fdf->map.zoom;
	fdf->map.z0 -= fdf->map.values[z][y] * fdf->map.x_value;
	fdf->map.y1 = fdf->map.angle_y * ((yt + 1) - zt) * fdf->map.zoom;
	fdf->map.z1 = fdf->map.angle_z * ((yt + 1) + zt) * fdf->map.zoom;
	fdf->map.z1 -= fdf->map.values[z][y + 1] * fdf->map.x_value;
	fdf->map.y0 = fdf->map.y0 * cos(fdf->map.angle_rot_x) -
	fdf->map.z0 * sin(fdf->map.angle_rot_x);
	fdf->map.z0 = fdf->map.y0 * sin(fdf->map.angle_rot_x) +
	fdf->map.z0 * cos(fdf->map.angle_rot_x);
	fdf->map.y1 = fdf->map.y1 * cos(fdf->map.angle_rot_x) -
	fdf->map.z1 * sin(fdf->map.angle_rot_x);
	fdf->map.z1 = fdf->map.y1 * sin(fdf->map.angle_rot_x) +
	fdf->map.z1 * cos(fdf->map.angle_rot_x);
	fdf->map.y0 += (WIN_WIDTH / 2) + fdf->map.coordinate_y;
	fdf->map.y1 += (WIN_WIDTH / 2) + fdf->map.coordinate_y;
	fdf->map.z0 += (WIN_HEIGHT / 2) - fdf->map.coordinate_z;
	fdf->map.z1 += (WIN_HEIGHT / 2) - fdf->map.coordinate_z;
	draw_lines(fdf);
}

static void	draw_vertical(t_fdf *fdf, int y, int z)
{
	int		yt;
	int		zt;

	yt = y - fdf->map.width / 2;
	zt = z - fdf->map.height / 2;
	fdf->map.y0 = fdf->map.angle_y * (yt - zt) * fdf->map.zoom;
	fdf->map.z0 = fdf->map.angle_z * (yt + zt) * fdf->map.zoom;
	fdf->map.z0 -= fdf->map.values[z][y] * fdf->map.x_value;
	fdf->map.y1 = fdf->map.angle_y * (yt - (zt + 1)) * fdf->map.zoom;
	fdf->map.z1 = fdf->map.angle_z * (yt + (zt + 1)) * fdf->map.zoom;
	fdf->map.z1 -= fdf->map.values[z + 1][y] * fdf->map.x_value;
	fdf->map.y0 = fdf->map.y0 * cos(fdf->map.angle_rot_x) -
	fdf->map.z0 * sin(fdf->map.angle_rot_x);
	fdf->map.z0 = fdf->map.y0 * sin(fdf->map.angle_rot_x) +
	fdf->map.z0 * cos(fdf->map.angle_rot_x);
	fdf->map.y1 = fdf->map.y1 * cos(fdf->map.angle_rot_x) -
	fdf->map.z1 * sin(fdf->map.angle_rot_x);
	fdf->map.z1 = fdf->map.y1 * sin(fdf->map.angle_rot_x) +
	fdf->map.z1 * cos(fdf->map.angle_rot_x);
	fdf->map.y0 += (WIN_WIDTH / 2) + fdf->map.coordinate_y;
	fdf->map.y1 += (WIN_WIDTH / 2) + fdf->map.coordinate_y;
	fdf->map.z0 += (WIN_HEIGHT / 2) - fdf->map.coordinate_z;
	fdf->map.z1 += (WIN_HEIGHT / 2) - fdf->map.coordinate_z;
	draw_lines(fdf);
}

int			fdf_draw(t_fdf *fdf)
{
	int		y;
	int		z;

	z = 0;
	fdf->mlx.img = mlx_new_image(fdf->mlx.init, WIN_WIDTH, WIN_HEIGHT);
	fdf->image.data = mlx_get_data_addr(fdf->mlx.img, &fdf->image.bpp, \
		&fdf->image.size, &fdf->image.endian);
	while (fdf->map.height > z)
	{
		y = 0;
		while (fdf->map.width > y)
		{
			fdf->map.y0 = y;
			fdf->map.z0 = z;
			if (fdf->map.width > y + 1)
				draw_horizontal(fdf, y, z);
			if (fdf->map.height > z + 1)
				draw_vertical(fdf, y, z);
			y += 1;
		}
		z += 1;
	}
	mlx_put_image_to_window(fdf->mlx.init, fdf->mlx.win, fdf->mlx.img, 0, 0);
	mlx_destroy_image(fdf->mlx.init, fdf->mlx.img);
	return (0);
}
