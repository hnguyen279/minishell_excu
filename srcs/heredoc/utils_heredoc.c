#include "../../includes/shell.h"


int	is_fully_quoted(const char *str)
{
	size_t	i;
	size_t	end_i;
	char	quote;
	size_t	len;

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
	while (end_i > i && (str[end_i] == ' ' || (str[end_i] >= 9
				&& str[end_i] <= 13)))
		end_i--;
	if (end_i < i || str[end_i] != quote)
		return (0);
	return (1);
}

// char	*get_delimiter(char *file)
// {
// 	size_t	i;
// 	size_t	end_i;
// 	size_t	len;

// 	i = 0;
// 	if (!file || !file[0])
// 		return (NULL);
// 	while (file[i] && (file[i] == ' ' || (file[i] >= 9 && file[i] <= 13)))
// 		i++;
// 	len = strlen(file);
// 	if (i >= len)
// 		return (ft_strdup(""));
// 	end_i = len - 1;
// 	while (end_i > i && (file[end_i] == ' ' || (file[end_i] >= 9
// 				&& file[end_i] <= 13)))
// 		end_i--;
// 	if ((file[i] == '\'' || file[i] == '\"') && file[end_i] == file[i])
// 	{
// 		if (end_i > i + 1)
// 			return (ft_substr(file, i + 1, end_i - i - 1));
// 		else
// 			return (ft_strdup(""));
// 	}
// 	return (ft_substr(file, i, end_i - i + 1));
// }


