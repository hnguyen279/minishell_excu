/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:46:42 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/19 02:50:36 by trpham           ###   ########.fr       */
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
	if (mshell->history_head)
		free_token_list(mshell->history_head);
	if (mshell->token_list)
		free_token_list(mshell->token_list);
	if (mshell->cmd_list)
		free_cmd_list(mshell->cmd_list);
	if (mshell->tree)
		free_ast(mshell->tree);
	if (mshell->envp)
		env_free(mshell);
	if (mshell->old_pwd)
		free(mshell->old_pwd);
	if (mshell->curr_pwd)
		free(mshell->curr_pwd);
}

void	shell_interactive(t_shell *mshell)
{
	char	*line;

	while (1)
	{
		line = read_user_input(mshell);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		if (process_non_empty_input(line, mshell) == FALSE)
		{
			free(line);
			loop_clean(mshell);
			break ;
		}
		loop_clean(mshell);
		free(line);
	}
	rl_clear_history();
}

int	process_non_empty_input(char *line, t_shell *mshell)
{
	if (store_history(line, &mshell->history_head) == FALSE
		|| process_user_line(line, mshell) == FALSE)
	{
		return (FALSE);
	}
	loop_clean(mshell);
	return (TRUE);
}

char	*read_user_input(t_shell *mshell)
{
	char	*line;

	g_signum = 0;
	line = readline("minishell$ ");
	if (g_signum) // check siganl after readline for Ctrl C in main shell
		sig_exit_code(mshell);
	return (line);
}

/////for test

// void	shell_interactive(t_shell *mshell)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		/// /// test
// 		if (isatty(fileno(stdin)))
// 			line = readline("");
// 		else
// 		{
// 			char *raw_line = get_next_line(fileno(stdin)); 

// 			if (!raw_line)
// 				break ;
// 			line = ft_strtrim(raw_line, "\n");
// 			free(raw_line);
// 		}
// 		////////
// 		if (!line)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (line[0] == '\0')
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (process_non_empty_input(line, mshell) == FALSE)
// 		{
// 			free(line);
// 			loop_clean(mshell);
// 			break;
// 		}
// 		loop_clean(mshell);
// 		free(line);
// 	}
// 	rl_clear_history();
// }