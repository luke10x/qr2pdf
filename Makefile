.DEFAULT_GOAL=qr2pdf
vpath %.c ./src

src = qr2pdf.c
#test_src = test.c qr2pdf.c

objects = $(patsubst %.c,obj/%.o,$(src))
test_objects = $(patsubst %.c,obj/%.o,$(test_src))

$(objects): | obj

obj:
	mkdir -p $@

obj/%.o : %.c
	@echo $<
	$(CC) -c $< `pkg-config --cflags --libs cairo` \
		`pkg-config --cflags --libs librsvg-2.0` \
		`pkg-config --cflags --libs qrencode` \
		-g \
		-o $@

qr2pdf: $(objects)
	echo $<
	$(CC) $^ `pkg-config --libs cairo` \
		`pkg-config --libs librsvg-2.0` \
		-L/usr/lib/x86_64-linux-gnu/libqrencode.so -lqrencode \
		-o $@

.PHONY: clean

clean:
	@rm -f obj/*.o
	@rm -f qr2pdf
	@rm -f output.pdf
