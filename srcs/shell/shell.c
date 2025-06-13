/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:46:42 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/13 16:03:01 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	init_shell(t_shell *mshell, char **envp)
{
	ft_memset(mshell, 0, sizeof(t_shell));
	mshell->envp = env_dup(envp);
	if (!mshell->envp)
		return (error_msg(mshell, "init_shell: failed to duplicate environment",
				0));
	if (init_shlvl_env(mshell))
	{
		mshell->exit_code = 1;
		return (1);
	}
	if (init_pwd_env(mshell))
	{
		mshell->exit_code = 1;
		return (1);
	}
	mshell->exit_code = 0;
	return (0);
}

void	shell_cleanup(t_shell *mshell)
{
	if (!mshell)
		return ;
	if (mshell->envp)
		env_free(mshell);
}

void	shell_interactive(t_shell *mshell)
{
	char	*line;
	t_token	*history_head;
	int		status;

	history_head = NULL;
	while (1)
	{
		line = read_user_input(mshell);
		// //for test
		// if (isatty(fileno(stdin)))
		// 	line = readline("");
		// else
		// {
		// 	char *raw_line = get_next_line(fileno(stdin)); 

		// 	if (!raw_line)
		// 		break ;
		// 	line = ft_strtrim(raw_line, "\n");
		// 	free(raw_line);
		// }
		// ////
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		status = process_user_line(line, &history_head, mshell);
		free_string(line);
		if (status == FALSE)
			break;
	}
	clear_working_history(&history_head);
}


// save old main for testing
// void	shell_interactive(t_shell *mshell)
// {
// 	char	*line;
// 	t_token	*history_head;

// 	history_head = NULL;
// 	while (1)
// 	{
// 		g_signum = 0;
// 		line = readline("minishell$ ");
// 		if (g_signum) // check siganl after readline for Ctrl C in main shell
// 			sig_exit_code(mshell);
// 		line = read_user_input(mshell);
// 		if (!line) // Ctrl+D
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (line[0] == '\0') // Ctrl+C → empty str//need?
// 		{
// 			free(line);
// 			continue;
// 		}
// 		if (line[0] != '\0')
// 		{
// 			store_history(line, &history_head);
// 			if (handle_special_command_line(line, &history_head) == TRUE)
// 			{
// 				if (ft_strcmp(line, "exit") == 0)
// 				{
// 					free_string(line);
// 					break ;
// 				}
// 			}
// 			else
// 				handle_line(line, mshell);
// 		}
// 		free_string(line);
// 	}
// 	clear_working_history(&history_head);
// }