/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkuhic <hkuhic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 16:05:24 by jaleman           #+#    #+#             */
/*   Updated: 2019/09/23 16:07:44 by hkuhic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	fdf_usage(char *argv0)
{
	ft_putstr("Usage:\n\t");
	ft_putstr(ft_program_name(argv0));
	ft_putstr(" fdf_file\n");
	ft_putstr("Controls:\n");
	ft_putstr("\t[Esc]        -> Exit the program.\n");
	ft_putstr("\t[R]          -> Reset the map.\n");
	ft_putstr("\t[M]          -> Colorize this shit!.\n");
	ft_putstr("\t[B][N]       -> Zoom in and zoom out.\n");
	ft_putstr("\t[J][K]       -> Spiky time!\n");
	ft_putstr("\t[Space]      -> Change the camera (isometric <-> plain).\n");
	ft_putstr("\t[W][A][S][D] -> Move the map.\n");
}

static void	reset_map(t_fdf *fdf)
{
	fdf->map.coordinate_z = 0;
	fdf->map.coordinate_y = 0;
	fdf->map.x_value = -1.00;
	fdf->map.angle_rot_x = 0;
	fdf->map.angle_rot_y = 0;
	fdf->map.angle_rot_z = 0;
	fdf->map.angle_y = cos(M_PI / 3);
	fdf->map.angle_z = fdf->map.angle_y * sin(M_PI / 6);
	fdf->map.zoom = ceil((fdf->map.width > fdf->map.height)) \
		? (WIN_WIDTH / fdf->map.width) + MAGIC_ZOOM \
		: (WIN_HEIGHT / fdf->map.height) + MAGIC_ZOOM;
	fdf->map.isometric = 1;
	fdf->color.red = 0x4F;
	fdf->color.green = 0x4F;
	fdf->color.blue = 0x4F;
}

static void	random_color(t_fdf *fdf)
{
	fdf->color.red = (rand() % 0x7F);
	fdf->color.green = (rand() % 0x7F);
	fdf->color.blue = (rand() % 0x7F);
}

static int	fdf_keys(int keycode, t_fdf *fdf)
{
	if (keycode == 53)
		exit(0);
	else if (keycode == 15)
		reset_map(fdf);
	else if (keycode == 46)
		random_color(fdf);
	else if (keycode == 13 || keycode == 126)
		fdf->map.coordinate_z += 3;
	else if (keycode == 1 || keycode == 125)
		fdf->map.coordinate_z -= 3;
	else if (keycode == 2 || keycode == 124)
		fdf->map.coordinate_y += 3;
	else if (keycode == 0 || keycode == 123)
		fdf->map.coordinate_y -= 3;
	else if (keycode == 11)
		fdf->map.zoom += 10;
	else if ((keycode == 45) && (fdf->map.zoom > MAX_ZOOM))
		fdf->map.zoom -= 10;
	else if ((keycode == 38) && (fdf->map.x_value < MAX_X))
		fdf->map.x_value += 0.25;
	else if ((keycode == 40) && (fdf->map.x_value > -MAX_X))
		fdf->map.x_value -= 0.25;
	else if (keycode == 49)
		fdf->map.angle_z *= (fdf->map.isometric++ % 2) ? 0.2 : 5;
	return (0);
}

int			main(int argc, char *argv[])
{
	t_fdf	*fdf;

	if (argc == 2)
	{
		if (!(fdf = (t_fdf *)malloc(sizeof(t_fdf))))
			ft_puterror("Memory Allocation failed!", 3);
		fdf_read(argv[1], fdf);
		reset_map(fdf);
		fdf->mlx.init = mlx_init();
		fdf->mlx.win = mlx_new_window(fdf->mlx.init, WIN_WIDTH, WIN_HEIGHT, \
			TITLE(ft_remove_extension(argv[1])));
		mlx_hook(fdf->mlx.win, 2, 3, fdf_keys, fdf);
		mlx_loop_hook(fdf->mlx.init, fdf_draw, fdf);
		mlx_loop(fdf->mlx.init);
	}
	else
		fdf_usage(argv[0]);
	return (0);
}
