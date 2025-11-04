#include "../../Header/custHeader.h"

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

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        long msisdn;
        char opName[64], callType[16], destOp[64];
        int opCode, destCode;
        float duration = 0, download = 0, upload = 0;
        char dest[64];

        // Parse CDR line
        int matched = sscanf(line, "%ld|%63[^|]|%d|%15[^|]|%f|%f|%f|%63[^\n]",
                         &msisdn, opName, &opCode, callType, &duration,
                         &download, &upload, dest);

        if (matched < 7) continue;

        // Get destination code
        char *slash = strrchr(dest, '/');
        destCode = slash ? atoi(slash + 1) : opCode;

        Customer *cust = getCustomer(msisdn, opName, opCode);
        
        // Update statistics based on call type
        if (strcmp(callType, "MOC") == 0)
        {
            if (opCode == destCode)
                cust->outVoiceWithin += duration;
            else
                cust->outVoiceOutside += duration;
        }
        else if (strcmp(callType, "MTC") == 0)
        {
            if (opCode == destCode)
                cust->inVoiceWithin += duration;
            else
                cust->inVoiceOutside += duration;
        }
        else if (strcmp(callType, "SMS-MO") == 0)
        {
            if (opCode == destCode)
                cust->smsOutWithin++;
            else
                cust->smsOutOutside++;
        }
        else if (strcmp(callType, "SMS-MT") == 0)
        {
            if (opCode == destCode)
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

    fprintf(fp, "#Customers Data Base:\n");

    for (int i = 0; i < HASH_SIZE; i++)
    {
        Customer *cust = hashTable[i];
        while (cust)
        {
            fprintf(fp, "\nCustomer ID: %ld (%s)\n", cust->msisdn, cust->operatorName);
            fprintf(fp, "Services within the mobile operator *\n");
            fprintf(fp, "Incoming voice call durations: %.2f\n", cust->inVoiceWithin);
            fprintf(fp, "Outgoing voice call durations: %.2f\n", cust->outVoiceWithin);
            fprintf(fp, "Incoming SMS messages: %d\n", cust->smsInWithin);
            fprintf(fp, "Outgoing SMS messages: %d\n", cust->smsOutWithin);
            fprintf(fp, "Services outside the mobile operator *\n");
            fprintf(fp, "Incoming voice call durations: %.2f\n", cust->inVoiceOutside);
            fprintf(fp, "Outgoing voice call durations: %.2f\n", cust->outVoiceOutside);
            fprintf(fp, "Incoming SMS messages: %d\n", cust->smsInOutside);
            fprintf(fp, "Outgoing SMS messages: %d\n", cust->smsOutOutside);
            fprintf(fp, "Internet use *\n");
            fprintf(fp, "MB downloaded: %.2f | MB uploaded: %.2f\n",
                    cust->mbDownload, cust->mbUpload);
            fprintf(fp, "----------------------------------------\n");
            cust = cust->next;
        }
    }
    fclose(fp);
}

// 🟦 MAIN FUNCTION — now with proper paths

void CustomerBillingFile(const char *input_path, const char *output_path)
{
    for (int i = 0; i < HASH_SIZE; i++)

        hashTable[i] = NULL;
    processCDRFile(input_path);
    writeCBFile(output_path);
}









// int main()
// {

//     // Define paths
//     const char *input_path = "../../Data/data.cdr";
//     const char *output_path = "../../Output/CB.txt";
//     // const char *inputPath = "data.cdr"; // input in cbprocess/

//     // const char *outputPath = "cb.txt"; // output in cbprocess/

//     printf("🚀 Starting CDR processing...\n");

//     // Initialize hash table

//     // Step 1: Process input CDR file

//     CustomerBillingFile(input_path, output_path);

//     printf("✅ Processing completed successfully!\n");

//     return 0;
// }
