.PHONY: runClient

CC = gcc
OUT_DIR = target
OUT_FILE = a.out
SOURCES = lib/client.c lib/parseURL.c

default: runClient

runClient: $(OUT_DIR)/$(OUT_FILE)

$(OUT_DIR)/$(OUT_FILE): $(SOURCES)
	mkdir -p $(OUT_DIR)
	$(CC) $(SOURCES) -o $(OUT_DIR)/$(OUT_FILE)

clean:
	rm -rf $(OUT_DIR)
