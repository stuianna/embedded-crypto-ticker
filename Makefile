ESP32_DOCKER_IMAGE=espressif/idf:release-v4.4
SDL2_DOCKER_IMAGE=tsukisuperior/sdl2-docker
PORT=/dev/ttyUSB0
DOCKER_ESP32=docker run --rm -it --device=$(PORT) -v $(PWD):/project -w /project $(ESP32_DOCKER_IMAGE)
DOCKER_SDL2=docker run --rm -it -v $(PWD):/project -w /project -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$(DISPLAY) -h $$HOSTNAME -v $(HOME)/.Xauthority:/root/.Xauthority $(SDL2_DOCKER_IMAGE)

.PHONY: build configure menuconfig build-esp32 erase monitor flash run-image configure simulator build-x86_64 docs

all: build-esp32

configure:
	git apply patches/ttgo-t-display.patch --directory  esp32/components/lvgl_esp32_drivers
	git apply patches/esp32-button.patch --directory  esp32/components/esp32-button

menuconfig:
	idf.py -C esp32 menuconfig

run-image-esp32:
	$(DOCKER_ESP32) /bin/bash

run-image-sdl2:
	$(DOCKER_SDL2) /bin/bash

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
	rm -rf tests/build
	cd esp32/components/esp32-button && git restore .
	cd esp32/components/lvgl_esp32_drivers && git restore .
	rm -rf docs/html

docs:
	doxygen
	# Try to open the docs with the browser environmental variable.
	# Otherwise default to brave.
	if [ -z "$$BROWSER" ]; \
	then \
		brave $(PWD)/docs/docs/index.html; \
	else \
		$$BROWSER $(pwd)/docs/docs/index.html; \
	fi;

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
	cmake --build $(PWD)/x86_64/build --config Release --target all -j 18 --

test:
	cmake -DCMAKE_BUILD_TYPE:STRING=Release -H$(PWD)/tests -B$(PWD)/tests/build -G "Unix Makefiles"
	cmake --build $(PWD)/tests/build --config Release --target all -j 18 --
	find -type d -exec run-parts \{\} \;