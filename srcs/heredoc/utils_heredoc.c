#include "../../includes/shell.h"

static char *env_find_value_heredoc(const char *key, char **env)
{
    size_t key_len = ft_strlen(key);
    size_t i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
            return (env[i] + key_len + 1); //pointer to value (home/user) HOME=home/user
        i++;
    }
    return (NULL);
}

static size_t exchange_variable(char *str, int fd, char **env)
{
    size_t i;
    char tmp;
    const char *var;

    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    tmp = str[i];
    str[i] = '\0';
    var = env_find_value_heredoc(str, env); //env_find_value_heredoc
    str[i] = tmp;
    if (var)
    {
        size_t var_len = ft_strlen(var);
        write(fd, var, var_len);
    }
    return (i);
}

// void exe_handle_dollar_expansion(char *input, int fd_write, t_shell *ms)
// {
//     size_t i = 0;
//     size_t len;

//     while (input[i] != '\0')
//     {
//         if (input[i] == '$' && input[i + 1] == '{')
//         {
//             size_t j = i + 2;
//             while (input[j] && input[j] != '}')
//                 j++;
//             if (input[j] == '}')
//             {
//                 if (j == i + 2)
//                 {
//                     write(fd_write, "$", 1);
//                     i = j + 1;
//                     continue;
//                 }
//                 char tmp = input[j];
//                 input[j] = '\0';
//                 const char *varname = &input[i + 2];
//                 const char *val = env_find_value_heredoc(varname, ms->envp);
//                 input[j] = tmp;
//                 if (val)
//                     write(fd_write, val, strlen(val));
//                 i = j + 1;
//                 continue;
//             }
//             else
//             {
//                 write(fd_write, "$", 1);
//                 i++;
//                 continue;
//             }
//         }
//         if (input[i] == '$' && (ft_isdigit(input[i + 1]) || input[i + 1] == '?'))
//         {
//             if (input[i + 1] == '?')
//                 ft_putnbr_fd(ms->exit_code, fd_write);
//             i += 2;
//             continue;
//         }
//         if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
//         {
//             len = exchange_variable(&input[i + 1], fd_write, ms->envp);
//             i += 1 + len;
//             continue;
//         }
//         write(fd_write, &input[i], 1);
//         i++;
//     }
// }


int inside_quotes(const char *str)
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
    size_t i;
    size_t end_i;
    char *delimiter;
    size_t len;

    i = 0;
    if (!file || !file[0])
        return (NULL);
    while (file[i] && (file[i] == ' ' || (file[i] >= 9 && file[i] <= 13)))
        i++;
    if (!file[i])
        return (strdup(""));
    len = ft_strlen(file);
    end_i = len - 1;
    while (end_i > i && (file[end_i] == ' ' || (file[end_i] <= 9 && file[end_i] >= 13)))
        end_i--;
    if ((file[i] == '\'' || file[i] == '"') && file[end_i] == file[i])
    {
        i++;
        end_i--;
    }
    if (end_i < i)
    {
        return (ft_strdup(""));
    }
    delimiter = ft_substr(file, i, end_i - i + 1);
    return (delimiter);
}


//// new check
void exe_handle_dollar_expansion(char *input, int fd_write, t_shell *ms)
{
    size_t i = 0;
    size_t len;

    while (input[i] != '\0')
    {
        if (input[i] == '$' && input[i + 1] == '{')
        {
            size_t j = i + 2;
            while (input[j] && input[j] != '}')
                j++;
            if (input[j] == '}')
            {
                if (j == i + 2)
                {
                    write(fd_write, "$", 1);
                    i = j + 1;
                    continue;
                }
                char tmp = input[j];
                input[j] = '\0';
                const char *varname = &input[i + 2];
                const char *val = env_find_value(ms, varname);
                input[j] = tmp;
                if (val)
                {
                    size_t val_len = strlen(val);
                    write(fd_write, val, val_len);
                }
                i = j + 1;
                continue;
            }
            else
            {
                write(fd_write, "$", 1);
                i++;
                continue;
            }
        }
        if (input[i] == '$' && (ft_isdigit(input[i + 1]) || input[i + 1] == '?'))
        {
            if (input[i + 1] == '?')
                ft_putnbr_fd(ms->exit_code, fd_write);
            i += 2;
            continue;
        }
        if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
        {
            len = exchange_variable(&input[i + 1], fd_write, ms->envp);
            i += 1 + len;
            continue;
        }
        write(fd_write, &input[i], 1);
        i++;
    }
}

// static size_t exchange_variable(char *str, int fd, t_shell *ms)
// {
//     size_t i;
//     char tmp;
//     const char *var;

//     i = 0;
//     while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
//         i++;
//     tmp = str[i];
//     str[i] = '\0';
//     var = env_find_value(ms, str);
//     str[i] = tmp;
//     if (var)
//     {
//         size_t var_len = ft_strlen(var);
//         write(fd, var, var_len);
//     }
//     return (i);
// }