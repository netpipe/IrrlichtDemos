#OSRPG Team Internal Build-Maker
#(c) 2006 Ivan "Kai SD" Korystin
#Version 0.4
#License: MIT

#==================================================
#Options
#==================================================
SRC_PATH = "../../"
DATA_PATH = "../../../branches/maggun"
PROJECT = "maggun"
EXEC = "game"
SETTINGS = "settings"
IRRLIB = "/usr/local/lib/libIrrlicht.so"
RUN_CMD = "LD_LIBRARY_PATH=lib/"
#==================================================
#Some internal variables
YR=`date +%Y`
MD=`date +%d`
MM=`date +%m`
#==================================================

help:
	@echo
	@echo '"make release" will create a standalone copy of a game'
	@echo '"make clean" will delete a standalone copy of a game'
	@echo '"make dist" will crerate a tar.bz2 archive with a game.'
	@echo '"make src" will create a tar.bz2 archive with sources only and no data.'
	@echo '"make fullsrc" will create a tar.bz2 archive with sources and data. Should be enougth to build a game (Irrlicht and ODE required).'
	@echo '"make help" will just let you to read this message again.'

release:
	@echo CREATING STANDALONE BUILD
	@echo Building revision
	@make -C $(SRC_PATH)
	@echo Copying Data
	@tar -cvf data.tar -C "$(DATA_PATH)/" --exclude="*.svn"  data/ > /dev/null
	@tar -xvf data.tar
	@rm data.tar
	@echo Copying Libs
	@mkdir lib
	@cp -uL $(IRRLIB) lib/
	@echo Copying binary
	@cp "$(SRC_PATH)/$(EXEC)" ./
	@echo Copying settings
	@echo "DATA_DIR=data/" > $(SETTINGS)
	@cat "$(SRC_PATH)/$(SETTINGS)" | grep -v DATA_DIR= >> $(SETTINGS)
	@echo Creating run.sh
	@echo "$(RUN_CMD) ./$(EXEC)" > run.sh
	@chmod 755 run.sh
	@echo All Done.
	@echo You can now run the project with \"./run.sh\" or \"make run\".
	@echo You can clean everything with \"make clean\".

run:
	./run.sh

clean:
	@echo Cleaning
	@rm -R data
	@rm -R lib
	@rm $(EXEC)
	@rm $(SETTINGS)
	@rm run.sh

dist:
	@echo Creating archive
	@echo Building revision
	@make -C $(SRC_PATH)
	@echo Creating Game Archive
	@mkdir $(PROJECT)-$(YR)$(MM)$(MD)
	@echo Copying Data
	@tar -cvf data.tar -C "$(DATA_PATH)/" --exclude="*.svn"  data/ > /dev/null
	@tar -xvf data.tar -C $(PROJECT)-$(YR)$(MM)$(MD)
	@rm data.tar
	@echo Copying Libs
	@mkdir $(PROJECT)-$(YR)$(MM)$(MD)/lib
	@cp -uL $(IRRLIB) $(PROJECT)-$(YR)$(MM)$(MD)/lib/
	@echo Copying binary
	@cp "$(SRC_PATH)/$(EXEC)" $(PROJECT)-$(YR)$(MM)$(MD)/
	@echo Copying settings
	@echo "DATA_DIR=data/" > $(PROJECT)-$(YR)$(MM)$(MD)/$(SETTINGS)
	@cat "$(SRC_PATH)/$(SETTINGS)" | grep -v DATA_DIR= >> $(PROJECT)-$(YR)$(MM)$(MD)/$(SETTINGS)
	@echo Creating run.sh
	@echo "$(RUN_CMD) ./$(EXEC)" > $(PROJECT)-$(YR)$(MM)$(MD)/run.sh
	@chmod 755 $(PROJECT)-$(YR)$(MM)$(MD)/run.sh
	@echo Compressing...
	@tar -cvjpf $(PROJECT)-$(YR)$(MM)$(MD).tar.bz2 $(PROJECT)-$(YR)$(MM)$(MD)/
	@chmod +w -R $(PROJECT)-$(YR)$(MM)$(MD)
	@rm -R $(PROJECT)-$(YR)$(MM)$(MD)
	@echo Done

src:
	@echo Creating source archive
	@mkdir $(PROJECT)-$(YR)$(MM)$(MD)-src
	@cp -R $(SRC_PATH)/src $(PROJECT)-$(YR)$(MM)$(MD)-src/
	@cp $(SRC_PATH)/Makefile $(SRC_PATH)/BUGS $(SRC_PATH)/CHANGELOG $(SRC_PATH)/Doxyfile $(SRC_PATH)/README $(SRC_PATH)/TODO $(SRC_PATH)/settings $(PROJECT)-$(YR)$(MM)$(MD)-src/
	@echo Compressing...
	@tar -cvjf $(PROJECT)-$(YR)$(MM)$(MD)-src.tar.bz2 --exclude="*.svn" $(PROJECT)-$(YR)$(MM)$(MD)-src/ > /dev/null
	@chmod +w -R $(PROJECT)-$(YR)$(MM)$(MD)-src
	@rm -R $(PROJECT)-$(YR)$(MM)$(MD)-src
	@echo Done

fullsrc:
	@echo Creating full source and data archive
	@mkdir $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc
	@cp -R $(SRC_PATH)/src $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc/
	@cp $(SRC_PATH)/Makefile $(SRC_PATH)/BUGS $(SRC_PATH)/CHANGELOG $(SRC_PATH)/Doxyfile $(SRC_PATH)/README $(SRC_PATH)/TODO $(SRC_PATH)/settings $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc/
	@echo Copying Data
	@tar -cvf data.tar -C "$(DATA_PATH)/" --exclude="*.svn"  data/ > /dev/null
	@tar -xvf data.tar -C $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc
	@rm data.tar
	@echo Compressing...
	@tar -cvjf $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc.tar.bz2 --exclude="*.svn" $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc/ > /dev/null
	@chmod +w -R $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc
	@rm -R $(PROJECT)-$(YR)$(MM)$(MD)-fullsrc
	@echo Done