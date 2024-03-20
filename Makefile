NAME = ircserv
BONUS = feurbot

CC = g++
CFLAGS = -Wall -Wextra -Werror -g \
		 -std=c++98 -Iinclude

OBJ_DIR = obj/
SRC_DIR = src/
BONUS_DIR = bonus/

FILES = main Channel Client Server utils \
		commands/Invite commands/Join commands/Kick commands/List commands/Mode \
		commands/Nick commands/Part commands/Pass commands/Ping commands/Privmsg \
		commands/Topic commands/User
BONUS_FILES = feurbot

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(FILES)))
BONUS_SRCS = $(addprefix $(BONUS_DIR), $(addsuffix .cpp, $(BONUS_FILES)))

OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))
BONUS_OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(BONUS_FILES)))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) -o $@ $(OBJS)
	@echo "\033[32m✔ Creating executable...\033[37m"

bonus: $(BONUS_OBJS)
	$(CC) -o $(BONUS) $(BONUS_OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "\033[32m✔ Compiling $<...\033[37m"

$(OBJ_DIR)%.o: $(BONUS_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME) $(BONUS)

re: fclean all

.PHONY: all clean fclean re bonus
