/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:12:45 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 13:13:08 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	handle_word(char *line, t_token **token_list, int *i, t_shell *mshell)
{
	char	*extracted_str;
	char	*raw;

	raw = extract_ori_word(line, i);
	if (!raw)
		return (FALSE);
	extracted_str = extract_full_word(line, i, mshell);
	if (!extracted_str)
	{
		free_string(raw);
		print_error("Can't extract expanded word");
		return (FALSE);
	}
	if (create_word_token(token_list, &extracted_str, &raw) == FALSE)
		return (FALSE);
	return (TRUE);
}

int	create_word_token(t_token **token_list, char **extracted_str, char **raw)
{
	t_token	*new_token;

	new_token = NULL;
	new_token = create_token(*extracted_str, *raw, WORD);
	if (!new_token)
	{
		free_string(*raw);
		free_string(*extracted_str);
		return (FALSE);
	}
	add_token(token_list, new_token);
	free_string(*raw);
	free_string(*extracted_str);
	raw = NULL;
	extracted_str = NULL;
	return (TRUE);
}
