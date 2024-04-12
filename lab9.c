#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType ***pRecords;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i, j;
	for (i=0;i<hashSz;++i)
	{
		j=0;
		// if index is occupied with any records, print all
		if((pHashArray->pRecords[i]) != NULL)
		{
			if(j==0){
				printf("\n");
			}
			while(pHashArray->pRecords[i][j] != NULL){
				if(j==0)
				{
					printf("index %d -> %d, %c, %d ", i, pHashArray->pRecords[i][j]->id, pHashArray->pRecords[i][j]->name, pHashArray->pRecords[i][j]->order);
				}
				else
				{
					printf("-> %d, %c, %d ", pHashArray->pRecords[i][j]->id, pHashArray->pRecords[i][j]->name, pHashArray->pRecords[i][j]->order);
				}
				j++;
			}
		}
	}
	printf("\n");
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	int hashValue;
	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	int j = 0;
	struct HashType *hashtable = (struct HashType*)malloc(sizeof(struct HashType));
	hashtable->pRecords = (struct RecordType***)malloc(23 * sizeof(struct RecordType*));
	for(int i = 0; i < 23; i++)
	{
		hashtable->pRecords[i] = NULL;
	}
	for(int i = 0; i < recordSz; i++)
	{
		hashValue = hash(pRecords[i].id);
		if (hashtable->pRecords[hashValue] == NULL)
		{
			hashtable->pRecords[hashValue] = (struct RecordType **)malloc(sizeof(struct RecordType **));
			hashtable->pRecords[hashValue][0] = &pRecords[i];
			hashtable->pRecords[hashValue][1] = NULL;
		}
		else
		{
			j=0;
			while(hashtable->pRecords[hashValue][j] != NULL)
			{
				j++;
			}
			hashtable->pRecords[hashValue][j] = &pRecords[i];
			hashtable->pRecords[hashValue][j+1] = NULL;
		}
	}
	displayRecordsInHash(hashtable, 23);
}
