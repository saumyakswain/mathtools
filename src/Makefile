### Configuration.####################################################
# Default C compiler options.
CFLAGS				= -Wall -g
# 
OBJDIR := ../obj
EXECDIR := ../bin
OBJS := $(addprefix $(OBJDIR)/,main.o matsolve.o common.o node.o cmd.o mat.o node.o num_methods.o btree.o)
EXEC := $(addprefix $(EXECDIR)/,matsolve)

# Object files.
#OBJECTS				= $(addprefix $(OBJDIR)/, main.o matsolve.o common.o node.o cmd.o mat.o node.o num_methods.o btree.o)

### Rules.
############################################################
# Phony targets: namesfor conceptual build targets.
.PHONY:	all clean
# Default target: build everything.

# default rule for building object files from source files.

$(OBJDIR)/%.o : %.c matsolve.h
	$(COMPILE.c) $(OUTPUT_OPTION) $<
all: $(OBJS) $(EXEC)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)


    
# Clean up build products.
        
clean:
	rm -f $(OBJS) $(EXEC)

$(EXECDIR)/matsolve:$(OBJS)
	$(CC) $(CFLAGS)  -o $@ $^ -lm

$(EXEC): | $(EXECDIR)

$(EXECDIR):
	mkdir $(EXECDIR)


