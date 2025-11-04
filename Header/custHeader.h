#ifndef CUST_HEADER_H
#define CUST_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   Constants and Definitions
   ============================================================ */

/**
 * @brief Size of the hash table (prime number for better distribution)
 */
#define HASH_SIZE 100007

/* ============================================================
   Data Structures
   ============================================================ */

/**
 * @struct Customer
 * @brief Structure to hold each customer's aggregated billing data
 */
typedef struct Customer {
    long msisdn;                    // Customer's phone number
    char operatorName[64];          // Name of the operator
    int operatorCode;               // Operator's code
    float inVoiceWithin;            // Incoming voice calls within operator
    float outVoiceWithin;          // Outgoing voice calls within operator
    float inVoiceOutside;          // Incoming voice calls to other operators
    float outVoiceOutside;         // Outgoing voice calls to other operators
    int smsInWithin;               // Incoming SMS within operator
    int smsOutWithin;              // Outgoing SMS within operator
    int smsInOutside;              // Incoming SMS from other operators
    int smsOutOutside;             // Outgoing SMS to other operators
    float mbDownload;              // Data downloaded in MB
    float mbUpload;                // Data uploaded in MB
    struct Customer *next;          // Next customer in hash chain
} Customer;

/* ============================================================
   Function Declarations
   ============================================================ */

/**
 * @brief Calculates hash value for a customer's MSISDN
 * @param key The MSISDN to hash
 * @return unsigned int Hash value for the MSISDN
 */
unsigned int hashFunction(long key);

/**
 * @brief Creates a new customer entry
 * @param msisdn Customer's phone number
 * @param operatorName Name of the customer's operator
 * @param operatorCode Code of the customer's operator
 * @return Customer* Pointer to the newly created customer
 */
Customer *createCustomer(long msisdn, const char *operatorName, int operatorCode);

/**
 * @brief Gets existing customer or creates new one if not found
 * @param msisdn Customer's phone number
 * @param operatorName Name of the customer's operator
 * @param operatorCode Code of the customer's operator
 * @return Customer* Pointer to the customer record
 */
Customer *getCustomer(long msisdn, const char *operatorName, int operatorCode);

/**
 * @brief Processes the CDR file and updates customer records
 * @param filename Path to the CDR file
 */
void processCDRFile(const char *filename);

/**
 * @brief Writes processed customer data to output file
 * @param outputFile Path to the output file
 */
void writeCBFile(const char *outputFile);

/**
 * @brief Main processing function for customer billing
 * @param filename Path to the input CDR file
 * @param output_filename Path to the output file
 */
void CustomerBillingFile(const char *filename, const char *output_filename);

/* ============================================================
   External Variables
   ============================================================ */

/**
 * @brief Hash table for storing customer records
 */
extern Customer *hashTable[HASH_SIZE];

/**
 * @brief Counter for total records processed
 */
extern int totalRecords;

#endif /* CUST_HEADER_H */