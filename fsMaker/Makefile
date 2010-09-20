IMG = ../kernel/c.img
BINS = root/idle root/ottanta root/hello root/init root/shell

all: $(IMG)

$(IMG) : out.img
	@echo "fsMaker -> kernel"
	@mv $^ $@

$(BINS):
	@cd ../elves && make && cd ../fsMaker

out.img : main $(BINS)
	@echo "1024\n1024" | ./main > /dev/null

%: %.cpp
	@echo "GCC" $@
	@$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f main $(BINS)
