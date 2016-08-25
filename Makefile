NAME = qv
MAJOR = 0
MINOR = 16
VERSION = $(MAJOR).$(MINOR)

# Using cygwin -std=gnu++11 should be used rather than -std=c++11
ifeq ($(CXX),g++)
override CXXFLAGS += -Wall -Wextra -Werror -march=native\
	-fno-signed-zeros -fno-math-errno -fno-rounding-math\
	-fno-signaling-nans -fno-trapping-math\
	-ffinite-math-only
OPT = -Ofast
else
override CXXFLAGS = -Wall -std=c++11 -xHOST
OPT = -O3 -ipo -no-prec-div
B_OPT = $(OPT)
AR = xiar
endif

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
uname_M := $(shell sh -c 'uname -m 2>/dev/null || echo not')
uname_O := $(shell sh -c 'uname -o 2>/dev/null || echo not')

ifeq ($(uname_O),Cygwin)
	override CXXFLAGS += -std=gnu++11 -DCYGWIN_STOI
endif
ifeq ($(uname_S),Linux)
	override CXXFLAGS += -std=gnu++11 -fPIC
endif

LIB = lib$(NAME).so.$(VERSION)
STATIC = lib$(NAME).a
TEST = testqv

LDFLAGS = -shared -Wl,-soname,$(LIB)

# Specify base directory
BASE_DIR = .

# Specify source directory
SRC_DIR = $(BASE_DIR)/src

# Specify test directory
TEST_DIR = $(BASE_DIR)/test

# define the output directory for .o
OBJ_DIR = $(BASE_DIR)/build

INC_DIR = $(BASE_DIR)/include

# Install directory
PREFIX = $(HOME)

# define any directories containing header files other than /usr/include
# -I/home/newhall/include
INCLUDES = -I$(HOME)/include -I$(BASE_DIR)/include \
           -I$(BASE_DIR)/lib/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like: -L/home/newhall/lib
LFLAGS = -L$(HOME)/lib -L$(BASE_DIR)/lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) use -lx
LIBS = -lginac
TEST_LIBS = -lgtest -lgtest_main -lginac -pthread

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



.PHONY: clean

all:	$(LIB)

$(TEST): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(B_OPT) $(INCLUDES) -o $(TEST) $(TEST_OBJS) $(OBJS) $(LFLAGS) $(TEST_LIBS)

test: CXXFLAGS += -DQV_USE_GINAC
test: $(TEST)
	@echo Running tests
	@./$(TEST)

lib:	$(LIB)
static:	$(STATIC)

$(LIB):	$(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(STATIC): $(OBJS)
	$(AR) rcs $(STATIC) $(OBJS)

install:	$(LIB)
	cp $(LIB) $(PREFIX)/lib
	ldconfig -v -n $(PREFIX)/lib
	ln -fs $(PREFIX)/lib/$(LIB) $(PREFIX)/lib/lib$(NAME).so
	mkdir -p $(PREFIX)/include/qv
	cp -ru include/* $(PREFIX)/include/qv/

uninstall:
	rm $(PREFIX)/lib/lib$(NAME).*
	rm -r $(PREFIX)/include/qv

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -c $< -o $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) *.o *~ $(MAIN) $(OBJ_DIR)/*.o $(LIB) $(STATIC) $(TEST)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
