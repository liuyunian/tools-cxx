BUILD_DIR = ./build
RM = rm -rf

all: build
	cd $(BUILD_DIR); make

build:
	mkdir $(BUILD_DIR); cd $(BUILD_DIR); cmake ..

install:
	cd $(BUILD_DIR); sudo make install

uninstall:
	cd $(BUILD_DIR); cat install_manifest.txt | sudo xargs $(RM)

clean:
	-$(RM) $(BUILD_DIR)