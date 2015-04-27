all:
	/usr/share/qt4/bin/qmake -o tmpMakefile
	make -f tmpMakefile 

clean:
	make -f tmpMakefile clean
	rm tmpMakefile

