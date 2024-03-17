NAME = ircserv
BONUS = feurbot

CC = g++
CFLAGS = -Wall -Wextra -Werror -g \
		 -std=c++98 -Iinclude

OBJ_DIR = obj/
SRC_DIR = src/
BONUS_DIR = bonus/

FILES = main Channel Client Reply Server utils
BONUS_FILES = feurbot

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(FILES)))
BONUS_SRCS = $(addprefix $(BONUS_DIR), $(addsuffix .cpp, $(BONUS_FILES)))

OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))
BONUS_OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(BONUS_FILES)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS)

bonus: $(BONUS_OBJS)
	$(CC) -o $(BONUS) $(BONUS_OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(BONUS_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME) $(BONUS)

re: fclean all

.PHONY: all clean fclean re bonus
