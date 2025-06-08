#include "../../includes/shell.h"



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


