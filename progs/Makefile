include ../common.mk

SDIR=${CURDIR}
ODIR=${CURDIR}/.obj

SRC=$(shell find $(SDIR) -type f -name '*.cpp')
OBJ=$(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))

EXE=$(patsubst $(SDIR)/%.cpp,$(BDIR)/%,$(SRC))

$(shell mkdir -p $(ODIR))

DEPFILES=$(SRC:$(SDIR)/%.cpp=$(ODIR)/%.d)

# to prevent files from being auto-deleted by make after build completes
.SECONDARY:

.PHONY: all
all: $(EXE)

$(BDIR)/%: $(ODIR)/%.o
	$(CPPC) $(CPPFLAGS) $^ -lgame -lglib -o $@ $(IFLAGS) $(LDFLAGS) -lutil

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CPPC) $(CPPFLAGS) $< -c -o $@ $(IFLAGS)


-include $(wildcard $(DEPFILES))
-include $(wildcard $(PDEPFILES))

.PHONY: clean
clean:
	rm -rf $(ODIR)
	rm -rf $(PODIR)


# $(CPPC) $(CPPFLAGS) $(OBJ) -o $@ $(LDFLAGS) -lglib
