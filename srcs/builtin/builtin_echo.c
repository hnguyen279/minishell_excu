#include "../../includes/shell.h"

static int	should_suppress_newline(char *arg)
{
	int	j;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	j = 2;
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

int	builtin_echo(t_shell *mshell, char **token)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (token[i])
	{
		if (!should_suppress_newline(token[i]))
			break ;
		newline = 0;
		i++;
	}
	while (token[i])
	{
		write(STDOUT_FILENO, token[i], ft_strlen(token[i]));
		if (token[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	mshell->exit_code = 0;
	return (mshell->exit_code);
}
