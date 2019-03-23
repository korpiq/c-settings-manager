CFLAGS = -Wall -Werror -pedantic
INCLUDES = -I .

SRCS = settings_manager.c
OBJS = $(SRCS:.c=.o)

TEST_SRCS = settings_manager_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_TARGET = $(TEST_SRCS:.c=)

$(OBJS): $(SRCS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_TARGET) $(OBJS) $(TEST_OBJS)

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET)
