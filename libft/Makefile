# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/21 13:30:04 by cagonzal          #+#    #+#              #
#    Updated: 2022/07/06 11:59:26 by cagonzal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	file's name
NAME = libft.a

#	files
SRCS = \
	file/ft_putstr_fd.c file/ft_putendl_fd.c file/ft_putnbr_fd.c file/ft_putchar_fd.c \
	file/get_next_line.c \
	\
	lst/ft_lstadd_after.c lst/ft_lstadd_back.c  lst/ft_lstadd_front.c lst/ft_lstclear.c \
	lst/ft_lstdelone.c lst/ft_lstiter.c lst/ft_lstlast.c lst/ft_lstmap.c lst/ft_lstnew.c \
	lst/ft_lstsize.c \
	\
	mem/ft_memset.c mem/ft_bzero.c mem/ft_calloc.c mem/ft_memchr.c mem/ft_memcmp.c \
	mem/ft_memcpy.c mem/ft_memcpy.c mem/ft_memmove.c mem/ft_freetable.c \
	\
	nbr/ft_atoi_base.c nbr/ft_atoi.c nbr/ft_itoa.c \
	\
	others/ft_isprint.c others/ft_isdigit.c others/ft_isalnum.c others/ft_isalpha.c \
	others/ft_isascii.c others/ft_printf.c others/ft_swap.c others/ft_error.c \
	\
	str/ft_strjoin.c str/ft_strdup.c str/ft_tolower.c str/ft_strtrim.c str/ft_strlcat.c \
	str/ft_substr.c str/ft_strcpy.c str/ft_strlcpy.c str/ft_strlen.c str/ft_toupper.c \
	str/ft_strmapi.c str/ft_split.c str/ft_strncmp.c str/ft_strndup.c str/ft_strchr.c \
	str/ft_strichr.c str/ft_strnstr.c str/ft_strrchr.c str/ft_striteri.c str/ft_strtrim.c \
	str/ft_strcmp.c

OBJS = ${SRCS:.c=.o}

#	Flags
CFLAGS = -Wall -Werror -Wextra

#	Options to create the library
AR = ar -rc

#	Options to force the remove
RM = rm -f

#	Compilation
CC = gcc

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

#	Execute make
$(NAME): ${OBJS}
	@echo $(CURSIVE)$(GRAY)"		- Generating $(NAME)..." $(NONE)
	@${AR} ${NAME} ${OBJS}
	@echo $(CURSIVE)$(GRAY)"		- $(NAME) compiled..." $(NONE)

all:	${NAME}

clean:
	@echo $(CURSIVE)$(GRAY)"		- Removing libft object files"... $(NONE)
	@${RM} ${OBJS}

fclean:	clean
	@echo $(CURSIVE)$(GRAY)"		- Removing $(NAME)..." $(NONE)
	@${RM} ${NAME}

re:		fclean all

.PHONY:	all clean fclean re