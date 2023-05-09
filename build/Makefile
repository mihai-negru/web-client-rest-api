CC 			:= 	g++
CFLAGS 		:= 	-std=c++17 -g -Wall -Wextra

VALGRIND	:= valgrind
VAL_FLAGS	:= --leak-check=full \
			   --show-leak-kinds=all \
			   --track-origins=yes \
			   -s

RM			:= 	rm
RFLAGS		:= 	-rf

ZIP			:=	zip
ZIP_NAME	:=	323CD_Negru_Mihai_Tema3.zip
ZIP_FLAGS	:= -r
ZIP_FILES	:= 	src/ build/ README.md

SRC			:= 	../src
SRC_FILES	:= $(wildcard $(SRC)/*.cpp)

EXEC		:= 	main
O_FILES		:= 	$(patsubst $(SRC)/%.cpp,%.o,$(SRC_FILES))

all: $(EXEC) clean_o

%.o: $(SRC)/%.cpp
	@$(CC) $(CFLAGS) -c $<

$(EXEC): $(O_FILES)
	@$(CC) $^ -o $@

val: $(EXEC) clean_o
	@$(VALGRIND) $(VAL_FLAGS) ./$(EXEC)

zip_project: $(ZIP_FILES)
	@$(ZIP) $(ZIP_FLAGS) $(ZIP_NAME) $(ZIP_FILES)

clean:
	@$(RM) $(RFLAGS) $(EXEC) $(O_FILES)

clean_o:
	@$(RM) $(RFLAGS) $(O_FILES)