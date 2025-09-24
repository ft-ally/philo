NAME=philo
HEADER = -I inc/
CC = cc -pthread
FLAGS = -Wall -Werror -Wextra
SRC_DIR = src/
OBJ_DIR = obj/


SRC_FILES = init.c main.c routine.c routine_utils.c monitor.c utils.c utils_str.c free_utils.c

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

all: $(NAME)

$(NAME): $(OBJF) $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "All files compiled!"

OBJF = $(OBJ_DIR)/.cache_exists

$(OBJF):
	@mkdir -p $(OBJ_DIR)
	@touch $(OBJF)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
	@echo "Compiling $<.."
	@$(CC) $(FLAGS) $(HEADER) -c $< -o $@

clean:
	@$(RM) -rf $(OBJ_DIR) $(OBJF)
	@echo "All object files removed."
	
fclean: clean
	@$(RM) -f $(NAME)
	@echo "All executable files removed."

re: fclean all
	@echo "Object + exec files have been cleaned and rebuilt."

.PHONY: all re clean fclean