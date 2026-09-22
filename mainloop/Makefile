PACKAGE=mainloop

all: bad good glib doc

bad: bad/mainloop_bad
bad/mainloop_bad: bad/mainloop_bad.c
	(cd bad; $(MAKE))

good: good/mainloop_good
good/mainloop_good: good/mainloop_good.c
	(cd good; $(MAKE))

glib: glib/mainloop_glib
glib/mainloop_glib: glib/mainloop_glib.c
	(cd glib; $(MAKE))

doc: doxygen/index.html doxygen/mainloop.pdf doxygen/mainloop.ps

doxygen/index.html: bad/mainloop_bad.c good/mainloop_good.c glib/mainloop_glib.c
	doxygen .doxygen

doxygen/mainloop.pdf: doxygen/index.html
	(cd doxygen/latex; $(MAKE))
	mv doxygen/latex/refman.pdf doxygen/mainloop.pdf

doxygen/mainloop.ps: bad/mainloop_bad.c good/mainloop_good.c glib/mainloop_glib.c
	-(a2ps -f -i -f5 bad/mainloop_bad.c good/mainloop_good.c glib/mainloop_glib.c > doxygen/mainloop.ps; true)

clean:
	(cd bad; $(MAKE) clean)
	(cd good; $(MAKE) clean)
	(cd glib; $(MAKE) clean)
	-rm -rf doxygen/*

package:
	$(MAKE) clean
	$(MAKE) all
	-rm -f $(PACKAGE).tar.gz
	(cd ..; gtar zcf $(PACKAGE).tar.gz $(PACKAGE))
	mv ../$(PACKAGE).tar.gz doxygen

