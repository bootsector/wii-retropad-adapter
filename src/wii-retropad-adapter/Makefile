all:
	make -C arduinocore -f Makefile all
	make -f Makefile.app all
	
clean:
	make -C arduinocore -f Makefile clean
	make -f Makefile.app clean
	
program:
	make -f Makefile.app program
	