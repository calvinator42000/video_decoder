CC := gcc
CFLAGS := -Wall -I./inc
CPPFLAGS := 
LDFLAGS := 

src := $(wildcard src/*.c)
inc := $(wildcard inc/*.h)
obj := $(patsubst src/%.c,obj/%.o,$(src))
out := vvc_decoder

$(out): $(obj)
	$(CC) $^ -o $@

obj:
	mkdir -p obj

obj/%.o: src/%.c inc/%.h | obj
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f $(obj) $(out)