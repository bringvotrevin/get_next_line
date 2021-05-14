/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dim <dim@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 05:25:47 by dim               #+#    #+#             */
/*   Updated: 2021/02/27 04:20:03 by dim              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_fd	*find_fd(t_fd **start, int fd)
{
	t_fd	*save;

	save = *start;
	while (save != NULL)
	{
		if (save->fd == fd)
			return (save);
		save = save->next;
	}
	if (!(save = (t_fd *)malloc(sizeof(t_fd))))
		return (0);
	save->fd = fd;
	save->backup = NULL;
	save->backup_size = 0;
	save->next = *start;
	*start = save;
	return (save);
}

void	del_fd(t_fd **head, t_fd *save)
{
	t_fd	*cur;
	t_fd	*cur2;

	cur = *head;
	while (save == cur)
	{
		*head = cur->next;
		free(cur->backup);
		free(cur);
		return ;
	}
	while (cur != NULL)
	{
		cur2 = cur->next;
		if (cur2 == save)
			break ;
		cur = cur2;
	}
	cur->next = save->next;
	free(cur2->backup);
	free(cur2);
}

void	del_all_fd(t_fd **head)
{
	t_fd	*cur;
	t_fd	*cur2;

	cur = *head;
	while (cur != NULL)
	{
		cur2 = cur->next;
		free(cur->backup);
		free(cur);
		cur = cur2;
	}
	*head = NULL;
}

char	*ft_strdup(char *arr, size_t size)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!(str = (char *)malloc(sizeof(char) * (size + 1))))
		return (0);
	while (i < size)
	{
		str[i] = arr[i];
		i++;
	}
	str[i] = 0;
	return (str);
}
