CC := gcc
CFLAGS := -Wall -I./inc
CPPFLAGS := 
LDFLAGS := -lm

src := $(wildcard src/*.c)
inc := $(wildcard inc/*.h)
obj := $(patsubst src/%.c,obj/%.o,$(src))
out := vvc_decoder

all: $(out) $(out)_debug

$(out): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

$(out)_debug: $(obj)
	$(CC) -g $^ -o $@ $(LDFLAGS)

obj:
	mkdir -p obj

obj/%.o: src/%.c inc/%.h | obj
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f $(obj) $(out) $(out)_debug