SRCS_DIR = srcs

OBJS_DIR = objs

OBJSBONUS_DIR = objs_bonus

SRCSBONUS_DIR = srcs_bonus

SRCS	=	philo.c \
			check_philos.c \
			fork.c \
			free_exit.c \
			lst_utils.c \
			parsing.c \
			routine_utils.c \
			get_data.c \
			routine.c

SRCS_BONUS	=	check_philos_bonus.c \
				philo_bonus.c \
				routine_bonus.c \
				parsing_bonus.c \
				get_data_bonus.c \
				free_and_exit_bonus.c

OBJS = ${patsubst %.c,${OBJS_DIR}/%.o,${SRCS}}

OBJS_BONUS = ${patsubst %.c,${OBJSBONUS_DIR}/%.o,${SRCS_BONUS}}

HEADERS = philo.h

HEADERS_BONUS = philo_bonus.h

NAME = philo

NAME_BONUS = philo

CC = gcc

CFLAGS = -pthread -Werror -Wall -Wextra
# CFLAGS = -fsanitize=thread -pthread

# -- RULES -- #

${NAME}: ${OBJS_DIR} ${OBJS} ${HEADERS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	@echo "\033[32m$ ${NAME} compiled !"
	@echo "----------------------------\033[0m"

all: ${NAME}

$(OBJS_DIR):
	@mkdir -p ${OBJS_DIR}
	@echo "\033[33mCompiling Philo..."

${OBJSBONUS_DIR}:
	@mkdir -p ${OBJSBONUS_DIR}
	@echo "\033[33mCompiling Philo..."

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c
	@${CC} ${CFLAGS} -I. -c $< -o $@

${OBJSBONUS_DIR}/%.o: ${SRCSBONUS_DIR}/%.c
	@${CC} ${CFLAGS} -I. -c $< -o $@

clean:
	@rm -rf ${OBJSBONUS_DIR}
	@rm -rf ${OBJS_DIR}

fclean: clean
	@rm -f ${NAME} ${NAME_BONUS}

re: fclean all

bonus: ${OBJSBONUS_DIR} ${OBJS_BONUS} ${HEADERS_BONUS}
	@rm -rf ${OBJS_DIR}
	@${CC} ${CFLAGS} ${OBJS_BONUS} -o ${NAME_BONUS}
	@echo "\033[32mBonus compiled !"
	@echo "----------------------------\033[0m"

.PHONY:	all clean fclean re bonus