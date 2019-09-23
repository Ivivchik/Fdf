/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkuhic <hkuhic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/22 02:29:54 by jaleman           #+#    #+#             */
/*   Updated: 2019/09/23 16:04:41 by hkuhic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <time.h>

# include "mlx.h"
# include "libft.h"

# define MAX_X			(10)
# define MAX_ZOOM		(5)
# define MAGIC_ZOOM		(2)
# define WIN_WIDTH		(1280)
# define WIN_HEIGHT		(720)
# define TITLE(file)	(ft_strjoin("fdf - ", file))

typedef struct	s_mlx
{
	void	*init;
	void	*win;
	void	*img;
}				t_mlx;

typedef struct	s_image
{
	char	*data;
	int		size;
	int		endian;
	int		bpp;
}				t_image;

typedef struct	s_map
{
	int		**values;
	int		width;
	int		height;
	int		coordinate_y;
	int		coordinate_z;
	double	y0;
	double	y1;
	double	z0;
	double	z1;
	int		zoom;
	int		isometric;
	double	rotate_y0;
	double	rotate_z0;
	double	rotate_x;
	double	rotate_y1;
	double	rotate_z1;
	double	x_value;
	double	angle_y;
	double	angle_z;
	double	angle_rot_x;
	double	angle_rot_y;
	double	angle_rot_z;

}				t_map;

typedef struct	s_color
{
	int		red;
	int		green;
	int		blue;
}				t_color;

typedef struct	s_fdf
{
	t_mlx	mlx;
	t_map	map;
	t_image	image;
	t_color	color;
}				t_fdf;

void			fdf_read(char *argv, t_fdf *fdf);
int				fdf_draw(t_fdf *fdf);

#endif
