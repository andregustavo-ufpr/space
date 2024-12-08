CC = gcc
CFLAGS = -Wall -g -Imotion -Iattack -Ientities -Ialien -IC:\allegro-x86_64-w64-mingw32-gcc-13.2.0-posix-seh-static-5.2.9.0\allegro\include
LDFLAGS = -LC:\allegro-x86_64-w64-mingw32-gcc-13.2.0-posix-seh-static-5.2.9.0\allegro\lib -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives

TARGET = space.exe
SRC = main.c motion/motion.c attack/attack.c entities/entity.c alien/alien.c
OBJ = $(SRC:.c=.o)

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o motion/*.o entities/*.o attack/*.o alien/*.o $(TARGET)