/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
/*   Updated: 2025/05/30 15:09:18 by trpham           ###   ########.fr       */
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

void cleanup_heredoc_tempfiles(t_ast *tree)
{
    if (!tree)
        return;
    if (tree->redirects)
    {
        t_redirect *redir = tree->redirects;
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
		line = readline("minishell$ ");
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
        store_history(line, &history_head);
        if (handle_special_command_line(line, &history_head) == TRUE)
        {
            if (ft_strcmp(line, "exit") == 0)
                break;
        }
        else
            handle_line(line, mshell);
        free_string(line);
    }
    free_string(line);
    clear_working_history(&history_head);
}

int handle_special_command_line(char *line, t_token **history_head)
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

void    handle_line(char *line, t_shell *mshell)
{
    t_token	*tokenized_input_list;
	t_cmd	*cmd_list;
	t_ast	*tree;
    // char    *expanded_line;

    // printf("print line: %s\n", line);
    if (validate_quote(line) == FALSE)
    {
        mshell->exit_code = 2;
        print_error("Failed to validate quote");
        return ;
    }
    // printf("validate line successfully\n");
    // expanded_line = expand_variables(line, mshell);
    tokenized_input_list = NULL;
	cmd_list = NULL;
	tree = NULL;
    process_valid_line(line, mshell, &tokenized_input_list, &cmd_list, &tree);
    // free_string(expanded_line);
    free_token_list(tokenized_input_list);
    free_cmd_list(cmd_list);
    free_ast(tree, mshell);
    mshell->ast = NULL;
    tree = NULL;
    g_signum = 0;  // Reset signal 
 
}

void    process_valid_line(char *line, t_shell *mshell, t_token **tokenized_input_list, t_cmd **cmd_list, t_ast **tree)
{
    *tokenized_input_list = convert_user_input_to_token(line);
    if (!*tokenized_input_list)
    {
        mshell->exit_code = 1;
        // free_string(line);
        print_error("Failed to convert user input to token");
        return ;
    }
    expand_variables(tokenized_input_list, mshell);
    // print_linked_list(*tokenized_input_list);
    if (validate_token(*tokenized_input_list) == FALSE)
    {
        mshell->exit_code = 2;
        print_error("Failed to validate token");
        return ;
    }
    *cmd_list = parse_tokens_to_commands(*tokenized_input_list);
    if (!*cmd_list)
    {
        print_error("Failed to parse token to cmd");
        mshell->exit_code = 2;
        return ;
    }
    
    // printf("parse token to cmds succeed\n");
    // print_cmd_list(cmd_list);
    *tree = convert_cmd_to_ast(*cmd_list);
    if (!*tree)
    {
        mshell->exit_code = 2;
        print_error("Failed to convert cmd to ast");
        return ;
    }
    run_ast_pipeline(mshell, *tree);
}

void    run_ast_pipeline(t_shell *mshell, t_ast *tree)
{
    // printf("ast tree is successfully created\n");
    if (process_heredocs(mshell, tree))
    {
        printf("execute process heredocts\n");
        mshell->exit_code = 1;
        return ;
    }
    //printf("execute ast\n");
    execute_ast(tree, mshell);
    cleanup_heredoc_tempfiles(tree);
    if (!mshell->has_pipe && tree->cmd && tree->cmd[0])
        env_set_last_argument(mshell, tree->cmd);
}
