#include "../../includes/shell.h"

static size_t exchange_variable(char *str, int fd, t_shell *ms)
{
    size_t i = 0;
    char tmp;
    const char *val;

    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    tmp = str[i];
    str[i] = '\0';
    val = env_find_value(ms, str);
    str[i] = tmp;
    if (val)
        write(fd, val, ft_strlen(val));
    return i;
}

int is_fully_quoted(const char *str)
{
    size_t i;
    size_t end_i;
    char quote;
    size_t len;

    i = 0;
    if (!str || !str[0])
        return (0);
    while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
        i++;
    if (!str[i])
        return (0);
    if (str[i] != '\'' && str[i] != '"')
        return (0);
    quote = str[i];
    i++;
    len = ft_strlen(str);
    end_i = len - 1;
    while (end_i > i && (str[end_i] == ' ' || (str[end_i] >= 9 && str[end_i] <= 13)))
        end_i--;
    if (end_i < i || str[end_i] != quote)
        return (0);
    return (1);
}

char *get_delimiter(char *file)
{
	size_t i = 0;
    size_t end_i;
    size_t len;

	if (!file || !file[0]) 
        return NULL;
	while (file[i] && (file[i] == ' ' || (file[i] >= 9 && file[i] <= 13)))
		i++;
	len = strlen(file);
	if (i >= len)
		return ft_strdup("");
	end_i = len - 1;
	while (end_i > i && (file[end_i] == ' ' || (file[end_i] >= 9 && file[end_i] <= 13)))
		end_i--;
	if ((file[i] == '\'' || file[i] == '\"') && file[end_i] == file[i])
	{
		if (end_i > i + 1)
			return ft_substr(file, i + 1, end_i - i - 1);
		else
			return ft_strdup("");
	}
	return ft_substr(file, i, end_i - i + 1);
}

void exe_handle_dollar_expansion(char *input, int fd_write, t_shell *ms)
{
    size_t i = 0;
    size_t len;

    while (input[i] != '\0')
    {
        // ${VAR} expansion
        if (input[i] == '$' && input[i + 1] == '{')
        {
            size_t j = i + 2;
            while (input[j] && input[j] != '}')
                j++;
            if (input[j] == '}')
            {
                if (j == i + 2) // "${}" 
                {
                    write(fd_write, "$", 1);
                    i = j + 1;
                    continue;
                }
                char tmp = input[j];
                input[j] = '\0';
                len = exchange_variable(&input[i + 2], fd_write, ms);
                input[j] = tmp;
                i = j + 1;
                continue;
            }
            else
            {
                // if not '}' close → print '$' 
                write(fd_write, "$", 1);
                i++;
                continue;
            }
        }
        // special case: $?
        if (input[i] == '$' && input[i + 1] == '?')
        {
            ft_putnbr_fd(ms->exit_code, fd_write);
            i += 2;
            continue;
        }
        // $VAR or $_ (no braces)
        if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
        {
            len = exchange_variable(&input[i + 1], fd_write, ms);
            i += 1 + len;
            continue;
        }
        write(fd_write, &input[i], 1);
        i++;
    }
}


