#Makefile by Maciek :)

CC=gcc
linkerArgs= -lm -lwiringPi #-lGL -L/opt/vc/lib/ -lbcm_host -lvcos -lvchiq_arm -lpthread

objects=main.o gyroLib.o client.o


PIgyro.out : ${objects}
	${CC} -o PCdisp.out ${objects} ${linkerArgs}

main.o : main.c gyroLib.h client.h
	${CC} -c main.c

gyroLib.o : gyroLib.c gyroLib.h
	${CC} -c gyroLib.c

client.o : client.c client.h
	${CC} -c client.c

clean : 
	rm ${objects} 
