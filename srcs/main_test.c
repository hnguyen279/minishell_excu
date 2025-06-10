
// #include "../includes/shell.h"

// void	shell_interactive(t_shell *mshell)
// {
// 	char		*line;
// 	t_token		*history_head = NULL;

// 	while (1)
// 	{
// 		g_signum = 0;

// 		if (isatty(fileno(stdin)))
// 			line = readline("");
// 		else
// 		{
// 			char *raw_line = get_next_line(fileno(stdin)); 

// 			if (!raw_line)
// 				break ;
// 			line = ft_strtrim(raw_line, "\n");
// 			free(raw_line);
// 		}

// 		if (g_signum)  // Signal interrupt (e.g., Ctrl+C)
// 			sig_exit_code(mshell);

// 		if (!line)  // Ctrl+D or EOF
// 			break ;

// 		if (line[0] == '\0')  // Empty line (Enter key)
// 		{
// 			free_string(line);
// 			continue ;
// 		}

// 		store_history(line, &history_head);

// 		if (handle_special_command_line(line, &history_head) == TRUE &&
// 			ft_strcmp(line, "exit") == 0)
// 		{
// 			free_string(line);
// 			break ;
// 		}
// 		else
// 			handle_line(line, mshell);

// 		free_string(line);
// 	}
// 	clear_working_history(&history_head);
// 	// printf("exit\n");  <-- REMOVE OR COMMENT OUT for tester compatibility
// }