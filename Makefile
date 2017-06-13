IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)
FLAGS=-g -Wall

LDIR =./lib

LIBS=-pthread

_DEPS = parser.c operations_queue.c stock_market_lib.c concurrency_layer.c
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = parser.o operations_queue.o stock_market_lib.o concurrency_layer.o
OBJ = $(patsubst %,$(LDIR)/%,$(_OBJ))


$(LDIR)/%.o: $(LDIR)/%.c
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

concurrent_market: concurrent_market.c $(OBJ)
	gcc $(FLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(LDIR)/*.o concurrent_market
