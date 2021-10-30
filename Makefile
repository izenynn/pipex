# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpoveda- <me@izenynn.com>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/24 19:08:53 by dpoveda-          #+#    #+#              #
#    Updated: 2021/10/24 19:08:54 by dpoveda-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COLORS
NOCOL=\033[0m
RED=\033[1;31m
YEL=\033[1;33m
ORG=\033[0;33m
GRN=\033[1;32m
DGRAY=\033[1;30m
BLU=\033[1;34m

# NAMES
NAME = pipex

# COMPILER
CC = gcc

CFLAGS = -Wall -Wextra -Werror
CFLAGS += -fsanitize=address
CFLAGS += -g3

# PATHS
SRC_PATH = src
OBJ_PATH = obj

# SOURCES
SRC_FILES = main.c utils.c ft_split.c
SRCB_FILES = bonus.c utils.c

SRC = $(addprefix $(SRC_PATH)/, $(SRC_FILES))
SRCB = $(addprefix $(SRC_PATH)/, $(SRCB_FILES))

OBJ_FILES = $(SRC_FILES:%.c=%.o)
OBJB_FILES = $(SRCB_FILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_FILES))
OBJB = $(addprefix $(OBJ_PATH)/, $(OBJB_FILES))

.PHONY: all bonus clean fclean re norm

all: $(NAME) $(CHECKER_NAME)

$(NAME): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

bonus: $(OBJB)
	$(CC) $^ -o $(NAME) $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH) 2> /dev/null

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean all

norm:
	@printf "\n${GRN}##########${YEL} NORMINETTE ${GRN}##########${NOCOL}\n"
	@printf "\n${GRN}SRC:${BLU}\n\n"
	@printf "${NOCOL}"
	@norminette $(SRC_PATH)
