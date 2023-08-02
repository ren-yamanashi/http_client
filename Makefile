.PHONY: run

OUT_DIR = target

runClient: $(OUT_DIR)/a.out

$(OUT_DIR)/a.out: lib/client.c
	mkdir -p $(OUT_DIR)
	gcc lib/client.c -o $@
