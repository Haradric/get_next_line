/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraslav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/17 15:54:35 by mbraslav          #+#    #+#             */
/*   Updated: 2017/01/17 15:54:37 by mbraslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

t_list	*get_buff(int fd, t_list **list)
{
	t_list	*last;
	t_list	*new;

	last = *list;
	while (last)
	{
		if (fd == (int)last->content_size)
			return (last);
		last = last->next;
	}
	if (!(new = malloc(sizeof(t_list))))
		return (NULL);
	new->content_size = fd;
	new->content = ft_strnew(0);
	new->next = *list;
	*list = new;
	return (new);
}

void	expand_str(char **str, const char *buf)
{
	char	*new_str;

	new_str = ft_strjoin(*str, buf);
	free(*str);
	*str = new_str;
}

void	save_and_cut_rest(char **str, t_list *list)
{
	char	*new_str;
	char	*linebreak;

	linebreak = ft_strchr(*str, '\n');
	free(list->content);
	if (linebreak == NULL)
	{
		list->content = ft_strnew(0);
		return ;
	}
	else
	{
		list->content = ft_strdup(linebreak + 1);
		new_str = ft_strsub(*str, 0, ft_strlen(*str) - ft_strlen(linebreak));
		free(*str);
		*str = new_str;
	}
}

int		reading(const int fd, char **str)
{
	int		bytes;
	char	*buf;

	if (!(buf = ft_memalloc(BUFF_SIZE + 1)))
		return (-1);
	while ((bytes = (int)read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[bytes] = '\0';
		expand_str(str, buf);
		ft_bzero(buf, BUFF_SIZE + 1);
		if (ft_strchr(*str, '\n'))
			break ;
	}
	free(buf);
	return (bytes == 0 ? 0 : 1);
}

int		get_next_line(const int fd, char **line)
{
	static t_list	*list = NULL;
	t_list			*static_buff;
	char			*str;
	int				isempty;
	int				read_ret;

	if (fd < 0 || BUFF_SIZE <= 0 || read(fd, NULL, 0))
		return (-1);
	static_buff = get_buff(fd, &list);
	str = ft_strdup(static_buff->content);
	isempty = (str[0]) ? 0 : 1 ;
	read_ret = reading(fd, &str);
	if (read_ret == -1)
		return (-1);
	save_and_cut_rest(&str, static_buff);
	if (!ft_strlen(str) && !ft_strlen(static_buff->content) && isempty && \
		!read_ret)
	{
		*line = NULL;
		free(str);
		return (0);
	}
	*line = str;
	return (1);
}
