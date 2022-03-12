DOCKER_IMAGE=espressif/idf:release-v4.4
PORT=/dev/ttyUSB0
DOCKER_INTERACTIVE=docker run --rm -it --device=$(PORT) -v $(PWD):/project -w /project $(DOCKER_IMAGE)

.PHONY: build configure menuconfig build-esp32 erase monitor flash run-image configure simulator build-x86_64

all: build

configure:
	git apply patches/ttgo-t-display.patch --directory  esp32/components/lvgl_esp32_drivers
	git apply patches/esp32-button.patch --directory  esp32/components/esp32-button

menuconfig:
	idf.py -C esp32 menuconfig

run-image:
	$(DOCKER_INTERACTIVE) /bin/bash

build-esp32:
	idf.py -C esp32 build
	idf.py -C esp32 size

flash:
	idf.py -C esp32 -p $(PORT) flash

erase:
	idf.py -C esp32 -p $(PORT) erase-flash

monitor:
	idf.py -C esp32 -p $(PORT) monitor

clean:
	rm -rf esp32/build
	rm -rf x86_64/build
	cd esp32/components/esp32-button && git restore .
	cd esp32/components/lvgl_esp32_drivers && git restore .

simulate_legacy_screen: build-x86_64
	${PWD}/x86_64/build/simulate_legacy_screen

simulate_provisioning_screen: build-x86_64
	${PWD}/x86_64/build/simulate_provisioning_screen

simulate_loading_screen: build-x86_64
	${PWD}/x86_64/build/simulate_loading_screen

simulate_main: build-x86_64
	${PWD}/x86_64/build/simulate_main

build-x86_64: 
	cmake -DCMAKE_BUILD_TYPE:STRING=Release -H$(PWD)/x86_64 -B$(PWD)/x86_64/build -G "Unix Makefiles"
	cmake --build $(PWD)/x86_64/build --config MinSizeRel --target all -j 18 --