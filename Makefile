.PHONY: run

OUT_DIR = target

run: $(OUT_DIR)/a.out
	./$(OUT_DIR)/a.out


$(OUT_DIR)/a.out: lib/main.c
	mkdir -p $(OUT_DIR)
	gcc lib/main.c -o $@
