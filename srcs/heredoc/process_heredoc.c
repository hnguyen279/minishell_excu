#include "../../includes/shell.h"

char *make_heredoc_filename_from_fd(int fd)
{
    char *fd_str;
    char *path;
    int i;
    int j;
    const char *prefix;

    prefix  = "/tmp/.heredoc_";
    i = 0;
    j = 0;
    fd_str = ft_itoa(fd);
    if (!fd_str)
        return (NULL);
    path = malloc(16 + 10); // "/tmp/.heredoc_" + max 10 digit
    if (!path)
        return (free(fd_str), NULL);
    while (prefix[i])
    {
        path[i] = prefix[i];
        i++;
    }
    while (fd_str[j])
        path[i++] = fd_str[j++];
    path[i] = '\0';
    free(fd_str);
    return (path);
}

int process_heredocs(t_shell *mshell, t_ast *node)
{
    if (!node)
        return 0;
    t_redirect *redir = node->redirects;
    while (redir)
    {
        if (redir->type == REDIR_HEREDOC)
        {
            if (open_heredoc_pipe(mshell, redir) != 0)
                return mshell->exit_code;
        }
        redir = redir->next;
    }
    if (node->type == NODE_PIPE)
    {
        if (process_heredocs(mshell, node->left) != 0)
            return mshell->exit_code;
        if (process_heredocs(mshell, node->right) != 0)
            return mshell->exit_code;
    }
    return 0;
}
