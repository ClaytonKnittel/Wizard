include ../common.mk

SDIR=src
ODIR=.obj

SRC=$(shell find $(SDIR) -type f -name '*.cpp')
OBJ=$(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))
DEP=$(wildcard $(SDIR)/*.h)

DIRS=$(shell find $(SDIR) -type d)
OBJDIRS=$(patsubst $(SDIR)/%,$(ODIR)/%,$(DIRS))

$(shell mkdir -p $(ODIR))
$(shell mkdir -p $(OBJDIRS))

DEPFILES=$(SRC:$(SDIR)/%.cpp=$(ODIR)/%.d)

.PHONY: all
all: $(LDIR)/libgame.a

$(LDIR)/libgame.a: $(OBJ)
	$(AR) -rcs $@ $^
	# $(CPPC) $(CPPFLAGS) $(OBJ) -o $@ $(LDFLAGS) -lglib

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CPPC) $(CPPFLAGS) $< -c -o $@ $(IFLAGS)

-include $(wildcard $(DEPFILES))

.PHONY: clean
clean:
	rm -rf $(ODIR)

