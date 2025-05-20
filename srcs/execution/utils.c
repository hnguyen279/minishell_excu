
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

int	display_error(char *cmd)
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
int check_white_spaces_cmd(char *cmd)
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

int check_ambiguous_redir(t_shell *mshell, t_redirect *redir)
{
	if (!redir || !redir->file || redir->file[0] == '\0')
	{
		ft_printf_fd(2, "minishell: ambiguous redirect\n");
		mshell->exit_code = 1;
		return (1);
	}
	if (ft_strchr(redir->file, ' ') && !check_white_spaces_cmd(redir->file))
	{
		ft_printf_fd(2, "minishell: ambiguous redirect: `%s`\n", redir->file);
		mshell->exit_code = 1;
		return (1);
	}
	return (0);
}
