Name: Xu Ouyang
CWID: A20361390
Email: xouyang3@hawk.iit.edu

Name: Long Tang
CWID: A20365477
Email: ltang12@hawk.iit.edu

Name: Rui Zhang
CWID: A20365519
Email: rzhang50@hawk.iit.edu

github repository: https://github.com/MorPhingG/assign1.git

File:

README.txt
dberror.c
dberror.h
storage_mgr.c
storage_mgr.h
test_assign1_1.c
test_helper.h
Makefile


Function:

// File related methods
extern RC createPageFile (char *fileName):
create page file

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle):
open page file and set the file handle

extern RC closePageFile (SM_FileHandle *fHandle):
close page file and unlink file handle

extern RC destroyPageFile (char *fileName):
delete page file

// reading blocks from disc
extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage):
The method reads the pageNumth block from a file and stores its content in the memory pointed to by the memPage page handle. If the file has less than pageNum pages, the method should return RC_READ_NON_EXISTING_PAGE.

extern int getBlockPos (SM_FileHandle *fHandle):
Return the current page position in a file.

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
Read the first page in a file.

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
Read the previous page relative to the curPagePos of the file.

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
Read the current page relative to the curPagePos of the file.

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
Read the next page relative to the curPagePos of the file.

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
Read the last page in a file.

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)：
1. Check the handle whether is initialized.
2. Check whether the the total number of pages is smaller than the number of page you want to write, if yes, return failed.
3. Use the fseek() function to find the page in the beginning.
4. Use the fwrite() function to write a page, if the return value is same as the page size, the write is done successfully, then return RC_OK, or return the failed message.

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)：
Write a page to disk using either the current position or an absolute position.

extern RC appendEmptyBlock (SM_FileHandle *fHandle)：
1. Set a new page which has all the elements as ‘/0’.
2. Use the fseek() function  to find end of the file.
3. Use the fwrite() function to write an empty block into the end of file. 
4. The total page plus one and the current page plus one.

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)：
1. Check whether the total number of pages is smaller than the number of pages you want.
2. If yes, use the number of pages minus the total number of pages to find the page remaining to write.
3. Use the loop to write the pages into the file.


Test result:

// File related methods
createPageFile (char *fileName);

#########################################################################
[test_assign1_1.c-testFile.txt-L25-17:53:32] OK: expected true: File created successfully
#########################################################################

openPageFile (char *fileName, SM_FileHandle *fHandle);

#########################################################################
[test_assign1_1.c-testFile.txt-L27-17:53:32] OK: expected true: File opened successfully
[test_assign1_1.c-testFile.txt-L29-17:53:32] OK: expected true: Filename correct
[test_assign1_1.c-testFile.txt-L30-17:53:32] OK: expected true: There is only 1 page in the file
[test_assign1_1.c-testFile.txt-L31-17:53:32] OK: expected true: Current page position for the new file must be 0
[test_assign1_1.c-testFile.txt-L32-17:53:32] OK: expected true: mgmtInfo correct
#########################################################################

closePageFile (SM_FileHandle *fHandle);

#########################################################################
[test_assign1_1.c-testFile.txt-L34-17:53:32] OK: expected true: File closed successfully
#########################################################################

destroyPageFile (char *fileName);

#########################################################################
[test_assign1_1.c-testFile.txt-L36-17:53:32] OK: expected true: File destoryed successfully
#########################################################################

operation without opening the file

#########################################################################
[test_assign1_1.c-testFile.txt-L38-17:53:32] OK: expected an error and was RC <1>: File is not in the handle list
[test_assign1_1.c-testFile.txt-L39-17:53:32] OK: expected an error and was RC <1>: File not found
#########################################################################

[test_assign1_1.c-testFile.txt-L41-17:53:32] OK: finished test


// reading blocks from disc
extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage):
set the pageNum=0, and then read this 0th block.

#######################################################################
[test_assign1_1.c-testFile.txt-L57-17:53:32] OK: expected true: We read the block given pageNum

[test_assign1_1.c-testFile.txt-L57-17:53:32] OK: expected true: We read the block given pageNum

[test_assign1_1.c-testFile.txt-L57-17:53:32] OK: expected true: We read the block given pageNum

[test_assign1_1.c-testFile.txt-L57-17:53:32] OK: expected true: We read the block given pageNum

[test_assign1_1.c-testFile.txt-L57-17:53:32] OK: expected true: We read the block given pageNum
#######################################################################
——————————————————————————————————————————-

extern int getBlockPos (SM_FileHandle *fHandle):
get the current block position.

#######################################################################
[test_assign1_1.c-testFile.txt-L61-17:53:32] OK: expected true: We get the block position
#######################################################################

——————————————————————————————————————————-

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
read the first block.

#######################################################################
[test_assign1_1.c-testFile.txt-L66-17:53:32] OK: expected true: We read the first block

[test_assign1_1.c-testFile.txt-L66-17:53:32] OK: expected true: We read the first block

[test_assign1_1.c-testFile.txt-L66-17:53:32] OK: expected true: We read the first block

[test_assign1_1.c-testFile.txt-L66-17:53:32] OK: expected true: We read the first block

[test_assign1_1.c-testFile.txt-L66-17:53:32] OK: expected true: We read the first block
#######################################################################


——————————————————————————————————————————-

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
after appending 6 blocks, read the 5th block(index start from 0).
#######################################################################
[test_assign1_1.c-testFile.txt-L98-17:53:32] OK: expected true: We read the current block

[test_assign1_1.c-testFile.txt-L98-17:53:32] OK: expected true: We read the current block

[test_assign1_1.c-testFile.txt-L98-17:53:32] OK: expected true: We read the current block

[test_assign1_1.c-testFile.txt-L98-17:53:32] OK: expected true: We read the current block

[test_assign1_1.c-testFile.txt-L98-17:53:32] OK: expected true: We read the current block

the number of current block: 5
#######################################################################

——————————————————————————————————————————-

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
read the 4th block.

#########################################################################
[test_assign1_1.c-testFile.txt-L104-17:53:32] OK: expected true: We read the previous block

[test_assign1_1.c-testFile.txt-L104-17:53:32] OK: expected true: We read the previous block

[test_assign1_1.c-testFile.txt-L104-17:53:32] OK: expected true: We read the previous block

[test_assign1_1.c-testFile.txt-L104-17:53:32] OK: expected true: We read the previous block

[test_assign1_1.c-testFile.txt-L104-17:53:32] OK: expected true: We read the previous block

the number of previous block: 4
#########################################################################

——————————————————————————————————————————-

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
read the 5th block:

#########################################################################
[test_assign1_1.c-testFile.txt-L110-17:53:32] OK: expected true: We read the next block

[test_assign1_1.c-testFile.txt-L110-17:53:32] OK: expected true: We read the next block

[test_assign1_1.c-testFile.txt-L110-17:53:32] OK: expected true: We read the next block

[test_assign1_1.c-testFile.txt-L110-17:53:32] OK: expected true: We read the next block

[test_assign1_1.c-testFile.txt-L110-17:53:32] OK: expected true: We read the next block

the number of next block: 5
#########################################################################

——————————————————————————————————————————-

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
read the last block.

#########################################################################
[test_assign1_1.c-testFile.txt-L72-17:53:32] OK: expected true: We read the last block

[test_assign1_1.c-testFile.txt-L72-17:53:32] OK: expected true: We read the last block

[test_assign1_1.c-testFile.txt-L72-17:53:32] OK: expected true: We read the last block

[test_assign1_1.c-testFile.txt-L72-17:53:32] OK: expected true: We read the last block

[test_assign1_1.c-testFile.txt-L72-17:53:32] OK: expected true: We read the last block
#########################################################################


