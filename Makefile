NAME = ircserv
BONUS = feurbot

CC = c++
CFLAGS =	-Wall -Wextra -Werror -g \
			-std=c++98

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

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(NAME)

re: fclean all

$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS)

bonus: $(BONUS)

$(BONUS): $(BONUS_OBJS)
	$(CC) -o $@ $(BONUS_OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(BONUS_DIR)%.cpp 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re