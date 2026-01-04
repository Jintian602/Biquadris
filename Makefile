CXX = g++-14
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -g
HEADERFLAGS = -c -x c++-system-header
LDFLAGS = -L/usr/X11R6/lib -lX11

# Source files in dependency order (all .cc files in root folder)
SOURCES = position.cc position-impl.cc \
          cell.cc cell-impl.cc \
          observer.cc observer-impl.cc \
          subject.cc subject-impl.cc \
          block.cc block-impl.cc \
          iblock.cc iblock-impl.cc \
          jblock.cc jblock-impl.cc \
          lblock.cc lblock-impl.cc \
          oblock.cc oblock-impl.cc \
          sblock.cc sblock-impl.cc \
          tblock.cc tblock-impl.cc \
          zblock.cc zblock-impl.cc \
          level.cc level-impl.cc \
          level0.cc level0-impl.cc \
          level1.cc level1-impl.cc \
          level2.cc level2-impl.cc \
          level3.cc level3-impl.cc \
          level4.cc level4-impl.cc \
          xwindow.cc xwindow-impl.cc \
          board.cc board-impl.cc \
          player.cc player-impl.cc \
          basicplayer.cc basicplayer-impl.cc \
          playereffect.cc playereffect-impl.cc \
          blindeffect.cc blindeffect-impl.cc \
          heavyeffect.cc heavyeffect-impl.cc \
          forceeffect.cc forceeffect-impl.cc \
          textobserver.cc textobserver-impl.cc \
          graphicsobserver.cc graphicsobserver-impl.cc \
          commandinterpreter.cc commandinterpreter-impl.cc \
          game.cc game-impl.cc \
          main.cc

OBJECTS = $(SOURCES:.cc=.o)
EXEC = biquadris

.PHONY: all clean headers rebuild

all: headers $(EXEC)

headers:
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) iostream
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) vector
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) memory
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) fstream
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) sstream
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) string
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) map
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) utility
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) algorithm
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) cstdlib
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) chrono
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) limits
	-$(CXX) $(CXXFLAGS) $(HEADERFLAGS) cstring

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all objects to create executable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(EXEC) $(OBJECTS)
	rm -rf gcm.cache

rebuild: clean all
