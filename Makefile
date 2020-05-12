VERSION := -std=c11
CC := gcc
FILES := main.c
FILE_OUT := main
LVL_OPTIM := -O2
all: $(FILES)
	$(CC) -g $(VERSION) $(LVL_OPTIM) $(FILES) -o $(FILE_OUT)
clear: $(FILE_OUT)
	rm $(FILE_OUT)
