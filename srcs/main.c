/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 00:42:49 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	main(int ac, char *av[], char *env[])
{
	t_shell	mshell;

	if (ac != 1)
	{
		print_error("No arguments required");
		return (EXIT_FAILURE);
	}
	if (init_shell(&mshell, env))
	{
		shell_cleanup(&mshell);
		return (EXIT_FAILURE);
	}
	(void)av;
	setup_signals(&mshell, MODE_INTERACTIVE);
	shell_interactive(&mshell);
	shell_cleanup(&mshell);
	return (mshell.exit_code);
}
