# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/15 12:52:46 by cagonzal          #+#    #+#              #
#    Updated: 2022/07/06 11:59:41 by cagonzal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = gcc

CFLAGS = -Werror -Wall -Wextra#-fsanitize=address

RM = rm -rf

#	files
LIBFT_A = libft.a
LIBF_DIR = libft/
LIBFT  = $(addprefix $(LIBF_DIR), $(LIBFT_A))

SRC = src/pipex.c

SRCB = src_bonus/pipex_bonus.c src_bonus/utils_bonus.c \

OBJ = ${SRC:.c=.o}

OBJB = ${SRCB:.c=.o}

# Color Aliases
NONE	= '\033[0m'
DEFAULT	= '\033[0;39m'
GRAY	= '\033[2;37m'
RED		= '\033[0;91m'
GREEN	= '\033[32m'
YELLOW	= '\033[33m'
BLUE	= '\033[0;94m'
MAGENTA	= '\033[0;95m'
CYAN	= '\033[0;96m'
WHITE	= '\033[0;97m'
CURSIVE	= '\033[3m'

.c.o:
	@${CC} ${CFLAGS} -g -c $< -o ${<:.c=.o}

all: $(NAME)

$(NAME): $(OBJ)
	@echo $(CURSIVE)$(GRAY) "	- Making libft..." $(NONE)
	@make -C $(LIBF_DIR)
	@echo $(CURSIVE)$(GRAY) "	- Compiling $(NAME)..." $(NONE)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(INCLUDE) -o $(NAME)
	@echo $(GREEN)"- Compiled -"$(NONE)

bonus: $(OBJB)
	@echo $(CURSIVE)$(GRAY) "	- Making libft..." $(NONE)
	@make -C $(LIBF_DIR)
	@echo $(CURSIVE)$(GRAY) "	- Compiling $(NAME)..." $(NONE)
	@$(CC) $(CFLAGS) $(OBJB) $(LIBFT) $(INCLUDE) -o $(NAME)
	@echo $(GREEN)"- Compiled -"$(NONE)

clean:
	@echo $(CURSIVE)$(GRAY) "	- Removing object files..." $(NONE)
	@rm -rf $(OBJ)
	@make -C $(LIBF_DIR) clean

fclean: clean
	@echo $(CURSIVE)$(GRAY) "	- Removing $(NAME)..." $(NONE)
	@rm -rf $(NAME) $(OBJB)
	@make -C $(LIBF_DIR) fclean
	@echo $(GREEN)"- Cleaned -"$(NONE)

re: fclean all