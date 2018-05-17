CC       =  gcc
CFLAGS   = -Wall -O2 -g
LIB      = -lSDL -lGLU -lGL -lm -lSDL_image

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = -I include

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o, $(SRC_FILES))

RM       = rm -f
BIN      = flapimac
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz

all : $(BIN)

$(BIN) : $(OBJ_FILES)
	$(CC) -o $(BIN) $(OBJ_FILES) $(CFLAGS) $(LIB)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN) &"
	@echo "--------------------------------------------------------------"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@echo "compiling"
	@mkdir -p "$(@D)"
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_PATH)
	@echo "done..."

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) $(OBJ_FILES) $(BIN)

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)
