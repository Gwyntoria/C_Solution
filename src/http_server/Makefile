EXECUTABLE:=http_server

CC := gcc

CFLAGS += -Wall -g -lpthread
# CFLAGS += -Werror
# CFLAGS += -I$(INC_DIR)

# LDLIBS=-lm
# LDFLAGS=-L/usr/local/include

SRCS := $(wildcard *.c)

# OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXECUTABLE)

echo:
	@echo $(OBJS)