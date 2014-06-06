BASE_LIBS=
BASE_CFLAGS=-c -g -O2 -Wall -std=c++0x

PROGRAMNAME=blockattack
PROGRAMNAME_OUT=../../game/blockattack

O_FILES=${PROGRAMNAME}.o

BASE_LIBS+=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio  -lphysfs -ljson-c
O_FILES+= BlockMenu.o sago/FrameCounter.o sago/SagoCommandQueue.o sago/SagoDataHolder.o sago/SagoMenu.o sago/SagoMisc.o sago/SagoSprite.o sago/SagoSpriteHolder.o
O_FILES+= sago/SagoMusicBackground.o sago/GameStateManager.o CommandProcessor.o BlockClient.o

total: ${PROGRAMNAME_OUT}

clean: 
	rm -f */*.o *.o *.P */*.P ${PROGRAMNAME_OUT}

${PROGRAMNAME_OUT}: $(O_FILES)
	$(CXX) -O -o ${PROGRAMNAME_OUT} $(O_FILES) $(BASE_LIBS)

%.o : %.cpp
	$(CXX) -MD ${BASE_CFLAGS} -o $@ $<
	@cp $*.d $*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
             rm -f $*.d

-include *.P
