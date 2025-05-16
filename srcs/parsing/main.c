/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:57:54 by trpham            #+#    #+#             */
/*   Updated: 2025/05/14 07:18:50 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	main(int ac, char *av[], char *env[])
{

	if (ac != 1 || !env || !*env)
		return (EXIT_FAILURE);
	(void) av;
	shell_interactive();
	
	return (0);	
}

/* Print promt to stdout, readlline to get the text after promt and remove \n at the end.
	Call split function to separate words and operators.
	Type exit to exit the prompt.
*/
void	shell_interactive(void)
{
	char	*line;
	static t_token	*history_head;
	t_token	*tokenized_input_list = NULL;
	t_cmd	*cmd_list = NULL;

	history_head = NULL;
	while (1)
	{
		line =	readline("minishell$ ");
		if (!line)
			return;
		if (ft_strcmp(line, "") != 0)
		{
			store_history(line, &history_head);
			if (ft_strcmp(line, "exit") == 0)
			{
				free_string(line);
				return;
			}
			else if (ft_strcmp(line, "history") == 0)
				print_working_history(history_head);
			else if (ft_strcmp(line, "history -c") == 0)
				clear_working_history(&history_head);
			else if (validate_quote(line) != -1)
			{
				tokenized_input_list = convert_user_input_to_token(line);
				if (!tokenized_input_list)
					return ;
				// print_linked_list(tokenized_input_list);
				if (validate_token(tokenized_input_list) == TRUE)
				{
					cmd_list = parse_tokens_to_commands(tokenized_input_list);
					if (!cmd_list)
						printf("Parse tokens to commands failed\n");
					else
						print_cmd_list(cmd_list);
				}	
					
			}
		}
		free_string(line);
	}
}

// void	validate_cmd(t_cmd *cmd_list)
// {
	
// }