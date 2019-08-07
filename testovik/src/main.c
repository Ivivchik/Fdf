/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkuhic <hkuhic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 16:05:24 by jaleman           #+#    #+#             */
/*   Updated: 2019/08/07 19:18:55 by hkuhic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
** Display a usage message on how to run and use fdf.
*/

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

/*
** Reset the map to the initial values.
*/

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
/*
** Colorize the map with a random color.
** Colors go from 0x00 to 0x7F, because of technical issues (integer overflow).
*/

static void	random_color(t_fdf *fdf)
{
	fdf->color.red = (rand() % 0x7F);
	fdf->color.green = (rand() % 0x7F);
	fdf->color.blue = (rand() % 0x7F);
}

/*
** Assign a key code (macros defined in "includes/keys.h") to a specific task,
** like changing colors, moving the map, changing the view, zoom level, etc.
** There is an ugly code for changing the view (angle) of the map, because of
** norm... I guess? Everytime "map.isometric" isn't even, map the map.
*/

static int	fdf_keys(int keycode, t_fdf *fdf)
{
	if (keycode == 53)
		exit(0);
	else if (keycode == KEY_ANSI_R)
		reset_map(fdf);
	else if (keycode == KEY_ANSI_M)
		random_color(fdf);
	else if (keycode == KEY_ANSI_W || keycode == 126)
		fdf->map.coordinate_z += 3;
	else if (keycode == KEY_ANSI_S || keycode == KEY_DOWNARROW)
		fdf->map.coordinate_z -= 3;
	else if (keycode == KEY_ANSI_D || keycode == KEY_RIGHTARROW)
		fdf->map.coordinate_y += 3;
	else if (keycode == KEY_ANSI_A || keycode == KEY_LEFTARROW)
		fdf->map.coordinate_y -= 3;
	else if (keycode == KEY_ANSI_B)
		fdf->map.zoom += 10;
	else if ((keycode == KEY_ANSI_N) && (fdf->map.zoom > MAX_ZOOM))
		fdf->map.zoom -= 10;
	else if ((keycode == KEY_ANSI_J) && (fdf->map.x_value < MAX_X))
		fdf->map.x_value += 0.25;
	else if ((keycode == 40) && (fdf->map.x_value > -MAX_X))
		fdf->map.x_value -= 0.25;
	else if (keycode == KEY_SPACE)
		fdf->map.angle_z *= (fdf->map.isometric++ % 2) ? 0.2 : 5;
	else if (keycode == 4)
		fdf->map.angle_rot_x += M_PI / 4;
	else if (keycode == 5)
		fdf->map.angle_rot_y += M_PI / 4;
	else if (keycode == 3)
		fdf->map.angle_rot_z += M_PI / 4;
	fdf_draw(fdf);
	return (0);
}

/*
** Main function, creates the fdf structure and a network connection between
** the program and the x-server, making it possible to draw on the screen,
** and getting the keyboard inputs.
** Didn't used mlx_key_hook, because I wanted to press and hold the keys :D
*/

int			main(int argc, char *argv[])
{
	t_fdf	*fdf;

	if (argc == 2)
	{
		if (!(fdf = (t_fdf *)malloc(sizeof(t_fdf))))
			ft_puterror("Memory Allocation failed! :O", 3);
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
