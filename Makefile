# anything with a name main*.c will be compiled and run with valgrind

# compile all build a dynamic shared lib and object files by default
# so you can choose how to link the runtime

# there are some fancy bits in here the common uses will be:
# make
# make compile // differencial build
# make clean // clean
# make clean all // clean build


CC		 	 := gcc
SRCDIR 	     := .
SRC          := $(shell find $(SRCDIR) -maxdepth 1 -name "*.c" -not -name "main*.c" )
SRCMAIN      := $(shell find $(SRCDIR) -name "main*.c")
MAINS        := $(shell find $(OBJDIR) -name "main*.e")
INCLUDEDIR   := include
OBJDIR       := obj
OPTIMISATION := -O3
PARAMS       := -Wall -Wno-format -Wno-unused-variable -std=gnu17 -I$(INCLUDEDIR)
VALGRIND	 := valgrind

OBJ 		:= $(SRC:%.c=$(OBJDIR)/%.o)
MAIN 		:= $(SRCMAIN:%.c=$(OBJDIR)/%.e)
RUNMAINS 	:= $(MAINS:%.e=$(OBJDIR)/%.e)

all 	: compile test
compile : obj $(MAIN)

printdebug 	: CC += -g -DPRINT_DEBUG=1
printdebug 	: OPTIMISATION = -O0
printdebug 	: clean all

debug 	: CC += -g
debug 	: clean all

test : $(RUNMAINS) 

createdirs :
	@mkdir -p obj
	@mkdir -p obj/test

obj : createdirs $(OBJ)

clean:
	rm -fr obj 

$(OBJDIR)/%.o : %.c 
	@echo "COMPILING SOURCE $< INTO OBJECT $@ with $(CC) $(PARAMS)"
	@$(CC)  $(OPTIMISATION) -c $(PARAMS) $< -o $@

$(OBJDIR)/%.e : %.c 
	@echo "COMPILING SOURCE $< INTO MAIN $@"
	$(CC)  $(OPTIMISATION) $(PARAMS) $(OBJ) $< -o $@	
	# resample the newly compiled mains
	# $(call MAIN)
	time $@

$(OBJDIR)/%.e : %.e
	# @echo "valgrind -s --track-origins=yes --error-exitcode=2 --leak-check=full $<"
	$(VALGRIND) -s --track-origins=yes --error-exitcode=2 --leak-check=full $<
