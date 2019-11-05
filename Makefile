# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/21 12:50:11 by rkirszba          #+#    #+#              #
#    Updated: 2019/06/03 15:50:13 by rkirszba         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIB_PATH = ./libft

SRC_PATH = src
SRC_NAME = minishell.c\
		   initialize_cmdtab_arrow.c\
		   scanner.c\
		   signal_and_input_manager.c\
		   tokens_functions.c\
		   errors.c\
		   errors_built_in.c\
		   free_manager.c\
		   env_finder.c\
		   env_initializer.c\
		   env_modifier.c\
		   executer.c\
		   execve_args.c\
		   word_interpreter.c\
		   built_in_echo_exit_unsetenv.c\
		   built_in_env.c\
		   built_in_setenv.c\
		   built_in_cd.c

HEADER_PATH = ./includes/

OBJ_PATH = obj
OBJ_NAME = $(SRC_NAME:.c=.o)

CC = gcc
CCFLAGS = -Wall\
		  -Wextra\
		  -Werror

CPPFLAGS = -I $(HEADER_PATH) -MMD
LIBFLAGS = -L $(LIB_PATH) -lft

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: lib $(NAME)

lib:
	@make -C libft/

$(NAME): $(OBJ)
	@make -C libft/
	$(CC) $(CCFLAGS) $(LIBFLAGS) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CCFLAGS) $(CPPFLAGS) -o $@ -c $<

$(OBJ_PATH):
	@mkdir $(OBJ_PATH) 2> /dev/null || true

clean:
	@make clean -C libft/
	@rm -f $(OBJ) $(OBJ:.o=.d)
	@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean: clean
	@make fclean -C libft/
	@rm -f $(NAME)

re: fclean
	$(MAKE) all

san: $(OBJ)
	make san -C libft/
	gcc -g3 -fsanitize=address -o $(NAME) $(OBJ) -I $(HEADER_PATH)

norme:
	norminette $(SRC)
	norminette $(HEADER_PATH)

.PHONY: lib make clean fclean re san

-include $(OBJ:.o=.d)
