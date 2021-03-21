include common.mk

.PHONY: all
all: dirs glib utils main progs

.PHONY: dirs
dirs:
	@mkdir -p $(LDIR)
	@mkdir -p $(BDIR)

.PHONY: progs
progs:
	(make -C $(PROGDIR) BASE_DIR=${CURDIR} BDIR=$(BDIR) LDIR=$(LDIR))

.PHONY: main
main:
	(make -C $(GAMEDIR) BASE_DIR=${CURDIR} LDIR=$(LDIR))

.PHONY: glib
glib:
	(make -C $(GLIBDIR) BASE_DIR=${CURDIR} TDIR=$(TDIR) LDIR=$(LDIR))

.PHONY: utils
utils:
	(make -C $(UTILSDIR) BASE_DIR=$(UTILSDIR) TDIR=$(TDIR) LIB_DIR=$(LDIR))

.PHONY: clean
clean:
	(make clean -C $(GLIBDIR))
	(make clean -C $(UTILSDIR))
	(make clean -C $(GAMEDIR))
	rm -rf $(BDIR)
	rm -rf $(LDIR)

