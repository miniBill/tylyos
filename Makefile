.PHONY: all

all:
	cd kernel && make

.PHONY: clean

clean:
	cd elves && make clean && cd .. && cd fsMaker && make clean && cd .. && cd kernel && make clean
