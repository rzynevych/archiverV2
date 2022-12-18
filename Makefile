NAME = arch.exe
SRC =	src/pack/CompressIO.cpp \
		src/pack/Compressor.cpp \
		src/pack/ParallelCompressor.cpp \
		src/Dictionary.cpp \
		src/unpack/DecompressIO.cpp \
		src/unpack/Decompressor.cpp \
		src/main.cpp

CC = clang++
HEADER_DIR	=	includes/
HEADER		=	includes/archiver.hpp \
				includes/Dictionary.hpp \
				includes/CompressIO.hpp \
				includes/Compressor.hpp \
				includes/ParallelCompressor.hpp \
				includes/DecompressIO.hpp \
				includes/Decompressor.hpp

CFLAGS = -g -std=c++11 -I $(HEADER_DIR)

OBJ = $(patsubst %.cpp,%.o,$(SRC))

%.o: %.cpp $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -pthread -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
