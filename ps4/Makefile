# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wall -g

# Define any directories containing header files other than /usr/include
#
# Example:
# INCLUDES=-I/home/newhall/include  -I../include
INCLUDES=

# Define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#
# LFLAGS=-L/home/newhall/lib  -L../lib
LFLAGS= -lm

# Define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#
# LIBS=-lmylib -lm
LIBS=

# Define the C source files
SRCS=main.c bmp.c transformations.c

# Define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#   For each word in 'name' replace 'string1' with 'string2'
#
OBJS=$(SRCS:.c=.o)

# Define the executable file 
MAIN=my_program

#
# The following part of the Makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the commands below. 
#

all:	$(MAIN)
	@echo  Simple compiler named my_program has been compiled

$(MAIN):	$(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# This is a suffix replacement rule for building .o's from .c's
# It uses automatic variables $<: the name of the prerequisite of 
# the rule(a .c file) and $@: the name of the target of the rule 
# (a .o file) 
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
