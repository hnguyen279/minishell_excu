/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:58:16 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 21:04:34 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	process_user_line(char *line, t_shell *mshell)
{
	if (ft_strcmp(line, "exit") == 0)
		return (FALSE);
	else if (ft_strcmp(line, "history") == 0)
	{
		print_working_history(mshell->history_head);
		return (TRUE);
	}
	else if (ft_strcmp(line, "history -c") == 0)
	{
		clear_working_history(&mshell->history_head);
		return (TRUE);
	}
	else
	{
		if (init_and_validate_input(line, mshell) == FALSE)
			return (TRUE); //not false
		process_valid_line(mshell);
		return (TRUE);
	}
}

int	init_and_validate_input(char *line, t_shell *mshell)
{
	if (validate_quote(line) == FALSE)
	{
		mshell->exit_code = 2;
		return (FALSE);
	}
	if (tokenization_expansion_validation(line, mshell) == FALSE 
			|| skip_expanded_empty_var(&mshell->token_list) == FALSE)
	{
		// free_token_list(*token_list);
		// *token_list = NULL; //clear loop in shell interactive
		return (FALSE);
	}
	return (TRUE);
}

int	tokenization_expansion_validation(char *line, t_shell *mshell)
{
	mshell->token_list = convert_user_input_to_token(line, mshell);
	if (!mshell->token_list)
	{
		mshell->exit_code = 1;
		return (FALSE);
	}
	// print_linked_list(mshell->token_list);
	if (validate_token(mshell->token_list) == FALSE)
	{
		mshell->exit_code = 2;
		return (FALSE);
	}
	// print_linked_list(mshell->token_list);//debug
	return (TRUE);
}
