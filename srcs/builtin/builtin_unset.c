#include "../../includes/shell.h"

static int	is_valid_var_name(const char *name)
{
	size_t	i;

	i = 0;
	if (!name || !name[0] || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(t_shell *mshell, char **token)
{
	size_t	i;

	i = 1;
	if (!mshell || !token || !token[0])
		return (error_msg(mshell, "unset: internal error", 0));
	while (token[i])
	{
		if (token[i][0] == '-')
		{
			ft_printf_fd(2, "minishell: unset: %s: invalid option\n", token[i]);
			mshell->exit_code = 2;
			return (mshell->exit_code);
		}
		if (!is_valid_var_name(token[i]))
		{
			ft_printf_fd(2, "minishell: unset: `%s': not a valid identifier\n",
				token[i]);
			mshell->exit_code = 1;
			return (mshell->exit_code);
		}
		else
			env_remove(mshell, token[i]);
		i++;
	}
	mshell->exit_code = 0;
	return (mshell->exit_code);
}
