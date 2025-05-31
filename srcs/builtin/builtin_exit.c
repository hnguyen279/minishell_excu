#include "../../includes/shell.h"
#include <limits.h> // LONG_MAX, LONG_MIN

int ft_atol_safe(const char *str, long *out)
{
	int sign = 1;
	size_t i = 0;
	unsigned long result = 0;
	unsigned long limit = LONG_MAX;

	if (!str || !out)
		return 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			sign = -1;
			limit = (unsigned long)LONG_MAX + 1;
		}
		i++;
	}
	if (!ft_isdigit(str[i]))
		return 0;
	while (ft_isdigit(str[i]))
	{
		int digit = str[i] - '0';
		if (result > (limit - digit) / 10)
			return 0; // overflow
		result = result * 10 + digit;
		i++;
	}
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] != '\0')
		return 0; 
	*out = (long)(sign * result);
	return 1;
}


int to_valid_exit_code(long num)
{
	num %= 256;
	if (num < 0)
		num += 256;
	return (int)num;
}


void builtin_exit(t_shell *mshell, char **token)
{
	int argc = 0;
	long code;

	while (token[argc])
		argc++;

	ft_putstr_fd("exit\n", STDOUT_FILENO);

	if (argc > 2)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: exit: too many arguments\n");
		mshell->exit_code = 1;
		return;
	}
	else if (argc == 2)
	{
		if (!ft_atol_safe(token[1], &code))
		{
			ft_printf_fd(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", token[1]);
			mshell->exit_code = 2;
		}
		else
		{
			mshell->exit_code = to_valid_exit_code(code);
		}
	}

	free_ast(mshell->ast, mshell);
	mshell->ast = NULL;
	shell_cleanup(mshell);
	exit(mshell->exit_code);
}
