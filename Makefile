output: 2d_engine.o shape.o libs/RGBA.o component.o object.o edge.o line.o
	g++  -I include/ -o 2d_engine 2d_engine.o shape.o component.o object.o libs/RGBA.o edge.o line.o glad.c -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl

2d_engine.o: 2d_engine.cpp
	g++ -I include/ -c -g 2d_engine.cpp

shape.o: shape.cpp
	g++ -I include/ -c -g shape.cpp

component.o: component.cpp
	g++ -I include/ -c -g component.cpp

object.o: object.cpp
	g++ -I include/ -c -g object.cpp;

libs/RGBA.o: libs/RGBA.cpp
	g++ -c libs/RGBA.cpp -o libs/RGBA.o

edge.o: edge.cpp
	g++ -c -g edge.cpp

libs/image.o: libs/image.cpp
	g++ -I include/ -c -g libs/image.cpp -o libs/image.o

line.o: line.cpp
	g++ -c -g line.cpp

clean:
	rm *.o libs/*.o 2d_engine