#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage_mgr.h"
#include "dberror.h"

// Create page file
RC createPageFile (char *fileName)
{
    FILE *fd = fopen(fileName, "w+");
    SM_PageHandle firstPage;
    int flag;
    // Create file if not exists
    if (fd == NULL)
    {
      return RC_FILE_NOT_FOUND;
    }
    firstPage = (SM_PageHandle) calloc(PAGE_SIZE, sizeof(char));
    // Fill the single page with '\0'
    flag = fwrite(firstPage, sizeof(char), PAGE_SIZE, fd);
    if (flag == -1)
    {
      return RC_WRITE_FAILED;
    }
    // free memory
    free(firstPage);
    // close file
    fclose(fd);
    return RC_OK;
}

// Open page file
RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
    int flag, endPos, length;
    FILE *fd = fopen(fileName, "r+");
    if (fd == NULL)
    {
      return RC_FILE_NOT_FOUND;
    }
    // move the pointer to the end of the file
    flag = fseek(fd, 0, SEEK_END);
    if (flag == 0)
    {
      // get the last position of the file
      endPos = ftell(fd);
      // get file length
      length = endPos + 1;
    }
    int totalNumPages = length/PAGE_SIZE;
    // move the pointer to the beginning of the file
    flag = fseek(fd, 0, SEEK_SET);
    // set file handle
    if (flag == 0)
      fHandle->curPagePos = 0;
    fHandle->fileName = fileName;
    fHandle->totalNumPages = totalNumPages;
    fHandle->mgmtInfo = fd;
    return RC_OK;
}

// Close page file
RC closePageFile (SM_FileHandle *fHandle)
{
    int flag = fclose(fHandle->mgmtInfo);
    if (flag == EOF)
    {
      return RC_FILE_NOT_FOUND;
    }
    else
    {
      fHandle->mgmtInfo= NULL;
      return RC_OK;
    }
}

// Destory page file
RC destroyPageFile (char *fileName)
{
    // Remove the file
    int flag = remove(fileName);
    if (flag == EOF)
        return RC_FILE_NOT_FOUND;
    else
      return RC_OK;
}

// Read Block
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int totalNumPages = fHandle->totalNumPages;
	int offset = pageNum * PAGE_SIZE * sizeof(char);
	FILE *fm = fHandle->mgmtInfo;

	// If the file has less than pageNum pages, the method should return RC_READ_NON_EXISTING_PAGE.
	if (totalNumPages >= pageNum)
	{
		// Set the file position and check whether it's successful.
		if (fseek(fm, offset, SEEK_SET) == 0)
		{
			// Read the file and check whether it's successful.
			if (fread(memPage, sizeof(char), PAGE_SIZE, fm) == PAGE_SIZE)
			{
				fHandle->curPagePos = pageNum;
				return RC_OK;
			}
			else
			{
				return RC_READ_FAILED;
			}
		}
		else
		{
			return RC_SEEK_FAILED;
		}
	}
	else
	{
		return RC_READ_NON_EXISTING_PAGE;
	}
}

int getBlockPos (SM_FileHandle *fHandle)
{
	// the current page position
	int pageNum = fHandle->curPagePos;
	return pageNum;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	// the number of first page is zero.
	int pageNum = 0;
	return readBlock(pageNum, fHandle, memPage);
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 	// the number of previous page is current minus one.
 	int pageNum = fHandle->curPagePos - 1;
 	return readBlock(pageNum, fHandle, memPage);
 }

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 	// the number of current page is curPagePos;
 	int pageNum = fHandle->curPagePos;
 	return readBlock(pageNum, fHandle, memPage);
 }

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
  	// the number of previous page is current add one.
 	int pageNum = fHandle->curPagePos + 1;
 	return readBlock(pageNum, fHandle, memPage);
 }

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 	// the number of last page is totalpages minus one.
 	int pageNum = fHandle->totalNumPages - 1;
 	return readBlock(pageNum, fHandle, memPage);
}

// write block
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    //check whether the file exists.
    if (fHandle == NULL)
    {
        return RC_FILE_HANDLE_NOT_INIT;
    }

    //if the total number of pages is smaller than the number of page you want to write, the write will be failed.
    if(fHandle->totalNumPages < pageNum || pageNum < 0)
    {
        return RC_WRITE_FAILED;
    }

    FILE *file = fHandle->mgmtInfo;

    //write a block into a page already exist in the file.
    if (fwrite(memPage, 1, PAGE_SIZE, file) == PAGE_SIZE)
    {
        return RC_OK;
    }
    else
    {
    return RC_WRITE_FAILED;
    }
}

//Write current block to the file.
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle->curPagePos<0||fHandle->curPagePos>fHandle->totalNumPages)
    {
        return RC_CURRENT_PAGE_NOT_FOUND;
    }
    else
    {
        return writeBlock(fHandle->curPagePos, fHandle, memPage);
    }
}

//write an empty block into the end of the file.
//increae the number of total page and the current page
RC appendEmptyBlock (SM_FileHandle *fHandle)
{

    FILE *file = fHandle->mgmtInfo;

    //allocate a memory block which has a same size with a page
    //set all the elememnts in the page to '\0'
    char *newPage = malloc(PAGE_SIZE);

    memset(newPage, '\0', PAGE_SIZE);

    //seek the end of the file.
    if(fseek(fHandle->mgmtInfo, 0, SEEK_END) != 0)
    {
        return RC_WRITE_FAILED;
    }

    //write the new page in the end of the file
    int result = fwrite(newPage, 1, PAGE_SIZE, fHandle->mgmtInfo);

    if(result == PAGE_SIZE)
    {
        // if write, the total number of page and the current page numebr plus one.
        fHandle->curPagePos += 1;
        fHandle->totalNumPages += 1;
        return RC_OK;
    }
    else
    {
        return RC_WRITE_FAILED;
    }
}

//ensure the capacity
//add the total number of page to expect number which is numberOfPages.
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
    if (numberOfPages > fHandle->totalNumPages)
    {
        //get the number of pages which are waiting to be added.
        int addPages = numberOfPages - fHandle->totalNumPages;

        int i;

        //use loop to add the page in the end of file.
        for(i = 0; i < addPages; i++){
            appendEmptyBlock(fHandle);
        }
    }
    return RC_OK;
}
