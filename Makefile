
prefix?=/usr

install: main
	install -d ${DESTDIR}${prefix}/bin/
	install main ${DESTdir}${prefix}/bin/
