/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpoveda- <me@izenynn.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 20:11:12 by dpoveda-          #+#    #+#             */
/*   Updated: 2021/10/24 20:11:13 by dpoveda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	cnt;

	cnt = 0;
	while (s[cnt])
		cnt++;
	return (cnt);
}

char	*ft_strchr(const char *s, int c)
{
	if (c == '\0')
		return ((char *)&s[ft_strlen(s)]);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char	*ft_strdup(const char *s1)
{
	char	*cpy;
	char	*save;
	size_t	len;

	len = ft_strlen(s1);
	cpy = malloc(len + 1);
	if (!cpy)
		return (NULL);
	save = cpy;
	while (*s1)
		*cpy++ = *s1++;
	*cpy = '\0';
	return (save);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	out_len;
	char	*save;
	char	*out;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	out_len = ft_strlen(s + start);
	if (out_len < len)
		len = out_len;
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	save = out;
	s += start;
	while (*s && len--)
		*out++ = *s++;
	*out = '\0';
	return (save);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
		i++;
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	else
		return (0);
}

char	*dir_join(char const *dir1, char const *dir2)
{
	char	*save;
	char	*out;

	if (!dir1 || !dir2)
		return (NULL);
	out = malloc(ft_strlen(dir1) + ft_strlen(dir2) + 2);
	if (!out)
		return (NULL);
	save = out;
	while (*dir1)
		*out++ = *dir1++;
	*out++ = '/';
	while (*dir2)
		*out++ = *dir2++;
	*out = '\0';
	return (save);
}

void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}
