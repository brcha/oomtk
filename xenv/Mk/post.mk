all: stamp

extract:
	cd ../build && tar xjvf ../sources/$(TARBZ2)

patch: extract
	cd ../build/`basename $(TARBZ2) .tar.bz2` && patch -Np1 < ../../sources/$(PATCH)

configure: extract
	cd ../build/`basename $(TARBZ2) .tar.bz2` && ./configure --host=$(OOMTK_HOST) --target=$(OOMTK_TARGET) --prefix=$(OOMTK_DIR)/xenv $(CONFIGURE_OPTS)

makeconfigurehost:
	cd ../build/`basename $(TARBZ2) .tar.bz2` && make configure-host

make: configure
	cd ../build/`basename $(TARBZ2) .tar.bz2` && make

makeinstall: make
	cd ../build/`basename $(TARBZ2) .tar.bz2` && make install

stamp: makeinstall clean
	touch ../build/$(NAME).stamp

clean:
	cd ../build && rm -rf `basename $(TARBZ2) .tar.bz2`
