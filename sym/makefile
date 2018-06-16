TAR=watcom
#TAR=gcc

#  To set a default target, add a line with TAR=target to 
#  a file called "makefile-tar".  Only try to include the 
#  file if it exists.

-include makefile-tar

ifeq ($(TAR),gcc)
   CC   = gcc
   OBJ  = o
   EXE  = sym.exe
   EOPT = -o $(EXE)
   OPT  = -g
endif

ifeq ($(TAR),watcom)
   CC   = wcl386 /d2 /w2 /zq
   OBJ  = obj
   EXE  = sym.exe
   EOPT = 
   OPT  = 
endif

OBJS = assoc.$(OBJ) command.$(OBJ) declare.$(OBJ) \
	   nodes.$(OBJ) output.$(OBJ) \
	   parse.$(OBJ) spprint.$(OBJ) \
	   symtable.$(OBJ) xmalloc.$(OBJ) \
      lists.$(OBJ) sets.$(OBJ) eqns.$(OBJ) \
      readfile.$(OBJ) syntax.$(OBJ) error.$(OBJ) options.$(OBJ) \
      numsub.$(OBJ) refinesets.$(OBJ) cart.$(OBJ) str.$(OBJ) \
      lang.$(OBJ) default.$(OBJ) wprint.$(OBJ)
      
LANGS = lang/setup.$(OBJ) lang/debug.$(OBJ) lang/msgproc.$(OBJ) \
        lang/oxnewton.$(OBJ) lang/troll.$(OBJ) lang/tablo.$(OBJ) \
        lang/html.$(OBJ) lang/oxgs.$(OBJ) lang/oxgst.$(OBJ)

LANGLINK = $(LANGS)
ifeq ($(TAR),watcom)
   LANGLINK = $(subst /,\\,$(LANGS))
endif

lang/%.obj : lang/%.c
	$(CC) $(OPT) -c lang\\$*.c /fo=lang\\ 
   
lang/%.o : lang/%.c
	$(CC) $(OPT) -c lang/$*.c -o lang/$*.o
   
%.$(OBJ) : %.c sym.h
	$(CC) $(OPT) -c $<  
   
#	$(CC) $(OPT) -c $< -o $*.$(OBJ)

$(EXE) : sym.$(OBJ) $(OBJS) $(LANGS) 
	$(CC) $(OPT) $(EOPT) sym.$(OBJ) $(OBJS) $(LANGLINK)

build.h : $(OBJS) $(LANGS) sym.c sym.h version.h
	lastbuild

#
#  add -t flag to yacc for parser debugging messages
#

parse.c : parse.y 
	byacc -o parse.c parse.y

syntax.c : parse.y
	perl makesyntax.p

$(LANGS) : sym.h lang.h output.h

gcubed.lis gcubed.out : $(EXE) gcubed.sym
	sym gcubed.sym gcubed.out > gcubed.lis

check : gcubed.lis gcubed.out
	diff gcubed.out.save gcubed.out
	diff gcubed.lis.save gcubed.lis

backup :
	zip -r sym.zip *.y *.c *.h lang/*.c makefile
	datename -t sym.zip

clean:
	rm -f *.$(OBJ) lang/*.$(OBJ) parse.c $(EXE)

#
# header file dependencies 
#    built by gcc -MM *.c lang/*.c
#

assoc.o: assoc.c assoc.h error.h str.h sym.h xmalloc.h
cart.o: cart.c cart.h lists.h error.h sets.h sym.h xmalloc.h
command.o: command.c
declare.o: declare.c declare.h nodes.h lists.h error.h options.h sets.h \
 str.h sym.h symtable.h
default.o: default.c lang.h output.h lists.h str.h sym.h
eqns.o: eqns.c eqns.h nodes.h lists.h error.h options.h sets.h spprint.h \
 str.h sym.h symtable.h xmalloc.h
error.o: error.c error.h output.h lists.h sym.h
lang.o: lang.c lang.h assoc.h codegen.h error.h lists.h str.h sym.h
lexical.o: lexical.c lexical.h error.h nodes.h lists.h
lists.o: lists.c lists.h error.h str.h sym.h xmalloc.h
nodes.o: nodes.c nodes.h lists.h error.h sym.h xmalloc.h
numsub.o: numsub.c error.h lists.h sets.h sym.h symtable.h
options.o: options.c options.h error.h lists.h sym.h
output.o: output.c output.h lists.h cart.h codegen.h eqns.h nodes.h \
 error.h options.h sets.h str.h sym.h symtable.h wprint.h xmalloc.h
parse.o: parse.c str.h sym.h nodes.h lists.h declare.h eqns.h lexical.c \
 lexical.h error.h
readfile.o: readfile.c error.h lexical.h output.h lists.h str.h sym.h \
 xmalloc.h
refinesets.o: refinesets.c error.h lists.h sets.h str.h sym.h
sets.o: sets.c sets.h lists.h error.h options.h str.h sym.h symtable.h \
 wprint.h xmalloc.h
spprint.o: spprint.c spprint.h nodes.h lists.h error.h str.h sym.h
str.o: str.c str.h error.h sym.h xmalloc.h
sym.o: sym.c sym.h build.h eqns.h nodes.h lists.h error.h lang.h output.h \
 readfile.h sets.h str.h symtable.h version.h xmalloc.h
symtable.o: symtable.c symtable.h lists.h error.h nodes.h options.h \
 sets.h str.h sym.h xmalloc.h
syntax.o: syntax.c
wprint.o: wprint.c wprint.h lists.h error.h sym.h xmalloc.h
xmalloc.o: xmalloc.c xmalloc.h
debug.o: lang/debug.c lang/../eqns.h lang/../nodes.h lang/../lists.h \
 lang/../error.h lang/../lang.h lang/../options.h lang/../output.h \
 lang/../sym.h lang/../symtable.h lang/../wprint.h
html.o: lang/html.c lang/../assoc.h lang/../eqns.h lang/../nodes.h \
 lang/../lists.h lang/../error.h lang/../lang.h lang/../options.h \
 lang/../output.h lang/../sets.h lang/../str.h lang/../sym.h \
 lang/../symtable.h lang/../xmalloc.h
msgproc.o: lang/msgproc.c lang/../cart.h lang/../lists.h lang/../eqns.h \
 lang/../nodes.h lang/../error.h lang/../lang.h lang/../options.h \
 lang/../output.h lang/../sets.h lang/../str.h lang/../sym.h \
 lang/../symtable.h
oxgs.o: lang/oxgs.c lang/../cart.h lang/../lists.h lang/../eqns.h \
 lang/../nodes.h lang/../error.h lang/../lang.h lang/../options.h \
 lang/../output.h lang/../str.h lang/../sym.h lang/../symtable.h
oxgst.o: lang/oxgst.c lang/../error.h lang/../eqns.h lang/../nodes.h \
 lang/../lists.h lang/../options.h lang/../cart.h lang/../lang.h \
 lang/../output.h lang/../sets.h lang/../str.h lang/../sym.h \
 lang/../symtable.h lang/../xmalloc.h
oxnewton.o: lang/oxnewton.c lang/../cart.h lang/../lists.h lang/../eqns.h \
 lang/../nodes.h lang/../error.h lang/../lang.h lang/../options.h \
 lang/../output.h lang/../str.h lang/../sym.h lang/../symtable.h
setup.o: lang/setup.c lang/../lang.h
tablo.o: lang/tablo.c lang/../assoc.h lang/../error.h lang/../lang.h \
 lang/../options.h lang/../output.h lang/../lists.h lang/../sets.h \
 lang/../str.h lang/../sym.h lang/../symtable.h
troll.o: lang/troll.c lang/../cart.h lang/../lists.h lang/../error.h \
 lang/../lang.h lang/../options.h lang/../output.h lang/../str.h \
 lang/../sym.h lang/../symtable.h
