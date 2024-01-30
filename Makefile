# Definitions de macros
OUT = projet
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
CXXFILES = error_squarecell.cc fourmi.cc fourmiliere.cc \
message.cc nourriture.cc projet.cc simulation.cc squarecell.cc \
graphic.cc gui.cc
OFILES = error_squarecell.o fourmi.o fourmiliere.o \
message.o nourriture.o projet.o simulation.o squarecell.o \
graphic.o gui.o

# Definition de la premiere regle

all : $(OUT)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)
	
# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	
clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o $(OUT) *.x *.cc~ *.h~ projet

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
error_squarecell.o: error_squarecell.cc error_squarecell.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
fourmi.o: fourmi.cc fourmi.h squarecell.h error_squarecell.h constantes.h \
 message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
fourmiliere.o: fourmiliere.cc fourmiliere.h fourmi.h squarecell.h \
 error_squarecell.h constantes.h message.h nourriture.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
nourriture.o: nourriture.cc nourriture.h squarecell.h error_squarecell.h \
 message.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
projet.o: projet.cc simulation.h squarecell.h error_squarecell.h \
 fourmiliere.h fourmi.h constantes.h message.h nourriture.h \
 gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
simulation.o: simulation.cc simulation.h squarecell.h error_squarecell.h \
 fourmiliere.h fourmi.h constantes.h message.h nourriture.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
squarecell.o: squarecell.cc squarecell.h error_squarecell.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
graphic.o: graphic.cc graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
gui.o: gui.cc gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
