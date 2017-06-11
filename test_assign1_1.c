#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

char *testName = "testFile.txt";

static void testFileRelatedMethods(void);
static void testReadBlockMethods(void);

int main (void)
{
    testFileRelatedMethods();
    testReadBlockMethods();
    return 0;
}

void testFileRelatedMethods(void)
{
    SM_FileHandle fh;

    // create page file
    ASSERT_TRUE((createPageFile (testName) == RC_OK), "File created successfully");
    // open page file
    ASSERT_TRUE((openPageFile (testName, &fh) == RC_OK), "File opened successfully");
    // test the file handle
    ASSERT_TRUE(fh.fileName == testName, "Filename correct");
    ASSERT_TRUE((fh.totalNumPages == 1), "There is only 1 page in the file");
    ASSERT_TRUE((fh.curPagePos == 0), "Current page position for the new file must be 0");
    ASSERT_TRUE((fh.mgmtInfo != NULL), "mgmtInfo correct");
    // close page file
    ASSERT_TRUE((closePageFile (&fh) == RC_OK), "File closed successfully");
    // destory page file
    ASSERT_TRUE((destroyPageFile (testName) == RC_OK), "File destoryed successfully");
    
    ASSERT_ERROR((fh.mgmtInfo == NULL), "File is not in the handle list");
    ASSERT_ERROR((openPageFile (testName, &fh) == RC_FILE_NOT_FOUND), "File not found");

    TEST_DONE();
}

void testReadBlockMethods(void)
{
    SM_FileHandle fh;
    SM_PageHandle ph;
    ph = (SM_PageHandle) malloc(PAGE_SIZE);
    int i;

    ASSERT_TRUE((createPageFile (testName) == RC_OK), "File created successfully");
    ASSERT_TRUE((openPageFile (testName, &fh) == RC_OK), "File opened successfully");

    // read block, and the firt block should be empty
    TEST_CHECK(readBlock(0, &fh, ph));
    for (i=0; i<5; i++)
    	ASSERT_TRUE((ph[i] == 0), "We read the block given pageNum");

    // get block position
    TEST_CHECK(getBlockPos (&fh));
    ASSERT_TRUE((fh.curPagePos == getBlockPos(&fh)), "We get the block position");

    // read the first block, and the block should be empty
    TEST_CHECK(readFirstBlock (&fh, ph));
    for (i=0; i<5; i++)
      ASSERT_TRUE((ph[i] == 0), "We read the first block");


    // read the last block
    TEST_CHECK(readLastBlock (&fh, ph));
    for (i=0; i<5; i++)
      ASSERT_TRUE((ph[i] == 0), "We read the last block");


    TEST_CHECK(appendEmptyBlock(&fh));
    // ASSERT_TRUE(fh.totalNumPages == 2, "After append an empty block, the totalNumPages changes to '2', which was '1' before the operation.");
    // ASSERT_TRUE(fh.curPagePos == 1, "After append an empty block, the curPagePos changes to '1', which was '0' before the operation.");

    for(i = 0; i < PAGE_SIZE; i++){
        ph[i] = (i % 10) + '0';
    }
    ASSERT_TRUE(writeBlock(10, &fh, ph) == RC_WRITE_FAILED, "Write 10 pages to the file which have totalNumber of pages as 2 cannot implemented.");


    TEST_CHECK(writeBlock(1, &fh, ph));

    TEST_CHECK(ensureCapacity(6, &fh));
    ASSERT_TRUE(fh.totalNumPages == 6, "After the ensureCapacity(), the totalNumPage changed to the 6 which is expected.");

    for(i = 0; i < PAGE_SIZE; i++)
        ph[i] = (i % 10) + '0';
    TEST_CHECK(writeCurrentBlock(&fh, ph));
    printf("write current block, the number of current block : %d\n", fh.curPagePos);

    // read the current block, and the block should be empty
    TEST_CHECK(readCurrentBlock (&fh, ph));
    for (i=0; i<5; i++)
      ASSERT_TRUE((ph[i] == 0), "We read the current block");
    printf("the number of current block: %d\n", fh.curPagePos);

    // read the previous block
    TEST_CHECK(readPreviousBlock (&fh, ph));
    for (i=0; i<5; i++)
      ASSERT_TRUE((ph[i] == 0), "We read the previous block");
    printf("the number of previous block: %d\n", fh.curPagePos);

    // read the next block
    TEST_CHECK(readNextBlock (&fh, ph));
    for (i=0; i<5; i++)
      ASSERT_TRUE((ph[i] == 0), "We read the next block");
    printf("the number of next block: %d\n", fh.curPagePos);

    TEST_DONE();

}
