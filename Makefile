DOCKER_IMAGE=espressif/idf:release-v4.4
BOARD=esp32
PORT=/dev/ttyUSB0
DOCKER=docker run --rm  -v $(PWD):/project -w /project $(DOCKER_IMAGE)
DOCKER_WITH_DEVICE=docker run --rm --device=$(PORT) -v $(PWD):/project -w /project $(DOCKER_IMAGE)
DOCKER_INTERACTIVE=docker run --rm -it --device=$(PORT) -v $(PWD):/project -w /project $(DOCKER_IMAGE)

.PHONY: build flash run-image setup simulator x86_64

all: build

setup:
	git apply patches/ttgo-t-display.patch --directory  esp32/components/lvgl_esp32_drivers
	git apply patches/esp32-button.patch --directory  esp32/components/esp32-button

configure:
	$(DOCKER_INTERACTIVE) idf.py -C esp32 menuconfig

run-image:
	$(DOCKER_INTERACTIVE) /bin/bash

build:
	$(DOCKER) idf.py -C esp32 build

flash:
	$(DOCKER_WITH_DEVICE) idf.py -C esp32 -p $(PORT) flash

erase:
	$(DOCKER_WITH_DEVICE) idf.py -C esp32 -p $(PORT) erase-flash

monitor:
	$(DOCKER_INTERACTIVE) idf.py -C esp32 -p $(PORT) monitor

clean:
	$(DOCKER) rm -rf esp32/build
	rm -rf x86_64/build

simulate_legacy_screen: x86_64
	${PWD}/x86_64/build/simulate_legacy_screen

simulate_provisioning_screen: x86_64
	${PWD}/x86_64/build/simulate_provisioning_screen

simulate_loading_screen: x86_64
	${PWD}/x86_64/build/simulate_loading_screen

simulate_main: x86_64
	${PWD}/x86_64/build/simulate_main

x86_64: 
	cmake -DCMAKE_BUILD_TYPE:STRING=Release -H$(PWD)/x86_64 -B$(PWD)/x86_64/build -G "Unix Makefiles"
	cmake --build $(PWD)/x86_64/build --config MinSizeRel --target all -j 18 --