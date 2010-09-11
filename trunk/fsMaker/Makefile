all:../c.img

../c.img : out.img
	cp $^ $@

.PHONY: out.img

out.img : main
	@echo "1024\n1024\n" | ./main

.PHONY: clean

clean:
	rm main out.img
