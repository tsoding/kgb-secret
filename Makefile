all: kgb-unlock secret.raw

kgb-unlock: kgb-unlock.c
	gcc -Wall -Werror -o kgb-unlock kgb-unlock.c

secret.raw: secret.asm
	nasm -fbin -o secret.raw secret.asm
