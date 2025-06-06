/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:47:59 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 12:52:26 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_token	*convert_user_input_to_token(char *line)
{
	int		i;
	t_token	*token_list;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]) == TRUE)
			i++;
		else if (line[i] == '|')
			handle_pipe(&token_list, &i);
		else if (line[i] == '<')
			handle_in_heredoc(line, &token_list, &i);
		else if (line[i] == '>')
			handle_out_append(line,  &token_list, &i);
		else
		{
			if (handle_word(line, &token_list, &i) == FALSE)
				return (NULL);
		}
	}
	return (token_list);
}

int	handle_word(char *line, t_token **token_list, int *i)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*extracted_str;
	t_token	*new_token;
	
	in_single_quote = FALSE;
	in_double_quote = FALSE;
	extracted_str = extract_full_word(line, i, &in_single_quote, &in_double_quote);
	if (!extracted_str)
	{
		print_error("Can't extract word");
		return (FALSE);
	}
	new_token = create_token(extracted_str, WORD);
	new_token->in_single_quote = in_single_quote;
	new_token->in_double_quote = in_double_quote;
	add_token(token_list, new_token);
	free_string(extracted_str);
	in_single_quote = FALSE;
	in_double_quote = FALSE;
	return (TRUE);
}

char *extract_full_word(char *line, int *i, int *in_single_quote, int *in_double_quote)
{
	char	*result;
	char	*part;
	// char	*tmp;

	result = ft_strdup("");
	if (!result)
	{
		print_error("malloc error\n");
		return (NULL);
	}
	while (line[*i] && ft_isspace(line[*i]) == FALSE &&
		   line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		if (line[*i] == '$' && (line[*i + 1] == '\'' || line[*i + 1] == '"'))
		{
			(*i)++;
			part = handle_quote(line, i, in_single_quote, in_double_quote);
			// part = extract_quoted_token(line, i);
		}
		else if (line[*i] == '\'' || line[*i] == '"')
			part = handle_quote(line, i, in_single_quote, in_double_quote);
		else
		{
			part = extract_word(line, i);  // extract until quote or space or special char
			printf("extract from no quote %s\n", part);
		}
		if (!part)
		{
			free(result);
			return (NULL);
		}
		// tmp = result;
		// result = ft_strjoin(result, part);
		// free(tmp);
		result = str_join_result_and_free(result, part);
		// free(part);
	}
	return result;
}


char	*handle_quote(char *line, int *i, int *in_single_quote, int *in_double_quote)
{
	char	*part;
	
	part = NULL;
	if (line[*i] == '\'')
	{
		*in_single_quote = TRUE;
		part = extract_quoted_token(line, i);
		// *in_single_quote = FALSE;
		printf("extract from single quote %s\n", part);
	}
	else if (line[*i] == '"')
	{
		*in_double_quote = TRUE;
		part = extract_quoted_token(line, i);
		// *in_double_quote = FALSE;
		printf("extract from dboule quote %s\n", part);
	}
	if (!part)
		return (NULL);
	return (part);
}

char	*extract_quoted_token(char *line, int *i)
{
	int		start_pos;
	char	quote;
	char	*str;

	quote = line[*i];
	(*i)++;
	start_pos = *i;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (line[*i] != quote)
	{
		print_error("Unclosed quote");
		return (NULL);
	}
	str = ft_substr(line, start_pos, *i - start_pos);
	if (!str)
	{
		print_error("Malloc failed to substr");
		return (NULL);
	}
	
	// printf("print extracted quoted token %s\n", str); // to remove
	(*i)++;
	return (str);
}

// char	*expand_inside_quote()

char	*extract_word(char *line, int *i)
{
	int		start_pos;
	char	*extracted_str;

	start_pos = *i;
	while (line[*i])
	{
		if (ft_isspace(line[*i]) == TRUE || line[*i] == '\'' || line[*i] == '\"'
			|| line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
			break ;
		(*i)++;
	}
	extracted_str = ft_substr(line, start_pos, *i - start_pos);
	if (!extracted_str)
		return (NULL);
	return (extracted_str);
}

