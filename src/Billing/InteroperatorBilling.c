#include "../../Header/interopHeader.h"

/* ============================================================
   Hash Map Implementation (Chaining)
   ============================================================ */

static OpNode *buckets[NUM_BUCKETS] = {NULL};

unsigned long str_hash(const char *s)
{
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*s++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

OpNode *get_or_create_opnode(const char *operator_id, const char *operator_name)
{
    unsigned long h = str_hash(operator_id);
    unsigned idx = (unsigned)(h % NUM_BUCKETS);
    OpNode *node = buckets[idx];

    while (node)
    {
        if (strcmp(node->operator_id, operator_id) == 0)
            return node;
        node = node->next;
    }

    // Create a new node
    OpNode *newnode = (OpNode *)calloc(1, sizeof(OpNode));
    newnode->operator_id = strdup(operator_id);
    newnode->stats.operator_name = operator_name ? strdup(operator_name) : strdup("UNKNOWN");
    newnode->next = buckets[idx];
    buckets[idx] = newnode;
    return newnode;
}

/* ============================================================
   Utility Helpers
   ============================================================ */

void chomp(char *s)
{
    if (!s)
        return;
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r'))
    {
        s[--n] = '\0';
    }
}

/* Handle empty tokens (||) correctly
   There are instances in data.cdr where, there are 2 pipes are consecutively without any value:
   This function handles those cases.

*/
int split_pipe(char *line, char **tokens, int max_tokens)
{
    int idx = 0;
    char *start = line;
    for (char *p = line; *p && idx < max_tokens - 1; p++)
    {
        if (*p == '|')
        {
            *p = '\0';
            tokens[idx++] = start;
            start = p + 1;
        }
    }
    tokens[idx++] = start;
    return idx;
}
long to_long_or_zero(const char *s)
{ // Conversion of string integer (long or zero)
    if (!s || *s == '\0')
        return 0;
    char *endptr;
    long v = strtol(s, &endptr, 10); // Converting string to long int
    if (endptr == s)
        return 0;

    return v;
}

/* ============================================================
   Line Processor
   ============================================================ */

void process_line(char *line)
{
    chomp(line);
    if (line[0] == '\0')
        return;

    char *tokens[12];
    int n = split_pipe(line, tokens, 9);
    for (int i = n; i < 9; ++i)
        tokens[i] = "";

    const char *msisdn = tokens[0];
    const char *operator_name = tokens[1];
    const char *operator_id = tokens[2];
    const char *call_type = tokens[3];
    const char *duration_s = tokens[4];
    const char *download_s = tokens[5];
    const char *upload_s = tokens[6];

    if (!operator_id || operator_id[0] == '\0')
        return; // must have operator_id

    OpNode *node = get_or_create_opnode(operator_id, operator_name);
    OperatorStats *s = &node->stats;

    if (call_type == NULL)
        return;

    // Normalize call_type (trim spaces)
    char ct[32];
    snprintf(ct, sizeof(ct), "%s", call_type);
    for (char *p = ct; *p; ++p)
        *p = toupper((unsigned char)*p);

    if (strcmp(ct, "MOC") == 0)
        s->total_moc_duration += to_long_or_zero(duration_s);
    else if (strcmp(ct, "MTC") == 0)
        s->total_mtc_duration += to_long_or_zero(duration_s);
    else if (strcmp(ct, "SMS-MO") == 0)
        s->sms_mo_count += 1;
    else if (strcmp(ct, "SMS-MT") == 0)
        s->sms_mt_count += 1;
    else if (strcmp(ct, "GPRS") == 0)
    {
        s->total_download += to_long_or_zero(download_s);
        s->total_upload += to_long_or_zero(upload_s);
    }
}

/* ============================================================
   Main Processing Function
   ============================================================ */

void InteroperatorBillingFile(const char *filename, const char *output_filename)
{
    FILE *fin = fopen(filename, "r");
    if (!fin)
    {
        fprintf(stderr, "Error opening input file '%s': %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    FILE *fout = fopen(output_filename, "w");
    if (!fout)
    {
        fprintf(stderr, "Error creating output file '%s': %s\n", output_filename, strerror(errno));
        fclose(fin);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    size_t read;

    while ((read = getline(&line, &len, fin)) != -1)
    {
        process_line(line);
    }

    free(line);
    fclose(fin);

    // Write all aggregated results
    for (unsigned i = 0; i < NUM_BUCKETS; ++i)
    {
        OpNode *node = buckets[i];
        while (node)
        {
            OperatorStats *s = &node->stats;
            fprintf(fout, "Operator Brand: %s (%s)\n", s->operator_name, node->operator_id);
            fprintf(fout, "\tIncoming voice call durations: %ld\n", s->total_mtc_duration);
            fprintf(fout, "\tOutgoing voice call durations: %ld\n", s->total_moc_duration);
            fprintf(fout, "\tIncoming SMS messages: %ld\n", s->sms_mt_count);
            fprintf(fout, "\tOutgoing SMS messages: %ld\n", s->sms_mo_count);
            fprintf(fout, "\tMB Download: %ld | MB Uploaded: %ld\n", s->total_download, s->total_upload);
            fprintf(fout, "----------------------------------------\n");
            node = node->next;
        }
    }

    fclose(fout);

    // Free memory (avoid leaks)
    for (unsigned i = 0; i < NUM_BUCKETS; ++i)
    {
        OpNode *node = buckets[i];
        while (node)
        {
            OpNode *tmp = node->next;
            free(node->operator_id);
            free(node->stats.operator_name);
            free(node);
            node = tmp;
        }
        buckets[i] = NULL;
    }

}



/* ============================================================
   Entry Point
   ============================================================ */

// int main(void)
// {
//     const char *input_path = "../../Data/data.cdr";
//     const char *output_path = "../../Output/ISOB.txt";
    
//     printf("Processing CDR file: %s\n", input_path);
//     printf("Output will be written to: %s\n", output_path);
    
//     InteroperatorBillingFile(input_path, output_path);
    
//     printf("Processing completed successfully\n");
//     return 0;
// }
