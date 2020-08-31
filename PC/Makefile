#Makefile by Maciek :)

CC=gcc
srcDir=src
inclDir=headers
linkerArgs=-lraylib -lpthread #-lm -lwiringPi -lGL -L/opt/vc/lib/ -lbcm_host -lvcos -lvchiq_arm

objects=main.o gfx_interface.o server.o


PCdisp.out : ${objects}
	${CC} -o PCdisp.out ${objects} ${linkerArgs}

main.o : ${srcDir}/main.c ${inclDir}/gfx_interface.h ${inclDir}/server.h
	${CC} -c ${srcDir}/main.c

gfx_interface.o : ${srcDir}/gfx_interface.c ${inclDir}/gfx_interface.h 
	${CC} -c ${srcDir}/gfx_interface.c

server.o : ${srcDir}/server.c ${inclDir}/server.h
	${CC} -c ${srcDir}/server.c

clean : 
	rm ${objects} 
