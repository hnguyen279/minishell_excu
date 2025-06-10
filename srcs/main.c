/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 10:19:06 by trpham           ###   ########.fr       */
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

void	cleanup_heredoc_tempfiles(t_ast *tree)
{
	t_redirect	*redir;

	if (!tree)
		return ;
	if (tree->redirects)
	{
		redir = tree->redirects;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && redir->tmp_file)
			{
				unlink(redir->tmp_file);
				free(redir->tmp_file);
				redir->tmp_file = NULL;
			}
			redir = redir->next;
		}
	}
	cleanup_heredoc_tempfiles(tree->left);
	cleanup_heredoc_tempfiles(tree->right);
}

void	shell_interactive(t_shell *mshell)
{
	char	*line;
	t_token	*history_head;

	history_head = NULL;
	while (1)
	{
		g_signum = 0;
		line = readline("minishell$ ");
		if (g_signum) // check siganl after readline for Ctrl C in main shell
			sig_exit_code(mshell);
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (line[0] == '\0') // Ctrl+C → empty str//need?
		{
			free(line);
			continue;
		}
		if (line[0] != '\0')
		{
			store_history(line, &history_head);
			if (handle_special_command_line(line, &history_head) == TRUE)
			{
				if (ft_strcmp(line, "exit") == 0)
				{
					free_string(line);
					break ;
				}
			}
			else
				handle_line(line, mshell);
		}
		free_string(line);
	}
	clear_working_history(&history_head);
}

int	handle_special_command_line(char *line, t_token **history_head)
{
	if (ft_strcmp(line, "exit") == 0)
		return (TRUE);
	else if (ft_strcmp(line, "history") == 0)
	{
		print_working_history(*history_head);
		return (TRUE);
	}
	else if (ft_strcmp(line, "history -c") == 0)
	{
		clear_working_history(history_head);
		return (TRUE);
	}
	return (FALSE);
}

void	run_ast_pipeline(t_shell *mshell, t_ast *tree)
{
	if (process_heredocs(mshell, tree))
	{
		cleanup_heredoc_tempfiles(tree);
		mshell->exit_code = 130;
		return ;
	}
	execute_ast(tree, mshell);
	cleanup_heredoc_tempfiles(tree);
	if (tree->cmd && tree->cmd[0] && !mshell->has_pipe)
		// tree->type == NODE_CMD &&
		env_set_last_argument(mshell, tree->cmd);
}

// for testing
// void	shell_interactive(t_shell *mshell)
// {
// 	char		*line;
// 	t_token		*history_head = NULL;

// 	while (1)
// 	{
// 		g_signum = 0;

// 		if (isatty(fileno(stdin)))
// 			line = readline("");
// 		else
// 		{
// 			char *raw_line = get_next_line(fileno(stdin)); 
				// non-interactive mode
// 			if (!raw_line)
// 				break ;
// 			line = ft_strtrim(raw_line, "\n");
// 			free(raw_line);
// 		}

// 		if (g_signum)  // Signal interrupt (e.g., Ctrl+C)
// 			sig_exit_code(mshell);

// 		if (!line)  // Ctrl+D or EOF
// 			break ;

// 		if (line[0] == '\0')  // Empty line (Enter key)
// 		{
// 			free_string(line);
// 			continue ;
// 		}

// 		store_history(line, &history_head);

// 		if (handle_special_command_line(line, &history_head) == TRUE &&
// 			ft_strcmp(line, "exit") == 0)
// 		{
// 			free_string(line);
// 			break ;
// 		}
// 		else
// 			handle_line(line, mshell);

// 		free_string(line);
// 	}
// 	clear_working_history(&history_head);
// 	// printf("exit\n");  <-- REMOVE OR COMMENT OUT for tester compatibility
// }
