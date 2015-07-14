CC=g++
CXX_FLAGS=-O3 -std=c++0x 
OBJECTS=main.o grayscale.o gaussianblur.o threshold.o morphology.o segmentation.o

all: ImageProcessor clean

ImageProcessor : ${OBJECTS}
	    ${CC} ${CXX_FLAGS} ${OBJECTS} ${SHARED} ${GUI_SHARED} -o $@

main.o : ./src/main.cpp
	    ${CC} ${CXX_FLAGS} -c $< -o $@ 

grayscale.o : ./src/Grayscale.cpp
		${CC} ${CXX_FLAGS} -c $< -o $@

gaussianblur.o : ./src/GaussianBlur.cpp
		${CC} ${CXX_FLAGS} -c $< -o $@

threshold.o : ./src/Threshold.cpp
		${CC} ${CXX_FLAGS} -c $< -o $@

morphology.o : ./src/Morphology.cpp
		${CC} ${CXX_FLAGS} -c $< -o $@

segmentation.o : ./src/Segmentation.cpp
		${CC} ${CXX_FLAGS} -c $< -o $@

.PHONY: clean

clean:
		rm -f *.o
