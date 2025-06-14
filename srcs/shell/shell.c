/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:46:42 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/16 03:54:54 by thi-huon         ###   ########.fr       */
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

void shell_cleanup(t_shell *mshell)
{
	if (!mshell)
		return ;
	if (mshell->history_head) //check again free every when out?
		free_token_list(mshell->history_head); // set NULL?
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

// void shell_cleanup(t_shell *mshell)
// {
// 	loop_clean(mshell);
// 	if (mshell->history_head)
// 	{
// 		free_token_list(mshell->history_head); //confirm with Tr, set NULL or not?
// 		mshell->history_head = NULL;
// 	}
// 	if (mshell->envp)
// 	{
// 		env_free(mshell);
// 		mshell->envp = NULL;
// 	}
// 	if (mshell->old_pwd)
// 	{
// 		free(mshell->old_pwd);
// 		mshell->old_pwd = NULL;
// 	}
// 	if (mshell->curr_pwd)
// 	{
// 		free(mshell->curr_pwd);
// 		mshell->curr_pwd = NULL;
// 	}
// }

void	shell_interactive(t_shell *mshell)
{
	char	*line;
	int		status;

	while (1)
	{
		line = read_user_input(mshell);
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != '\0') // Ctrl+C → empty str//need?
		{
			store_history(line, &mshell->history_head);
			status = process_user_line(line, mshell);
			loop_clean(mshell); ///check again need or not --> clean loop before the new input line
			mshell->heredoc_index = 0;
			mshell->has_pipe = 0; 
			if (status == FALSE)
			{
				free(line);
				break;
			}
		}
		free(line);
	}
	rl_clear_history();
}


/////for test
// void	shell_interactive(t_shell *mshell)
// {
// 	char	*line;
// 	int		status;

// 	while (1)
// 	{
// 		// line = read_user_input(mshell);
// 		// //for test
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
// 		///////////
// 		if (!line) // Ctrl+D
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (line[0] != '\0') // Ctrl+C → empty str//need?
// 		{
// 			store_history(line, &mshell->history_head);
// 			status = process_user_line(line, mshell);
// 			loop_clean(mshell); ///check again need or not --> clean loop before the new input line
// 			if (status == FALSE)
// 			{
// 				free(line);
// 				break;
// 			}
// 		}
// 		free(line);
// 	}
// 	rl_clear_history();
// }

char	*read_user_input(t_shell *mshell)
{
	char	*line;
	
	g_signum = 0;
	line = readline("minishell$ ");
	if (g_signum) // check siganl after readline for Ctrl C in main shell
		sig_exit_code(mshell);
	return (line);
}

void loop_clean(t_shell *mshell)
{
	if (!mshell)
		return ;
	if (mshell->token_list)
	{
		free_token_list(mshell->token_list);	
		mshell->token_list = NULL;
	}
	if (mshell->cmd_list)
	{
		free_cmd_list(mshell->cmd_list);
		mshell->cmd_list = NULL;
	}
	if (mshell->tree)
	{
		free_ast(mshell->tree);
		mshell->tree = NULL;
	}
}
