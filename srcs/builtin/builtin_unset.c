#include "../../includes/shell.h"

int builtin_unset(t_shell *mshell, char **token)
{
	size_t i;

	if (!mshell || !token || !token[0])
		return error_msg(mshell, "unset: internal error", 0);
	i = 1;
	while (token[i])
	{
		if (token[i][0] == '-')
		{
			ft_printf_fd(2, "minishell: unset: %s: invalid option\n", token[i]);
			mshell->exit_code = 2;
			return 2;
		}
		// if (ft_strcmp(token[i], "_") == 0)
		// {
		// 	i++;
		// 	continue;
		// }
		env_remove(mshell, token[i]);
		i++;
	}
	mshell->exit_code = 0;
	return 0;
}


