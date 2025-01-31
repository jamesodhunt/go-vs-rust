OUT = c-example

default: $(OUT)

OBJECTS = main.o foo.o

$(OUT): $(OBJECTS)
	$(CC) -g -o $(OUT) $^

%.o: %.c
	$(CC) -I. -g -c $< -o $@

clean:
	rm -f $(OUT) $(OBJECTS)
