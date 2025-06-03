/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:47:59 by trpham            #+#    #+#             */
/*   Updated: 2025/06/03 13:36:32 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
	Go through input character by character
	- Inside quote: extract quotet token
	- Outside quote: handle token and meta
	Split user input by space and categorize the input type.
	Each token includes string value and its type (command, operators, etc.)
	-> tokenization
*/
t_token	*convert_user_input_to_token(char *line)
{
	int		i;
	t_token	*new_token;
	t_token	*tokenized_input_list;
	char	*extracted_str;
	int		in_single_quote = FALSE;
	int		in_double_quote = FALSE;

	tokenized_input_list = NULL;
	// is it necessary to test the comment case, if not then can remove function is_comment
	// if (is_comment(line) == 0)
	// 	return (NULL);
	i = 0;
	while (line[i])
	{
		// printf("input letter %c\n", line[i]);
		if (ft_isspace(line[i]) == TRUE)
			i++;
		else if (line[i] == '|')
		{
			new_token = create_token("|", PIPE);
			add_token(&tokenized_input_list, new_token);
			i++;
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				new_token = create_token("<<", REDIR_HEREDOC_TOKEN);
				i += 2;
			}
			else
			{
				new_token = create_token("<", REDIR_IN_TOKEN);
				i++;
			}
			add_token(&tokenized_input_list, new_token);
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				new_token = create_token(">>", REDIR_APPEND_TOKEN);
				i += 2;
			}
			else
			{
				new_token = create_token(">", REDIR_OUT_TOKEN);
				i++;
			}
			add_token(&tokenized_input_list, new_token);
		}
		else
		{
			extracted_str = extract_full_word(line, &i, &in_single_quote, &in_double_quote);
			if (!extracted_str)
			{
				print_error("Can't extract word");
				return (NULL);
			}
			// printf("extracted-str %s\n", extracted_str);
			new_token = create_token(extracted_str, WORD);
			// printf("set single quote: %d\n", in_single_quote);
			// printf("set double quote: %d\n", in_double_quote);

			new_token->in_single_quote = in_single_quote;
			new_token->in_double_quote = in_double_quote;
			add_token(&tokenized_input_list, new_token);
			free_string(extracted_str);
			in_single_quote = FALSE;
			in_double_quote = FALSE;
		}
		
	}
	// print_linked_list(tokenized_input_list);
	return (tokenized_input_list);
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
	// printf("print extracted quoted token %d\n", *i);
	str = ft_substr(line, start_pos, *i - start_pos);
	if (!str)
	{
		print_error("Malloc failed to substr");
		return (NULL);
	}
	// printf("print extracted quoted token %s\n", str);
	(*i)++;
	return (str);
}

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

char *extract_full_word(char *line, int *i, int *in_single_quote, int *in_double_quote)
{
	char *result;
	char *part;

	// printf("enter extract full word function\n");
	result = ft_strdup("");
	if (!result)
	{
		print_error("malloc error\n");
		return (NULL);
	}
	while (line[*i] && ft_isspace(line[*i]) == FALSE &&
		   line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		// printf("letter %c\n", line[*i]);
		if (line[*i] == '$' && (line[*i + 1] == '\'' || line[*i + 1] == '"'))
		{
			
			// printf("enter here");
			(*i)++;
			part = extract_quoted_token(line, i);
			// printf("letter %s\n", part);
		}
		else if (line[*i] == '\'' || line[*i] == '\"')
		{
			if (line[*i] == '\'')
				*in_single_quote = TRUE;
			else
				*in_double_quote = TRUE;
			part = extract_quoted_token(line, i);
			
		}
		else
			part = extract_word(line, i);  // extract until quote or space or special char
		if (!part)
		{
			free(result);
			return (NULL);
		}
		// printf("extracted part %s\n:", part);
		// char *tmp = result;
		result = ft_strjoin(result, part);
		// printf("join with result %s\n:", result);

		// free(tmp);
		free(part);
		// (*i)++;
	}
	if (result[0] == '\0') // Handle empty result
    {
        free(result);
        return NULL;
    }
	return result;
}
