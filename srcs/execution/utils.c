
#include "../../includes/shell.h"

void	free_split(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}	
	free(tab);
}

int display_error_errno(t_shell *mshell, const char *msg, int use_errno)
{
	if (use_errno)
		ft_printf_fd(2, "minishell: %s: %s\n", msg, strerror(errno));
	else
		ft_printf_fd(2, "minishell: %s\n", msg);
    if (mshell)
		mshell->exit_code = 1;
	if (use_errno)
	    return (-1);
    else
	    return (1);
}

int	display_error_cmd(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
	else if (!ft_strcmp(cmd, "."))
		ft_printf_fd(STDERR_FILENO, "minishell: .: filename argument required\n.: usage: . filename [arguments]\n");
	else if (!ft_strcmp(cmd, ".."))
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
	else
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
	return (1);
}
int is_white_spaces_cmd(char *cmd)
{
    int i;
	i = 0;
    while (cmd[i])
    {
        if (cmd[i] != ' ' && (cmd[i] < 9 || cmd[i] > 13))
            return 0;
        i++;
    }
    return 1;
}

int is_ambiguous_redirect(t_shell *mshell, t_redirect *redir)
{
	if (!redir || !redir->file || !redir->ori_path)
		return (0);
	if (redir->file[0] == '\0' && redir->ori_path[0] != '\0'
		&& !is_fully_quoted(redir->ori_path)
		&& redir->type != REDIR_HEREDOC)
	{
		ft_printf_fd(2, "minishell: ambiguous redirect: `%s`\n", redir->ori_path);
		mshell->exit_code = 1;
		return (1);
	}
	if (ft_strchr(redir->ori_path, '$')
		&& ft_strchr(redir->file, ' ')
		&& !is_white_spaces_cmd(redir->file)
		&& !is_fully_quoted(redir->ori_path))
	{
		ft_printf_fd(2, "minishell: ambiguous redirect: `%s`\n", redir->ori_path);
		mshell->exit_code = 1;
		return (1);
	}
	return (0);
}

