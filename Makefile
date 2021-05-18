NAME = arch.exe
SRC =	src/pack/CompressIO.cpp \
		src/pack/Compressor.cpp \
		src/Dictionary.cpp \
		src/unpack/DecompressIO.cpp \
		src/unpack/Decompressor.cpp \
		src/main.cpp

CC = g++
HEADER_DIR	=	includes/
HEADER		=	includes/archiver.hpp \
				includes/CompressIO.hpp \
				includes/Compressor.hpp \
				includes/DecompressIO.hpp \
				includes/Decompressor.hpp \
				includes/Dictionary.hpp

CFLAGS = -g -I $(HEADER_DIR)

OBJ = $(patsubst %.cpp,%.o,$(SRC))

%.o: %.cpp $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
