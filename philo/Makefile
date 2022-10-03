SRCS_DIR = srcs

OBJS_DIR = objs

SRCSBONUS_DIR = srcs_bonus

SRCS	=	philo.c \
			get_table.c \
			fork.c

SRCS_BONUS =

OBJS = ${patsubst %.c,${OBJS_DIR}/%.o,${SRCS}}

OBJS_BONUS = ${patsubst %.c,${OBJS_DIR}/%.o,${SRCS_BONUS}}

HEADERS = philo.h

HEADERS_BONUS = 

NAME = philo

NAME_BONUS = pipex

CC = gcc

CFLAGS = -pthread
# CFLAGS = -fsanitize=thread -pthread

# -- RULES -- #

${NAME}: ${OBJS_DIR} ${OBJS} ${HEADERS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	@echo "\033[32m$ ${NAME} compiled !"
	@echo "----------------------------\033[0m"

all: ${NAME}

$(OBJS_DIR):
	@mkdir ${OBJS_DIR}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c
	@${CC} ${CFLAGS} -I. -c $< -o $@

${OBJS_DIR}/%.o: ${SRCSBONUS_DIR}/%.c
	@${CC} ${CFLAGS} -I. -c $< -o $@

clean:
	@rm -rf ${OBJS_DIR}

fclean: clean
	@rm -f ${NAME} ${NAME_BONUS}

re: fclean all

bonus: ${OBJS_DIR} ${OBJS_BONUS} ${HEADERS}
	@${CC} ${CFLAGS} ${OBJS_BONUS} -o ${NAME_BONUS}
	@echo "\033[32mBonus compiled !"
	@echo "----------------------------\033[0m"

.PHONY:	all clean fclean re bonus