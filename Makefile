CC=gcc -w
INC=-Icutil/
OUT=bin/

SVM_SRC=svm/svm.c
SVM_OUT=$(OUT)svm

svm_build:
	$(CC) $(INC) -c $(SVM_SRC)
	$(CC) -o $(SVM_OUT) *.o
	rm *.o
