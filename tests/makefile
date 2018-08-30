ifeq ($(VENDOR),cray)
	MPICXX=cc -dynamic
else
	MPICXX=mpic++
endif

CXX=g++

CCFLAGS=-std=c++11
LDFLAGS=-lcasa_tables -lcasa_casa
STMANFILES=libadios2stman.so
TARGET=writeScalar \
	   writeDirArr \
	   writeFixArr \
	   writeArbArr \
	   readArr \
	   readArrColumn \
	   readArrColumnSlice \
	   readScalar

mpi:writeMPI.cc $(STMANFILES)
	$(MPICXX) -g writeMPI.cc $(CCFLAGS) $(LDFLAGS) $(STMANFILES) -o writeMPI -DHAVE_MPI

all: $(TARGET) cl

$(TARGET): $(TARGET:=.cc) $(STMANFILES)
	$(CXX) $@.cc -o $@ $(CCFLAGS) $(LDFLAGS) $(STMANFILES) 



$(STMANFILES):  
	cd ..; make ln;

cl:
	rm -rf *.casa *.out

clean:cl
	rm -rf $(TARGET) *.dSYM *.so *.table

re: clean all
