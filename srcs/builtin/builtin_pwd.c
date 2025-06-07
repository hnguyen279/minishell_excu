#include "../../includes/shell.h"

static int parse_pwd_args(char **token, t_shell *mshell)
{
	int i = 1;

	while (token[i])
	{
		if (token[i][0] == '-' && token[i][1] != '\0')
		{
			ft_printf_fd(2, "minishell: pwd: options aren't supported\n");
			ft_printf_fd(2, "pwd: usage: pwd\n");
			mshell->exit_code = 2;
			return (0);
		}
		i++;
	}
	return (1);
}

int builtin_pwd(t_shell *mshell, char **token)
{
	char *cwd;
	const char *env_pwd;

	if (!parse_pwd_args(token, mshell))
		return (mshell->exit_code);

	env_pwd = env_find_value(mshell, "PWD");
	if (env_pwd)
	{
		printf("%s\n", env_pwd);
		mshell->exit_code = 0;
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: getcwd");
		mshell->exit_code = 1;
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	mshell->exit_code = 0;
	return (0);
}
