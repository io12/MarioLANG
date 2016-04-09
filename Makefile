CFLAGS = -Wall -O3
BUILD = MarioLANG

.PHONY = all install clean

all: $(BUILD)

install: $(BUILD)
	install $(BUILD) /usr/local/bin/$(BUILD)

clean:
	rm -f $(BUILD)
