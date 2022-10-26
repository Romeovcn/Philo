OBJSBONUS_DIR = objs_bonus

SRCSBONUS_DIR = srcs_bonus

SRCS_BONUS	=	check_philos_bonus.c \
				philo_bonus.c \
				routine_bonus.c \
				parsing_bonus.c \
				get_data_bonus.c \
				free_and_exit_bonus.c

OBJS_BONUS = ${patsubst %.c,${OBJSBONUS_DIR}/%.o,${SRCS_BONUS}}

HEADERS_BONUS = philo_bonus.h

NAME = philo_bonus

CC = gcc

CFLAGS = -pthread -Werror -Wall -Wextra
# CFLAGS = -fsanitize=thread -pthread -Werror -Wall -Wextra

# ----------------------------------------------------------- #
# 							 RULES 		   					  #
# ----------------------------------------------------------- #

${NAME}: ${OBJSBONUS_DIR} ${OBJS_BONUS} ${HEADERS_BONUS}
	@${CC} ${CFLAGS} ${OBJS_BONUS} -o ${NAME}
	@echo "\033[32m$ ${NAME} compiled !"
	@echo "----------------------------\033[0m"

all: ${NAME}

${OBJSBONUS_DIR}:
	@mkdir -p ${OBJSBONUS_DIR}
	@echo "\033[33mCompiling ${NAME}..."

${OBJSBONUS_DIR}/%.o: ${SRCSBONUS_DIR}/%.c
	@${CC} ${CFLAGS} -I. -c $< -o $@

clean:
	@rm -rf ${OBJSBONUS_DIR}
	@echo "\033[32mClean !\033[0m"

fclean: clean
	@rm -f ${NAME}

re: fclean all

bonus: ${NAME}

.PHONY:	all clean fclean re bonus