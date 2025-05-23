/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
/*   Updated: 2025/05/22 15:24:20 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int main(int ac, char *av[], char *env[])
{
    t_shell mshell;

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

void shell_interactive(t_shell *mshell)
{
    char *line;
    t_token *history_head = NULL;
    t_token *tokenized_input_list = NULL;
    t_cmd *cmd_list = NULL;
    t_ast *tree = NULL;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line) // Ctrl+D
        {
            printf("exit\n");
            break;
        }
        if (line[0] == '\0')
        {
            free(line);
            continue;
        }
        if (line[0] != '\0')
        {
            store_history(line, &history_head);
            if (ft_strcmp(line, "exit") == 0)
            {
                free_string(line);
                break;
            }
            else if (ft_strcmp(line, "history") == 0)
                print_working_history(history_head);
            else if (ft_strcmp(line, "history -c") == 0)
                clear_working_history(&history_head);
            else if (validate_quote(line) != FALSE)
            {
                tokenized_input_list = convert_user_input_to_token(line);
                if (!tokenized_input_list)
                {
                    mshell->exit_code = 1;
                    free_string(line);
                    continue;
                }
                if (validate_token(tokenized_input_list) == TRUE)
                {
                    cmd_list = parse_tokens_to_commands(tokenized_input_list);
                    if (!cmd_list)
                    {
                        print_error("Failed to parse token to cmd");
                        mshell->exit_code = 2;
                        get_error_msg(ERR_SYNTAX);
                    }
                    else
                    {
                        // printf("parse token to cmds succeed\n");
                        // print_cmd_list(cmd_list);
                        tree = convert_cmd_to_ast(cmd_list);
                        if (!tree)
                        {
                            // free_cmd_list(cmd_list);
                            mshell->exit_code = 2;
                            get_error_msg(ERR_MALLOC);
                        }
                        else
                        {
                            // printf("ast tree is successfully created\n");
                            if (process_heredocs(mshell, tree))
                            {
                                printf("execute process heredocts\n");
                                mshell->exit_code = 1;
                            }
                            else
                            {
                                //printf("execute ast\n");
                                execute_ast(tree, mshell);
                                if (!mshell->has_pipe && mshell->env_last_cmd)
                                    env_set_last_argument(mshell, NULL);
                            }
                        }
                    }
                }
                else
                {
                    mshell->exit_code = 2;
                    get_error_msg(ERR_SYNTAX);
                }
            }
            else
            {
                mshell->exit_code = 2;
                get_error_msg(ERR_QUOTE);
            }
        }
        free_string(line);
        free_token_list(tokenized_input_list);
        tokenized_input_list = NULL;
        free_cmd_list(cmd_list);
        cmd_list = NULL;
        free_ast(tree, mshell);
        tree = NULL;
        mshell->ast = NULL;
        g_signum = 0;  // Reset signal 
    }
    // printf("before clear working history\n");
    clear_working_history(&history_head);
}


