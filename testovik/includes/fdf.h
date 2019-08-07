/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkuhic <hkuhic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/22 02:29:54 by jaleman           #+#    #+#             */
/*   Updated: 2019/08/07 18:53:48 by hkuhic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

/*
** Required header files.
*/

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <time.h>

/*
** User defined header files.
*/

# include "mlx.h"
# include "keys.h"
# include "libft.h"

/*
** Macros for the max value of x coordinate, the window size (width and height),
** and the title of the window (removing the .fdf extension).
*/

# define MAX_X			(10)
# define MAX_ZOOM		(5)
# define MAGIC_ZOOM		(2)
# define WIN_WIDTH		(1280)
# define WIN_HEIGHT		(720)
# define TITLE(file)	(ft_strjoin("fdf - ", file))

/*
** FDF Structures
**
** @t_mlx   : required minilibx arguments.
** @t_image : variables to be used for creation of the image (map).
**            data -> information about the image, allowing to be modified.
**            size -> move from one line to another in the image.
**            endian -> how the pixel color in the image needs to be stored.
**            bpp -> filled with the number of bits to represent a pixel color.
** @t_map   : values used to manipulate the fdf map.
** @t_color : define the color, rgb value (24-bits).
** @t_fdf   : nested structure that calls all of the other structures.
*/

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

/*
** Prototypes of the read file and draw functions.
*/

void			fdf_read(char *argv, t_fdf *fdf);
int				fdf_draw(t_fdf *fdf);

#endif
