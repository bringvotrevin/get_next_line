/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dim <dim@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 15:04:54 by dim               #+#    #+#             */
/*   Updated: 2021/02/27 04:20:21 by dim              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int			get_next_line(int fd, char **line)
{
	static t_fd	*start;
	t_fd		*save;
	int			i;

	if (BUFFER_SIZE < 1 || line == NULL)
		return (-1);
	if (!(save = find_fd(&start, fd)))
		return (-1);
	i = read_fd(save, line);
	if (i == -1)
		del_all_fd(&start);
	else if (i == 0)
		del_fd(&start, save);
	return (i);
}

int			read_fd(t_fd *save, char **line)
{
	ssize_t		buf_size;
	int			l;
	char		buffer[BUFFER_SIZE];

	l = to_line(line, save);
	if (l)
		return (l);
	while ((buf_size = read(save->fd, buffer, BUFFER_SIZE)) > 0)
	{
		if (buf_to_backup(save, buffer, buf_size) == -1)
			return (-1);
		l = to_line(line, save);
		if (l == -1 || l == 1)
			return (l);
	}
	if (buf_size < 0)
		return (-1);
	l = if_eof(line, save);
	return (l);
}

int			buf_to_backup(t_fd *save, char *new_buffer, size_t buf_size)
{
	size_t		i;
	size_t		j;
	size_t		size;
	char		*backup_buf;

	size = save->backup_size + buf_size;
	if (!(backup_buf = (char *)malloc(sizeof(char) * size)))
		return (-1);
	i = 0;
	while (i < save->backup_size)
	{
		backup_buf[i] = save->backup[i];
		i++;
	}
	j = 0;
	while (j < buf_size)
	{
		backup_buf[i + j] = new_buffer[j];
		j++;
	}
	free(save->backup);
	save->backup = backup_buf;
	save->backup_size = size;
	return (1);
}

int			to_line(char **line, t_fd *save)
{
	size_t		i;
	char		*re_backup;

	i = 0;
	while (i < save->backup_size)
	{
		if (save->backup[i] == '\n')
			break ;
		i++;
	}
	if (i == save->backup_size)
		return (0);
	if (!(*line = ft_strdup(save->backup, i)))
		return (-1);
	if (!(re_backup = ft_strdup(save->backup + i + 1,
					save->backup_size - i - 1)))
	{
		free(*line);
		return (-1);
	}
	free(save->backup);
	save->backup = re_backup;
	save->backup_size = save->backup_size - i - 1;
	return (1);
}

int			if_eof(char **line, t_fd *save)
{
	int			l;

	while (1)
	{
		if (!(l = to_line(line, save)))
			break ;
	}
	if (!(*line = ft_strdup(save->backup, save->backup_size)))
		return (-1);
	return (0);
}
