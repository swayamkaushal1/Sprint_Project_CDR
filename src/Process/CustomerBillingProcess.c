#include "../../Header/custHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 
// Define the global variables declared in header
Customer *hashTable[HASH_SIZE];
int totalRecords = 0;
 
// Hash function (simple & fast)
 
unsigned int hashFunction(long key)
{
 
    return (unsigned int)(key % HASH_SIZE);
}
 
// Create a new customer entry
 
Customer *createCustomer(long msisdn, const char *operatorName, int operatorCode)
{
 
    Customer *cust = (Customer *)malloc(sizeof(Customer));
 
    cust->msisdn = msisdn;
 
    strcpy(cust->operatorName, operatorName);
 
    cust->operatorCode = operatorCode;
 
    cust->inVoiceWithin = cust->outVoiceWithin = 0;
 
    cust->inVoiceOutside = cust->outVoiceOutside = 0;
 
    cust->smsInWithin = cust->smsOutWithin = 0;
 
    cust->smsInOutside = cust->smsOutOutside = 0;
 
    cust->mbDownload = cust->mbUpload = 0;
 
    cust->next = NULL;
 
    return cust;
}
 
// Get or create a customer in the hash map
 
Customer *getCustomer(long msisdn, const char *operatorName, int operatorCode)
{
 
    unsigned int index = hashFunction(msisdn);
 
    Customer *curr = hashTable[index];
 
    while (curr)
    {
 
        if (curr->msisdn == msisdn)
 
            return curr;
 
        curr = curr->next;
    }
 
    Customer *newCust = createCustomer(msisdn, operatorName, operatorCode);
 
    newCust->next = hashTable[index];
 
    hashTable[index] = newCust;
 
    return newCust;
}
 
// Process data.cdr file once
 
void processCDRFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "Error opening CDR file '%s': %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
 
    printf("Processing CDR file: %s\n", filename);
 
    char line[512];
    totalRecords = 0;
 
    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline characters
        line[strcspn(line, "\r\n")] = 0;
 
        long msisdn = 0, thirdPartyMsisdn = 0;
        char opName[64] = {0}, callType[16] = {0};
        int opCode = 0, thirdPartyOpCode = 0;
        float duration = 0, download = 0, upload = 0;
 
        //Correct format parsing
        // MSISDN|OPERATOR NAME|OPERATOR CODE|CALL TYPE|DURATION|DOWNLOAD|UPLOAD|THIRD PARTY MSISDN|THIRD PARTY OPERATOR
        int matched = sscanf(line, "%ld|%63[^|]|%d|%15[^|]|%f|%f|%f|%ld|%d",
&msisdn, opName, &opCode, callType,
&duration, &download, &upload,
&thirdPartyMsisdn, &thirdPartyOpCode);
 
        if (matched < 9)
        {
            // Handle GPRS line with missing third party MSISDN (two consecutive ||)
            matched = sscanf(line, "%ld|%63[^|]|%d|%15[^|]|%f|%f|%f||%d",
&msisdn, opName, &opCode, callType,
&duration, &download, &upload, &thirdPartyOpCode);
            if (matched < 8)
                continue; // skip invalid or partial lines
        }
 
        Customer *cust = getCustomer(msisdn, opName, opCode);
        int sameOperator = (opCode == thirdPartyOpCode);
 
        // Update based on call type
        if (strcmp(callType, "MOC") == 0)
        {
            if (sameOperator)
                cust->outVoiceWithin += duration;
            else
                cust->outVoiceOutside += duration;
        }
        else if (strcmp(callType, "MTC") == 0)
        {
            if (sameOperator)
                cust->inVoiceWithin += duration;
            else
                cust->inVoiceOutside += duration;
        }
        else if (strcmp(callType, "SMS-MO") == 0)
        {
            if (sameOperator)
                cust->smsOutWithin++;
            else
                cust->smsOutOutside++;
        }
        else if (strcmp(callType, "SMS-MT") == 0)
        {
            if (sameOperator)
                cust->smsInWithin++;
            else
                cust->smsInOutside++;
        }
        else if (strcmp(callType, "GPRS") == 0)
        {
            cust->mbDownload += download;
            cust->mbUpload += upload;
        }
 
        totalRecords++;
    }
 
    fclose(fp);
    printf("Processed %d records from %s\n", totalRecords, filename);
}
 
// Write all processed data to cb.txt
 
void writeCBFile(const char *outputFile)
{
    FILE *fp = fopen(outputFile, "w");
    if (!fp)
    {
        fprintf(stderr, "Error creating output file '%s': %s\n", outputFile, strerror(errno));
        exit(EXIT_FAILURE);
    }
    int customerCount = 0;
    fprintf(fp, "#Customers Data Base:\n");
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Customer *cust = hashTable[i];
        while (cust)
        {
            fprintf(fp, "\nCustomer ID: %ld (%s)\n", cust->msisdn, cust->operatorName);
            fprintf(fp, "* Services within the mobile operator *\n");
            fprintf(fp, "Incoming voice call durations: %.2f\n", cust->inVoiceWithin);
            fprintf(fp, "Outgoing voice call durations: %.2f\n", cust->outVoiceWithin);
            fprintf(fp, "Incoming SMS messages: %d\n", cust->smsInWithin);
            fprintf(fp, "Outgoing SMS messages: %d\n", cust->smsOutWithin);
            fprintf(fp, "* Services outside the mobile operator *\n");
            fprintf(fp, "Incoming voice call durations: %.2f\n", cust->inVoiceOutside);
            fprintf(fp, "Outgoing voice call durations: %.2f\n", cust->outVoiceOutside);
            fprintf(fp, "Incoming SMS messages: %d\n", cust->smsInOutside);
            fprintf(fp, "Outgoing SMS messages: %d\n", cust->smsOutOutside);
            fprintf(fp, "* Internet use *\n");
            fprintf(fp, "MB downloaded: %.2f | MB uploaded: %.2f\n",
                    cust->mbDownload, cust->mbUpload);
            fprintf(fp, "----------------------------------------\n");
            customerCount++;
            cust = cust->next;
        }
    }
    fclose(fp);
    printf("Output written successfully to %s (%d customers processed)\n", outputFile, customerCount);
 
}
 
// MAIN CALLING FUNCTION — now with proper paths
 
void CustomerBillingFile(const char *input_path, const char *output_path)
{
    for (int i = 0; i < HASH_SIZE; i++)
 
        hashTable[i] = NULL;
    processCDRFile(input_path);
    writeCBFile(output_path);
}