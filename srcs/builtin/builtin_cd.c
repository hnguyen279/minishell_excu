
#include "../../includes/shell.h"

static void	update_pwd(t_shell *mshell)
{
	char		*cwd;
	const char	*oldpwd;

	oldpwd = env_find_value(mshell, "PWD");
	if (oldpwd)
		env_add(mshell, "OLDPWD", oldpwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		env_add(mshell, "PWD", cwd);
		free(cwd);
	}
	mshell->exit_code = 0;
}

static void	cd_to_home(t_shell *mshell)
{
	const char	*home;

	home = env_find_value(mshell, "HOME");
	if (!home)
	{
		ft_printf_fd(2, "minishell: cd: HOME not set\n");
		mshell->exit_code = 1;
		return ;
	}
	if (chdir(home) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", home);
		perror("");
		mshell->exit_code = 1;
		return ;
	}
	update_pwd(mshell);
}

static void	cd_to_tilde_path(t_shell *mshell, char *token)
{
	const char	*home;
	char		*path;

	home = env_find_value(mshell, "HOME");
	if (!home)
	{
		ft_printf_fd(2, "minishell: cd: HOME not set\n");
		mshell->exit_code = 1;
		return ;
	}
	path = ft_strjoin(home, token + 1);
	if (!path)
	{
		mshell->exit_code = 1;
		return ;
	}
	if (chdir(path) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: %s\n", path, strerror(errno));
		mshell->exit_code = 1;
		free(path);
		return ;
	}
	free(path);
	update_pwd(mshell);
}

static void	cd_to_oldpwd(t_shell *mshell)
{
	const char	*oldpwd;

	oldpwd = env_find_value(mshell, "OLDPWD");
	if (!oldpwd)
	{
		ft_printf_fd(2, "minishell: cd: OLDPWD not set\n");
		mshell->exit_code = 1;
		return ;
	}
	if (chdir(oldpwd) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", oldpwd);
		perror("");
		mshell->exit_code = 1;
		return ;
	}
	ft_printf_fd(1, "%s\n", oldpwd);
	update_pwd(mshell);
}

static void	cd_to_path(t_shell *mshell, char *path)
{
	if (chdir(path) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", path);
		perror("");
		mshell->exit_code = 1;
		return ;
	}
	update_pwd(mshell);
}

int	builtin_cd(t_shell *mshell, char **token)
{
	if (!mshell || !token)
	{
		mshell->exit_code = 1;
		return (mshell->exit_code);
	}
	if (token[1] && token[2])
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		mshell->exit_code = 1;
		return (mshell->exit_code);
	}
	if (!token[1] || token[1][0] == '\0' || ft_strcmp(token[1], "~") == 0
		|| ft_strcmp(token[1], "--") == 0)
		cd_to_home(mshell);
	else if (token[1][0] == '~')
		cd_to_tilde_path(mshell, token[1]);
	else if (ft_strcmp(token[1], "-") == 0)
		cd_to_oldpwd(mshell);
	else
		cd_to_path(mshell, token[1]);
	return (mshell->exit_code);
}
