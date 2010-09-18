all:../kernel/c.img

../kernel/c.img : out.img
	cp $^ $@

.PHONY: out.img

out.img : main
	@echo -e "1024\n1024\n" | ./main

.PHONY: clean

clean:
	rm main out.img
