
CFLAG:= -I../Include
TARGET := ../Library/libAPC.so
SRCS := $(wildcard *.c)

${TARGET} : ${SRCS}
	gcc -fPIC -shared ${CFLAG} $^ -o $@

clean:
	rm ${TARGET}
