#include "../../includes/shell.h"

int builtin_env(t_shell *mshell, char **token)
{
	if (!mshell || !token || !token[0])
	{
		ft_printf_fd(2, "minishell: builtin_env: Invalid parameter(s)\n");
		return (1);
	}
	if (token[1])
	{
		if (token[1][0] == '-')
		{
			ft_printf_fd(2, "env: unrecognized option '%s'\n", token[1]);
			return (125);
		}
		ft_printf_fd(2, "env: '%s': No such file or directory\n", token[1]);
		return (127);
	}
	if (!env_find_value(mshell, "PATH"))
	{
		ft_putstr_fd("minishell: env: No such file or directory\n", 2);
		return (127);
	}
	env_print(mshell->envp);
	return (0);
}
