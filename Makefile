CC=gcc -w
INC=-Icutil/ -Icommon/
OUT=bin/

SVM_SRC=svm/svm.c
SVM_OUT=$(OUT)svm

SASM_SRC=sasm/*.c
SASM_OUT=$(OUT)sasm

svm_build:
	$(CC) $(INC) -c $(SVM_SRC)
	$(CC) -o $(SVM_OUT) *.o
	rm *.o

sasm_build:
	$(CC) $(INC) -c $(SASM_SRC)
	$(CC) -o $(SASM_OUT) *.o
	rm *.o

