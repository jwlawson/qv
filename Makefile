#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define any compile-time flags
# Using cygwin -std=gnu++11 should be used rather than -std=c++11
CXXFLAGS = -Wall -g -std=gnu++11

# Specify base directory
BASE_DIR = .

# Specify source directory
SRC_DIR = $(BASE_DIR)/src

# Specify test directory
TEST_DIR = $(BASE_DIR)/test

# define the output directory for .o
OBJ_DIR = $(BASE_DIR)/build

# define any directories containing header files other than /usr/include
# -I/home/newhall/include 
INCLUDES = -I$(BASE_DIR)/include -I$(BASE_DIR)/src -I$(BASE_DIR)/lib/include 

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like: -L/home/newhall/lib
LFLAGS = -L$(BASE_DIR)/lib -L$(BASE_DIR)/lib/cygwin -L$(BASE_DIR)/lib \
				 -L$(BASE_DIR)/lib/linux 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS =
TEST_LIBS = -lgtest -lgtest_main -lpthread

# define the C source files
SRCS = $(wildcard $(SRC_DIR)/*.cc)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cc)

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .cc of all words in the macro SRCS
# with the .o suffix
#
_OBJS = $(SRCS:.cc=.o)
_TEST_OBJS = $(TEST_SRCS:.cc=.o)

# Puts objs in obj_dir
OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_OBJS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%,$(OBJ_DIR)/%,$(_TEST_OBJS))

# define the executable file 
MAIN = qv
TEST = testqv


#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: clean

all:    test

$(MAIN): $(OBJS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	
testqv: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TEST) $(TEST_OBJS) $(OBJS) $(LFLAGS) $(TEST_LIBS)

test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TEST) $(TEST_OBJS) $(OBJS) $(LFLAGS) $(TEST_LIBS)
	@echo Running tests
	@./$(TEST)
	

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@
	
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cc 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN) $(OBJ_DIR)/*.o

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
