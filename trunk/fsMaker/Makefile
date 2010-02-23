../c.img : out.img
	cp $^ $@

.PHONY: out.img

out.img : main
	@echo "512\n1000" | ./main
