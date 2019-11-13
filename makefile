# Example GLFW21
EXE=finalProject

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
errcheck.cpp.o: errcheck.cpp CSCIx229.h
fatal.cpp.o: fatal.cpp CSCIx229.h
finalProject.cpp.o: glfw21.cpp CSCIx229.h
loadtexbmp.cpp.o: loadtexbmp.cpp CSCIx229.h
print-dl.cpp.o: print-dl.cpp CSCIx229.h
mathHelpers.cpp.o: mathHelpers.cpp CSCIx229.h
project.cpp.o: project.cpp CSCIx229.h
drawLibrary.cpp.o: drawLibrary.cpp CSCIx229.h
Hangar.cpp.o: Hangar.cpp CSCIx229.h
Camera.cpp.o: Camera.cpp CSCIx229.h
XB70Bomber.cpp.o: XB70Bomber.cpp CSCIx229.h
FighterJet.cpp.o: FighterJet.o CSCIx229.h

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
finalProject:finalProject.o fatal.o loadtexbmp.o project.o errcheck.o print-dl.o mathHelpers.o drawLibrary.o Hangar.o Camera.o XB70Bomber.o FighterJet.o
	g++ -O3 -o $@ $^ Camera.cpp Hangar.cpp  -lglfw -lGLU -lGL -lm 

#  Clean
clean:
	$(CLEAN)
