// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
// /*   Updated: 2025/05/17 07:09:45 by trpham           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../includes/shell.h"



// static void shell_interactive(t_shell *mshell)
// {
//     char *line;
//     t_token *history_head = NULL;
//     t_token *tokenized_input_list = NULL;
//     t_cmd *cmd_list = NULL;
//     t_ast *tree = NULL;

//     while (1)
//     {
//         line = readline("minishell$ ");
//         if (!line) // Ctrl+D
//         {
//             printf("exit\n");
//             break;
//         }
//         if (line[0] != '\0')
//         {
//             store_history(line, &history_head);
//             if (ft_strcmp(line, "exit") == 0)
//             {
//                 free_string(line);
//                 break;
//             }
//             else if (ft_strcmp(line, "history") == 0)
//                 print_working_history(history_head);
//             else if (ft_strcmp(line, "history -c") == 0)
//                 clear_working_history(&history_head);
//             else if (validate_quote(line) != FALSE)
//             {
//                 tokenized_input_list = convert_user_input_to_token(line);
//                 if (!tokenized_input_list)
//                 {
//                     mshell->exit_code = 1;
//                     free_string(line);
//                     continue;
//                 }
//                 if (validate_token(tokenized_input_list) == TRUE)
//                 {
//                     cmd_list = parse_tokens_to_commands(tokenized_input_list);
//                     if (!cmd_list)
//                     {
//                         mshell->exit_code = 2;
//                         get_error_msg(ERR_SYNTAX);
//                     }
//                     else
//                     {
//                         tree = convert_cmd_to_ast(cmd_list);
//                         if (!tree)
//                         {
//                             mshell->exit_code = 2;
//                             get_error_msg(ERR_MALLOC);
//                         }
//                         else
//                         {
//                             if (process_heredocs(mshell, tree))
//                                 mshell->exit_code = 1;
//                             else
//                                 execute_ast(tree, mshell);
//                         }
//                     }
//                 }
//                 else
//                 {
//                     mshell->exit_code = 2;
//                     get_error_msg(ERR_SYNTAX);
//                 }
//             }
//             else
//             {
//                 mshell->exit_code = 2;
//                 get_error_msg(ERR_QUOTE);
//             }
//         }
//         free_string(line);
//         free_token_list(tokenized_input_list);
//         tokenized_input_list = NULL;
//         free_cmd_list(cmd_list);
//         cmd_list = NULL;
//         free_ast(tree, mshell);
//         tree = NULL;
//     }
//     clear_working_history(&history_head);
// }

// int main(int ac, char *av[], char *env[])
// {
//     t_shell shell;

//     if (ac != 1)
//         return (EXIT_FAILURE);
//     if (init_shell(&shell, env))
//     {
//         shell_cleanup(&shell);
//         return (EXIT_FAILURE);
//     }
//     (void)av;
//     setup_signals(MODE_INTERACTIVE);
//     shell_interactive(&shell);
//     shell_cleanup(&shell);
//     return (shell.exit_code);
// }


#include "../includes/shell.h"

static void free_token_list(t_token *tokens)
{
    t_token *tmp;
    while (tokens)
    {
        tmp = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

static void shell_interactive(t_shell *mshell)
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
                    cmd_list = parse_tokens_to_commands(tokenized_input_list); // Sửa: thêm mshell
                    if (!cmd_list)
                    {
                        mshell->exit_code = 2;
                        get_error_msg(ERR_SYNTAX);
                    }
                    else
                    {
                        print_cmd_list(cmd_list);
                        tree = convert_cmd_to_ast(cmd_list);
                        if (!tree)
                        {
                            mshell->exit_code = 2;
                            get_error_msg(ERR_MALLOC);
                        }
                        else
                        {
                            printf("ast tree is successfully created\n");
                            if (process_heredocs(mshell, tree))
                                mshell->exit_code = 1;
                            else
                                execute_ast(tree, mshell);
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
        // free_cmd_list(cmd_list);
        // cmd_list = NULL;
        // free_ast(tree, mshell);
        // tree = NULL;
    }
    clear_working_history(&history_head);
}

int main(int ac, char *av[], char *env[])
{
    t_shell mshell;

    if (ac != 1)
    {
        print_error("minishell: no arguments required");
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

