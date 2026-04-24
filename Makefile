main: main.c
	gcc -o main main.c -lm -lX11 -lraylib

clean:
	rm main
