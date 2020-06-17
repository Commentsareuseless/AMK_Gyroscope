#Makefile by Maciek :)

CC=gcc
srcDir=src
inclDir=headers
linkerArgs=-lraylib -lm -lwiringPi -lGL -L/opt/vc/lib/ -lbcm_host -lvcos -lvchiq_arm -lpthread

objects=main.o gfx_interface.o gyroLib.o 


klocek.out : ${objects}
	${CC} -o klocek.out ${objects} ${linkerArgs}

main.o : ${srcDir}/main.c ${inclDir}/gfx_interface.h ${inclDir}/gyroLib.h
	${CC} -c ${srcDir}/main.c

gfx_interface.o : ${srcDir}/gfx_interface.c ${inclDir}/gfx_interface.h 
	${CC} -c ${srcDir}/gfx_interface.c

gyroLib.o : ${srcDir}/gyroLib.c ${inclDir}/gyroLib.h
	${CC} -c ${srcDir}/gyroLib.c

# globals.o : ${srcDir}/globals.c ${inclDir}/globals.h
# 	${CC} -c ${srcDir}/globals.c

# I2Cif.o : ${srcDir}/I2Cif.c ${inclDir}/I2Cif.h
# 	${CC} -c ${srcDir}/I2Cif.c

clean : 
	rm ${objects} 
