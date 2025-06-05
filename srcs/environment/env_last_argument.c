#include "../../includes/shell.h"

int	env_set_last_argument(t_shell *mshell, char **cmd)
{
	size_t	i;
	char	*last_arg;

	if (!mshell || !cmd || !cmd[0])
		return (1);
	i = 0;
	while (cmd[i])
		i++;
	if (i == 0)
		return (0);
	last_arg = cmd[i - 1];
	if (!last_arg[0] || strcmp(last_arg, "<") == 0 || strcmp(last_arg, ">") == 0
		|| strcmp(last_arg, "<<") == 0 || strcmp(last_arg, ">>") == 0)
	{
		return (0);
	}
	return (env_add(mshell, "_", last_arg));
}
