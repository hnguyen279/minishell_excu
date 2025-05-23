/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:47:59 by trpham            #+#    #+#             */
/*   Updated: 2025/05/22 15:25:28 by trpham           ###   ########.fr       */
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
	t_token	*tokenized_input_list = NULL;
	char	*extracted_str;


	if (is_comment(line) == 0)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]) == TRUE)
			i++;
		else if (line[i] == '|')
		{
			new_token =	create_token("|", PIPE);
			add_token(&tokenized_input_list, new_token);
			i++;
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				new_token = create_token("<<", REDIR_HEREDOC_token);
				i += 2;
			}
			else
			{
				new_token = create_token("<", REDIR_IN_token);
				i++;
			}
			add_token(&tokenized_input_list, new_token);
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				new_token = create_token(">>", REDIR_APPEND_token);
				i += 2;
			}
			else
			{
				new_token = create_token(">", REDIR_OUT_token);
				i++;
			}
			add_token(&tokenized_input_list, new_token);
		}
		else if (line[i] == '\'' || line[i] == '\"')
		{
			extracted_str = extract_quoted_token(line, &i);
			// printf("extracted str: %s\n", extracted_str);
			if (!extracted_str)
				return (NULL);
			new_token = create_token(extracted_str, WORD);
			add_token(&tokenized_input_list, new_token);
			free_string(extracted_str);
		}
		else
		{
			extracted_str = extract_word(line, &i);
			new_token = create_token(extracted_str, WORD);
			add_token(&tokenized_input_list, new_token);
			free_string(extracted_str);
		}
	}
	// print_linked_list(tokenized_input_list);
	// validate_token(tokenized_input_list);
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
	str = ft_substr(line, start_pos, *i - start_pos);
	if (!str)
	{
		get_error_msg(ERR_MALLOC);
		return (NULL);
	}
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
			|| line[*i] == '<' || line[*i] == '>'|| line[*i] == '|') //add | pipe
			break;
		(*i)++;
	}
	extracted_str = ft_substr(line, start_pos, *i - start_pos);
	if (!extracted_str)
		return (NULL);
	return (extracted_str);
}
