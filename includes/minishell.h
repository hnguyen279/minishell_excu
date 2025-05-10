/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:02:35 by thi-huon          #+#    #+#             */
/*   Updated: 2025/04/21 14:15:43 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define FD_READ  0
#define FD_WRITE 1
#define MODE_INTERACTIVE 0
#define MODE_HEREDOC 1
#define MODE_CHILD 2

extern volatile sig_atomic_t g_signum;

// Structs
typedef enum e_node_type
{
    NODE_NONE = 0,
    NODE_CMD,
    NODE_PIPE,
} t_node_type;

typedef enum e_redirect_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redirect_type;

typedef struct s_redirect
{
    int fd;
    t_redirect_type type;
    char *file;
	char *ori_path;
    char *tmp_file;
    struct s_redirect *next;
} t_redirect;

typedef struct s_ast
{
    t_node_type type;
    char **cmd;
    t_redirect *redirects;
    struct s_ast *left;
    struct s_ast *right;
} t_ast;

typedef struct s_shell
{
    int exit_code;
    char **env;
    int heredoc_index;
    t_ast		*ast;
} t_shell;



typedef struct s_tokens
{
    int has_pipe;
    char *env_last_cmd; 
    char **token;       
} t_tokens;

#endif

// typedef struct s_shell
// {
//     char **envp;        
//     t_tokens *tokens;
//     int exit_code;  
//     int heredoc_index;
//     char *cwd;          // current folder
//     char *old_pwd;      // previous folder

// } t_shell;