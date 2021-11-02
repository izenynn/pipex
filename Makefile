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

# **************************************************************************** #
#                                     VARS                                     #
# **************************************************************************** #

# COLORS
NOCOL=\033[0m
RED=\033[1;31m
YEL=\033[1;33m
ORG=\033[0;33m
GRN=\033[1;32m
DGRAY=\033[1;30m
BLU=\033[1;34m

# **************************************************************************** #
#                                   PROGRAM                                    #
# **************************************************************************** #

NAME = pipex

# **************************************************************************** #
#                                   COMPILER                                   #
# **************************************************************************** #

CC = gcc

CFLAGS = -Wall -Wextra -Werror

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CFLAGS += -I inc

# **************************************************************************** #
#                                     LIBS                                     #
# **************************************************************************** #

CFLAGS += -I $(LFT_DIR)/inc

LDFLAGS = -L $(LFT_DIR)

LDLIBS = -lft

# **************************************************************************** #
#                                    PATHS                                     #
# **************************************************************************** #

SRC_PATH = src
OBJ_PATH = obj

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

SRC_FILES = main.c utils.c ft_split.c
SRCB_FILES = bonus.c utils.c

SRC = $(addprefix $(SRC_PATH)/, $(SRC_FILES))
SRCB = $(addprefix $(SRC_PATH)/, $(SRCB_FILES))

OBJ_FILES = $(SRC_FILES:%.c=%.o)
OBJB_FILES = $(SRCB_FILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_FILES))
OBJB = $(addprefix $(OBJ_PATH)/, $(OBJB_FILES))

# **************************************************************************** #
#                                ONLY DEV FLAGS                                #
# **************************************************************************** #

DEV ?= 0;

ifeq ($(DEV), 1)
	# common flags
	CFLAGS += -Wall -Werror -Wextra
	CFLAGS += -Wno-unknown-pragmas
	# check OS
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CFLAGS += -pedantic
		CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=bounds -fsanitize=null
		CFLAGS += -g3
	endif
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -pedantic
		CFLAGS += -fsanitize=address
		CFLAGS += -g3
	endif
endif

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

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
