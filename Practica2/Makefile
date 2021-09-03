#Compilacion de la practica
#Se agregaron las banderas
compilar:
	g++ -std=c++11 -pedantic-errors -Wall Practica2.cpp

#Redireccionamos la salida del programa a un .dot
#Y posteriormente borramos el ejecutable.
ejecutar: compilar
	./a.out > salida.dot
	rm a.out

#Con este comando pasamos el .dot a un png
#dot -Tpng -O salida.dot
