SRCS_DIR = srcs

OBJS_DIR = objs

SRCS	=	philo.c \
			check_philos.c \
			fork.c \
			free_exit.c \
			lst_utils.c \
			parsing.c \
			routine_utils.c \
			get_data.c \
			routine.c

OBJS = ${patsubst %.c,${OBJS_DIR}/%.o,${SRCS}}

HEADERS = philo.h

NAME = philo

CC = gcc

CFLAGS = -pthread -Werror -Wall -Wextra
# CFLAGS = -fsanitize=thread -pthread -Werror -Wall -Wextra

# ----------------------------------------------------------- #
# 							 RULES 		   					  #
# ----------------------------------------------------------- #

${NAME}: ${OBJS_DIR} ${OBJS} ${HEADERS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	@echo "\033[32m$ ${NAME} compiled !"
	@echo "----------------------------\033[0m"

all: ${NAME}

$(OBJS_DIR):
	@mkdir -p ${OBJS_DIR}
	@echo "\033[33mCompiling ${NAME}..."


${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c
	@${CC} ${CFLAGS} -I. -c $< -o $@

clean:
	@rm -rf ${OBJS_DIR}
	@echo "\033[32mClean !\033[0m"

fclean: clean
	@rm -f ${NAME}

re: fclean all

.PHONY:	all clean fclean re bonus