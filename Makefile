CC 			:= 	g++
CFLAGS 		:= 	-std=c++17 -g -Wall -Wextra

VALGRIND	:= valgrind
VAL_FLAGS	:= --leak-check=full \
			   --show-leak-kinds=all \
			   -s

RM			:= 	rm
RFLAGS		:= 	-rf

ZIP			:=	zip
ZIP_NAME	:=	323CD_Negru_Mihai_Tema3.zip
ZIP_FLAGS	:= -r
ZIP_FILES	:= 	src/ Makefile README.md

SRC			:= 	src
SRC_FILES	:= $(wildcard $(SRC)/*.cpp)

INCLUDE		:= $(SRC)/include

BUILD		:= build

EXEC		:= 	main
O_FILES		:= 	$(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))

all: create_dir $(EXEC)

create_dir:
	@if [ ! -d $(BUILD) ]; then mkdir -p $(BUILD); fi

$(BUILD)/%.o: $(SRC)/%.cpp
	@$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): $(O_FILES)
	@$(CC) $^ -o $@

val: $(EXEC)
	@$(VALGRIND) $(VAL_FLAGS) ./$(EXEC)

run: $(EXEC)
	@./$(EXEC)

zip_project: $(ZIP_FILES)
	@$(ZIP) $(ZIP_FLAGS) $(ZIP_NAME) $(ZIP_FILES)

clean:
	@$(RM) $(RFLAGS) $(EXEC) $(BUILD)

clean_build:
	@$(RM) $(RFLAGS) $(BUILD)
