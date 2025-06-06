#include "../../includes/shell.h"

static void	sign_and_limit(const char *str, size_t *i, int *sign,
		unsigned long *limit)
{
	*sign = 1;
	*limit = LONG_MAX;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
		{
			*sign = -1;
			*limit = (unsigned long)LONG_MAX + 1;
		}
		(*i)++;
	}
}

static int	digits_with_overflow(const char *str, size_t *i,
		unsigned long *result, unsigned long limit)
{
	int	digit;

	*result = 0;
	while (ft_isdigit(str[*i]))
	{
		digit = str[*i] - '0';
		if (*result > (limit - digit) / 10)
			return (0); // overflow
		*result = *result * 10 + digit;
		(*i)++;
	}
	return (1);
}

int	ft_atol_safe(const char *str, long *out)
{
	size_t			i;
	int				sign;
	unsigned long	result;
	unsigned long	limit;

	i = 0;
	if (!str || !out)
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	sign_and_limit(str, &i, &sign, &limit);
	if (!ft_isdigit(str[i]))
		return (0);
	if (!digits_with_overflow(str, &i, &result, limit))
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] != '\0')
		return (0);
	*out = (long)(sign * result);
	return (1);
}

int	to_valid_exit_code(long num)
{
	num %= 256;
	if (num < 0)
		num += 256;
	return ((int)num);
}

static void	handle_exit_with_argument(t_shell *mshell, const char *arg)
{
	long	code;

	if (!ft_atol_safe(arg, &code))
	{
		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n",
			arg);
		mshell->exit_code = 2;
	}
	else
		mshell->exit_code = to_valid_exit_code(code);
}

void	builtin_exit(t_shell *mshell, char **token)
{
	int	argc;

	argc = 0;
	while (token[argc])
		argc++;
	if (!mshell->has_pipe)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argc > 2)
	{
		ft_printf_fd(2, "minishell: exit: too many arguments\n");
		mshell->exit_code = 1;
		return ;
	}
	else if (argc == 2)
		handle_exit_with_argument(mshell, token[1]);
	free_ast(mshell->ast, mshell);
	mshell->ast = NULL;
	shell_cleanup(mshell);
	exit(mshell->exit_code);
}
