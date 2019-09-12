BUILD_DIR = ./build
RM = rm -rf

all: build
	cd $(BUILD_DIR); make

build:
	mkdir $(BUILD_DIR); cd $(BUILD_DIR); cmake ..

clean:
	-$(RM) $(BUILD_DIR)