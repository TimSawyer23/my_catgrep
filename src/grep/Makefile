NAME = s21_grep

SRC = *.c

OBJS = *.o

all: $(NAME)

$(NAME):
	gcc -c -Wall -Wextra -Werror -std=c11 $(SRC)
	gcc $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) *.log

fclean: clean
	rm -f $(NAME)

re: fclean all

tests:
	./tests.sh

test_leak:
	./test_leaks.sh