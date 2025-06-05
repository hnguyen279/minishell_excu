/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
/*   Updated: 2025/06/05 18:47:18 by trpham           ###   ########.fr       */
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
        g_signum = 0;
		line = readline("minishell$ ");
        if (g_signum)  // check siganl after readline for Ctrl C in main shell
           sig_exit_code(mshell);
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (line[0] == '\0')
		{
			free_string(line);
			continue ;
		}
        store_history(line, &history_head);
        if (handle_special_command_line(line, &history_head) == TRUE)
        {
            if (ft_strcmp(line, "exit") == 0)
            {
                free_string(line);
                break;
            }
        }
        else
            handle_line(line, mshell);
        free_string(line);
    }
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
    t_token *tokenized_input_list;
	t_cmd	*cmd_list;
	t_ast	*tree;

    mshell->heredoc_index = 0;
    if (validate_quote(line) == FALSE)
    {
        mshell->exit_code = 2;
        return ;
    }
    tokenized_input_list = NULL;
	cmd_list = NULL;
	tree = NULL;
    if (tokenization_expansion_validation(line, mshell, &tokenized_input_list) == FALSE ||
        empty_variable_extension(mshell, &tokenized_input_list) == FALSE)
    {
        free_token_list(tokenized_input_list);
        tokenized_input_list = NULL;
        return ;
    }
    print_linked_list(tokenized_input_list);
    process_valid_line(mshell, &tokenized_input_list, &cmd_list, &tree);
    free_token_list(tokenized_input_list);
    tokenized_input_list = NULL;
    free_cmd_list(cmd_list);
    free_ast(tree, mshell);
    mshell->ast = NULL;
    tree = NULL; 
}

int    tokenization_expansion_validation(char *line, t_shell *mshell, t_token **tokenized_input_list)
{
    *tokenized_input_list = convert_user_input_to_token(line);
    if (!*tokenized_input_list)
    {
        mshell->exit_code = 1;
        return (FALSE);
    }
    *tokenized_input_list = expand_variables(tokenized_input_list, mshell);
    if (!*tokenized_input_list)
    {
        free_token_list(*tokenized_input_list);
        return (FALSE);
    }
    if (validate_token(*tokenized_input_list) == FALSE)
    {
        mshell->exit_code = 2;
        return (FALSE);
    }
    return (TRUE);
}

int    empty_variable_extension(t_shell *mshell, t_token **tokenized_input_list)
{
    
    if (*tokenized_input_list && ft_strcmp((*tokenized_input_list)->value, "") == 0)
    {
        if ((*tokenized_input_list)->next)
        {
            t_token *to_free = *tokenized_input_list;
            (*tokenized_input_list) = (*tokenized_input_list)->next;
            free_string(to_free->value);
            free(to_free);
        }
        else
        {
            mshell->exit_code = 0;
            return (FALSE);
        }
    }
    return (TRUE);
}

void    process_valid_line(t_shell *mshell, t_token **tokenized_input_list, t_cmd **cmd_list, t_ast **tree)
{
    *cmd_list = parse_tokens_to_commands(*tokenized_input_list);
    if (!*cmd_list)
    {
        print_error("Failed to parse token to cmd");
        mshell->exit_code = 2;
        return ;
    }
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
    if (process_heredocs(mshell, tree))
    {
        cleanup_heredoc_tempfiles(tree);
        mshell->exit_code = 130;
        return ;
    }
    execute_ast(tree, mshell);
    cleanup_heredoc_tempfiles(tree);
    if (tree->cmd && tree->cmd[0] && !mshell->has_pipe) //tree->type == NODE_CMD &&
        env_set_last_argument(mshell, tree->cmd);
}
