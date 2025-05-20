#include "../../includes/shell.h"

t_cmd *create_cmd(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        get_error_msg(ERR_MALLOC);
        return (NULL);
    }
    cmd->cmd_name = NULL;
    cmd->args = NULL;
    cmd->redirects = NULL;
    cmd->next = NULL;
    return (cmd);
}

int add_redirects(t_redirect **redir_list, t_redirect_type type, char *file)
{
    t_redirect *new_redir = malloc(sizeof(t_redirect));
    if (!new_redir)
    {
        get_error_msg(ERR_MALLOC);
        return (FALSE);
    }
    new_redir->file = ft_strdup(file);
    if (!new_redir->file)
    {
        free(new_redir);
        get_error_msg(ERR_MALLOC);
        return (FALSE);
    }
    new_redir->type = type;
    new_redir->fd = (type == REDIR_IN || type == REDIR_HEREDOC) ? 0 : 1;
    new_redir->ori_path = NULL;
    new_redir->tmp_file = NULL;
    new_redir->next = NULL;

    if (!*redir_list)
        *redir_list = new_redir;
    else
    {
        t_redirect *current = *redir_list;
        while (current->next)
            current = current->next;
        current->next = new_redir;
    }
    return (TRUE);
}

int count_args(t_token *token_list)
{
    int count = 0;

    while (token_list && token_list->type != PIPE)
    {
        if (token_list->type == WORD && !is_redirection(token_list))
        {
            count++;
            token_list = token_list->next;
        }
        else if (is_redirection(token_list))
        {
            token_list = token_list->next; // skip redirection operator
            if (token_list) // skip file
                token_list = token_list->next;
        }
        else
        {
            token_list = token_list->next;
        }
    }
    return count;
}


char **fill_args(t_token **token_list)
{
    int count = count_args(*token_list);
    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args)
    {
        get_error_msg(ERR_MALLOC);
        return (NULL);
    }
    int i = 0;
    while (*token_list && (*token_list)->type != PIPE)
    {
        if ((*token_list)->type == WORD && !is_redirection(*token_list))
        {
            args[i] = ft_strdup((*token_list)->value);
            if (!args[i])
            {
                free_array(args, i);
                get_error_msg(ERR_MALLOC);
                return (NULL);
            }
            i++;
        }
        else if (is_redirection(*token_list))
        {
            *token_list = (*token_list)->next;
            if (*token_list)
                *token_list = (*token_list)->next;
            continue;
        }
        if (*token_list)
            *token_list = (*token_list)->next;
    }
    args[i] = NULL;
    return (args);
}

int parse_redirection(t_cmd **new_cmd, t_token **token_list)
{
    t_redirect_type redir_type;

    while (*token_list && (*token_list)->type != PIPE)
    {
        if (is_redirection(*token_list))
        {
            if ((*token_list)->type == REDIR_IN_token)
                redir_type = REDIR_IN;
            else if ((*token_list)->type == REDIR_OUT_token)
                redir_type = REDIR_OUT;
            else if ((*token_list)->type == REDIR_APPEND_token)
                redir_type = REDIR_APPEND;
            else if ((*token_list)->type == REDIR_HEREDOC_token)
                redir_type = REDIR_HEREDOC;
            else
                return (FALSE);

            *token_list = (*token_list)->next;
            if (!*token_list || (*token_list)->type != WORD)
            {
                get_error_msg(ERR_REDIR);
                return (FALSE);
            }
            if (!add_redirects(&(*new_cmd)->redirects, redir_type, (*token_list)->value))
                return (FALSE);
        }
        *token_list = (*token_list)->next;
    }
    return (TRUE);
}

int update_command_node(t_cmd **new_cmd, t_token **temp_token_list)
{
    (*new_cmd)->args = fill_args(temp_token_list);
    if (!(*new_cmd)->args)
        return (FALSE);
    if ((*new_cmd)->args[0])
    {
        (*new_cmd)->cmd_name = ft_strdup((*new_cmd)->args[0]);
        if (!(*new_cmd)->cmd_name)
        {
            free_array((*new_cmd)->args, array_size((*new_cmd)->args));
            get_error_msg(ERR_MALLOC);
            return (FALSE);
        }
    }
    if (!parse_redirection(new_cmd, temp_token_list))
    {
        free_array((*new_cmd)->args, array_size((*new_cmd)->args));
        free((*new_cmd)->cmd_name);
        return (FALSE);
    }
    return (TRUE);
}

t_cmd *parse_tokens_to_commands(t_shell *mshell, t_token *tokenized_input_list)
{
    t_cmd *cmd_list = NULL;
    t_cmd *current = NULL;
    t_token *temp_token_list = tokenized_input_list;

    mshell->has_pipe = 0;
    while (temp_token_list)
    {
        t_cmd *new_cmd = create_cmd();
        if (!new_cmd)
        {
            free_cmd_list(cmd_list);
            get_error_msg(ERR_MALLOC);
            return (NULL);
        }
        if (!cmd_list)
            cmd_list = new_cmd;
        else
            current->next = new_cmd;
        current = new_cmd;

        if (!update_command_node(&new_cmd, &temp_token_list))
        {
            free_cmd_list(cmd_list);
            return (NULL);
        }
        if (!new_cmd->args || !new_cmd->args[0])
        {
            get_error_msg(ERR_SYNTAX);
            free_cmd_list(cmd_list);
            return (NULL);
        }
        if (temp_token_list && temp_token_list->type == PIPE)
        {
            mshell->has_pipe = 1;
            temp_token_list = temp_token_list->next;
            if (!temp_token_list)
            {
                get_error_msg(ERR_SYNTAX);
                free_cmd_list(cmd_list);
                return (NULL);
            }
        }
    }
    return (cmd_list);
}

void free_cmd_list(t_cmd *head)
{
    t_cmd *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free_array(temp->args, array_size(temp->args));
        free(temp->cmd_name);
        t_redirect *redir = temp->redirects;
        while (redir)
        {
            t_redirect *next = redir->next;
            free(redir->file);
            free(redir->tmp_file);
            free(redir->ori_path);
            free(redir);
            redir = next;
        }
        free(temp);
    }
}
