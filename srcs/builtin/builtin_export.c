#include "../../includes/shell.h"

static int export_is_valid(const char *str)
{
    size_t i;

    if (!str || str[0] == '\0' || str[0] == '=' || ft_isdigit(str[0]) || str[0] == '+')
        return 0;
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

static void export_print(const char *env)
{
	const char *equal;

	equal = ft_strchr(env, '=');
	if (equal)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, env, equal - env + 1);
		ft_printf_fd(STDOUT_FILENO, "\"%s\"\n", equal + 1);
	}
	else
	{
		ft_printf_fd(STDOUT_FILENO, "declare -x %s\n", env);
	}
}
static char **copy_and_sort_envp(char **envp, size_t *out_count)
{
	size_t count;
	size_t i;
	char **copy;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: malloc failed\n");
		return NULL;
	}
	copy[count] = NULL;
	while (i < count)
	{
		copy[i] = envp[i];
		i++;
	}
	env_sort(copy, count);
	*out_count = count;
	return copy;
}

int export_standalone(char **envp)
{
	size_t count;
	size_t i;
	char **copy;

	count = 0;
	i = 0;
	copy = copy_and_sort_envp(envp, &count);
	if (!copy)
		return 1;
	while (i < count)
	{
		if (copy[i][0] != '_' || (copy[i][1] != '\0' && copy[i][1] != '='))
			export_print(copy[i]);
		i++;
	}
	free(copy);
	return 0;
}

static int export_plus_equal(t_shell *mshell, const char *arg, char *plus_equal)
{
	char *key;
	char *old_val;
	char *new_val; 

	key = ft_substr(arg, 0, plus_equal - arg);
	if (!key)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: malloc failed\n");
		return 1;
	}
	old_val = env_find_value(mshell, key);
	if (old_val)
		new_val = ft_strjoin(old_val, plus_equal + 2);
	else
		new_val = ft_strdup(plus_equal + 2);
	if (!new_val)
	{
		free(key);
		ft_printf_fd(STDERR_FILENO, "minishell: export: malloc failed\n");
		return 1;
	}
	env_remove(mshell, key);
	if (env_add(mshell, key, new_val) != 0)
	{
		free(key);
		free(new_val);
		return 1;
	}
	free(key);
	free(new_val);
	return 0;
}

static int export_with_equal(t_shell *mshell, const char *arg, char *equal)
{
	char *key = ft_substr(arg, 0, equal - arg);
	if (!key)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: malloc failed\n");
		return 1;
	}

	env_remove(mshell, key);
	if (env_add(mshell, key, equal + 1) != 0)
	{
		free(key);
		return 1;
	}
	free(key);
	return 0;
}

int export_handle_one(t_shell *mshell, const char *arg)
{
	char *equal;
	char *plus_equal;

	if (!arg)
		return 0;
	if (arg[0] == '-')
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: `%s`: invalid option\n", arg);
		return 2;
	}
	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_equal != NULL)
	{
		char *key = ft_substr(arg, 0, plus_equal - arg);
		if (!key || !export_is_valid(key))
		{
			free(key);
			ft_printf_fd(STDERR_FILENO, "minishell: export: `%s`: not a valid identifier\n", arg);
			return 1;
		}
		free(key);
		return export_plus_equal(mshell, arg, plus_equal);
	}	
	if (!export_is_valid(arg))
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: `%s`: not a valid identifier\n", arg);
		return 1;
	}
	equal = ft_strchr(arg, '=');
	if (equal != NULL)
		return export_with_equal(mshell, arg, equal);
	else if (env_add(mshell, arg, NULL) != 0)
		return 1;
	return 0;
}

static int handle_export_arguments(t_shell *mshell, char **token, int argc)
{
	int i;
	int code;
	int result;

	i = 1;
	code = 0;
	while (i < argc)
	{
		result = export_handle_one(mshell, token[i]);
		if (result > code)
			code = result;
		i++;
	}
	return code;
}

void builtin_export(t_shell *mshell, char **token)
{
	int argc;

	argc = 0;
	if (!mshell || !token || !token[0])
	{
		ft_putstr_fd("minishell: export: invalid input\n", STDERR_FILENO);
		mshell->exit_code = 1;
		return;
	}
	while (token[argc])
		argc++;
	if (argc == 1)
	{
		mshell->exit_code = export_standalone(mshell->envp);
		return;
	}
	mshell->exit_code = handle_export_arguments(mshell, token, argc);
}
