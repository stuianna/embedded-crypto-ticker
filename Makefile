DOCKER_IMAGE=espressif/idf:v4.3.1
BOARD=esp32
PORT=/dev/ttyUSB0
DOCKER=docker run --rm --device=$(PORT) -v $(PWD):/project -w /project $(DOCKER_IMAGE)
DOCKER_INTERACTIVE=docker run --rm -it --device=$(PORT) -v $(PWD):/project -w /project $(DOCKER_IMAGE)

.PHONY: build flash run-image setup

all: build

setup:
	git apply patches/ttgo-t-display.patch --directory  components/lvgl_esp32_drivers

configure:
	$(DOCKER_INTERACTIVE) idf.py menuconfig

run-image:
	$(DOCKER_INTERACTIVE) /bin/bash

build:
	$(DOCKER) idf.py build

flash:
	$(DOCKER) idf.py -p $(PORT) flash

monitor:
	screen $(PORT) 115200

clean:
	$(DOCKER) rm -rf build

simulator: x86_64
	${PWD}/x86_64/build/embedded-crypto-ticker

x86_64: 
	cmake -DCMAKE_BUILD_TYPE:STRING=Release -H$(PWD)/x86_64 -B$(PWD)/x86_64/build -G "Unix Makefiles"
	cmake --build $(PWD)/x86_64/build --config MinSizeRel --target all -j 18 --