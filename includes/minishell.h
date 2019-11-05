/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 18:14:27 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 16:48:32 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <sys/stat.h>
# include <signal.h>
# include "libft.h"

# define BUFF_SIZE_MS 1023
# define GRN "\x1B[01;32m"
# define RED "\x1B[01;31m"
# define YEL "\x1B[01;33m"
# define WHI "\x1B[47;30m"
# define RESET "\x1B[0m"

typedef enum		e_lex
{
	word,
	carriage,
	semicolon,
}					t_lex;

typedef struct		s_reader
{
	char			*input;
	int				ret;
	int				nb_chars;
	int				cursor;
}					t_reader;

typedef struct		s_token
{
	t_lex			lexem;
	char			*value;
	struct s_token	*next;
	char			quote;
}					t_token;

typedef struct		s_envi
{
	char			**environ;
	char			**paths;
	char			*ppwd;
	char			*apwd;
	char			*home;
	int				ret;
	int				in_env;
}					t_envi;

typedef struct		s_built_in
{
	char			*cmd;
	int				(*execute)(t_token*, t_envi*, struct s_built_in*);
}					t_built_in;

extern int			g_errno;
extern t_envi		*g_envi;

int					input_manager(t_reader *reader);
void				handle_signal(int signum);
int					print_prompt(t_envi *envi);

t_built_in			*initialize_tab_cmd(void);
void				initialize_arrow(char *arrow);

int					scanner(t_reader *reader, t_envi *envi, t_token **tokens);

int					executer(t_envi *envi, t_token *tokens,\
					t_built_in *tab_cmd);
int					cmd_dispatcher(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);
int					word_interpreter(t_token *token, t_envi *envi);
char				*concat_binary_path(char *path, char *cmd);
char				**give_args(t_token *token);

int					execute_echo(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);
int					execute_exit(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);
int					execute_env(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);
int					execute_setenv(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);
int					execute_unsetenv(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);
int					execute_cd(t_token *token, t_envi *envi,\
					t_built_in *tab_cmd);

t_token				*create_token(t_lex lex, t_reader *reader, int start,\
					int len);
void				append_token(t_token **tokens, t_token *token);

int					print_sys_error(void);
int					print_cmd_error(char *not_found);
int					print_var_error(char *value);
int					print_home_error(void);
int					print_env_cmd_error(char *not_found, int ret);
int					print_setenv_arg_error(int errnum);
int					print_unsetenv_arg_error(void);
int					print_cd_error(int errnum, char *name);
int					print_access_error(char *binary, int in_env);
int					print_exit_error(int errnum);

char				*find_line_env(char **environ_cp, char *name, int len);
int					find_index_env(char **environ_cp, char *name, int len);
char				*find_path(char *to_find, char **paths);

char				**get_paths(char **environ_cp);
char				*find_wkdir_name(void);
t_envi				*initialize_environ(void);
t_envi				*envi_dup(t_envi *envi);

int					count_arguments(t_token *token);

int					env_modifier(char *new_value, t_envi *envi);
int					append_line_env(t_envi *envi, char *line);
int					substitute_line_env(t_envi *envi, int index,\
					char *new_line);
int					remove_line_env(t_envi *envi, int index);
int					actualize_dirs(char *name, t_envi *envi);
int					print_env(char **environ_cp, t_envi *envi);

void				free_manager(t_envi *envi, char *input, t_token **tokens,\
					t_built_in *tab_cmd);
void				free_environ(t_envi *envi);
void				free_tokens(t_token **tokens);
void				free_token(t_token *token);
void				free_tab_cmd(t_built_in *tab_cmd);

#endif
