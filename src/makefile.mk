
# compiler include search paths
INCS = -I./ \
	-I../../boost/boost1_72 \
	-I../../raven-set \
	-I../../windex/include


SDIR = ../src
ODIR = ../build/obj

OPT = -g
CCflags = -std=c++17 $(OPT)
STATLibs = -static-libstdc++ -static-libgcc -static
BOOSTLIB = -L../../boost/boost1_72/lib -lboost_program_options-mgw82-mt-x64-1_72
WEXLIBS = -lstdc++fs -lgdiplus -lgdi32 -lcomdlg32

### OBJECT FILES

OBJ = $(ODIR)/main.o $(ODIR)/cmaze.o

### COMPILE

$(ODIR)/main.o : main.cpp
	g++ -o  $@ -c main.cpp $(INCS) $(CCflags)

$(ODIR)/cmaze.o : cmaze.cpp
	g++ -o  $@ -c cmaze.cpp $(INCS) $(CCflags)


#### LINKS

maze : $(OBJ)
	g++ -o ../bin/maze.exe \
		$(OBJ) $(BOOSTLIB) $(WEXLIBS) $(STATLibs) $(OPT)

### CLEAN

.PHONY: clean
clean:
	del /q ..\obj