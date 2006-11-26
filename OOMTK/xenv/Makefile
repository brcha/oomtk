THISMAKEFILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

include Makefile.vars

default: build

##################################################################
#
#  MULTITARGET targets.
#
#  Each of these make targets builds the corresponding component
#  for *all* target architectures
#
#  Invoke as something like:
#
#      "make -f Makefile.xenv TARGETS="oomtk-i386 oomtk-arm"
#
#  where each element of TARGETS is in the set
#
#    oomtk-{i386,x86_64,arm}
#
##################################################################

filesystem binutils pregcc newlib gcc gcc-links:
	@for buildtarget in $(TARGETS); do\
	  echo $(MAKE) -f Makefile.target BUILDTARGET=$${buildtarget} $@; \
	  $(MAKE) -f Makefile.target BUILDTARGET=$${buildtarget} $@; \
	done

##################################################################
#
#  HOST targets.
#
##################################################################

# Create the BUILD subdirectory, as needed:
BUILD:
	mkdir BUILD

.PSEUDO: build
build:
	$(MAKE) -f $(THISMAKEFILE) filesystem
	$(MAKE) -f $(THISMAKEFILE) binutils
	$(MAKE) -f $(THISMAKEFILE) pregcc
	$(MAKE) -f $(THISMAKEFILE) newlib
	$(MAKE) -f $(THISMAKEFILE) gcc
	$(MAKE) -f $(THISMAKEFILE) gcc-links

clean:
	-rm -rf BUILD
