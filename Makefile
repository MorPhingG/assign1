test_assign1_1 : dberror.o  storage_mgr.o test_assign1_1.o
		cc -o test_assign1_1 dberror.o storage_mgr.o test_assign1_1.o

dberror.o : dberror.c
		cc -c dberror.c

storage_mgr.o : storage_mgr.c
		cc -c storage_mgr.c

test_assign1_1.o : test_assign1_1.c
		cc -c test_assign1_1.c

clean :
		rm test_assign1_1 dberror.o storage_mgr.o test_assign1_1.o
