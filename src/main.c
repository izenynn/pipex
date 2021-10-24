/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpoveda- <me@izenynn.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 18:36:26 by dpoveda-          #+#    #+#             */
/*   Updated: 2021/10/24 18:36:46 by dpoveda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(int argc, char *argv[], char *envp[])
{
	extern char	**environ;

	printf("ENVIRON:\n");
	for (int i = 0; environ[i]; i++)
		printf("env: %s\n", environ[i]);

	printf("ENVP:\n");
	for (int i = 0; envp[i]; i++)
		printf("env: %s\n", envp[i]);

	// silence warnings :)
	if (argv[0] || argc)
		return (0);
	return (0);
}
