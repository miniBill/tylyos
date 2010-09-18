IMG = ../kernel/c.img
BINS = root/idle root/ottanta root/hello root/init root/shell

all: $(IMG)

$(IMG) : out.img
	mv $^ $@

out.img : main $(BINS)
	@cd ../elves && make && cd ../fsMaker
	@echo -e "1024\n1024\n" | ./main > /dev/null

.PHONY: clean
clean:
	rm -fR main $(BINS)
