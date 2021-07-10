#define NOT_EXIST -1
#include <iostream>

using namespace std;

typedef struct Dictionary
{
    char** seq;
    int*   code;
    int    size;
    int    max_size;
}Dictionary;

/*
 * declarition.
*/

void init_dictionary(Dictionary* dict, int max_size);
void insert_seq(Dictionary* dict, const char* seq);
void print_dictionary(Dictionary* dict);
int get_seq_code(Dictionary* dict, const char* seq);
char* get_code_seq(Dictionary* dict, int code);

void init_dictionary(Dictionary* dict, int max_size)
{
    int i = 0;
    dict->seq  = (char**)calloc(max_size, sizeof(char*));
    dict->code = (int*)calloc(max_size, sizeof(int));
    dict->size = 0;
    dict->max_size = max_size;

    insert_seq(dict, "#");
    char data[2] = "A";
    for(i = 0; i < 26; i++)
    {
        insert_seq(dict, data);
        data[0] += 1;
    }
}

void insert_seq(Dictionary* dict, const char* seq)
{
    int i = 0;
    dict->seq[dict->size]  = (char*)calloc(strlen(seq) + 1, sizeof(char));
    dict->code[dict->size] = dict->size;
    strcpy(dict->seq[dict->size], seq);
    dict->size += 1;
}

void print_dictionary(Dictionary* dict)
{
    printf("---------------\n");
    printf("   code     seq\n");
    printf("---------------\n");
    for(int i = 0; i < dict->size; i++)
    {
        printf("%5d        %s\n", i, dict->seq[i]);
    }
    printf("---------------\n");
}

char* get_code_seq(Dictionary* dict, int code)
{
    return dict->seq[code];
}

int get_seq_code(Dictionary* dict, const char* seq)
{
    int i = 0;
    for(i = 0; i < dict->size; i++)
    {
        if(strcmp(dict->seq[i],seq) == 0)
        {
            return dict->code[i];
        }
    }
    return NOT_EXIST;
}

void lzw_encode(const char* text, Dictionary* dict)
{
    char current[1000] = { 0 };
    char next = 0;
    int i = 0;
    int code = 0;
    while( i < strlen(text) )
    {
        sprintf(current,"%c",text[i]);
        next = text[i + 1];
        if(next == '\0')
        {
            code = get_seq_code(dict, current);
        }
        else
        {
            while(get_seq_code(dict, current) != NOT_EXIST)
            {
                sprintf(current, "%s%c", current, next);
                i += 1;
                next = text[i + 1];
                if(next == '\0')break;
            }
            if(next != '\0')
            {
                insert_seq(dict, current);
                current[strlen(current) - 1] = '\0';
                code = get_seq_code(dict, current);
                printf("%d,", code);
            }
            else
            {
                code = get_seq_code(dict, current);
                printf("%d\n", code);
                break;
            }
        }
    }

}

void lzw_decode(int codes[],int n, Dictionary* dict)
{
    int   code;
    char  prev[1000];
    char* output;

    code = codes[0];
    output = get_code_seq(dict, code);
    printf("%s\n", output);
    int i = 0;
    for(i = 1; i < n; i++)
    {
        code = codes[i];
        sprintf(prev,"%s", output);
        output = get_code_seq(dict, code);
        sprintf(prev,"%s%c", prev, output[0]);
        insert_seq(dict,prev);
        printf("%s\n", output);
    }
}

int main()
{
    Dictionary dict;
    init_dictionary(&dict, 1000);
//    lzw_encode("TOBEORNOTTOBEORTOBEORNOT", &dict);
    print_dictionary(&dict);
    int arr[] = {20,15,2,5,15,18,14,15,20,27,29,31,36,30,32,34};
    lzw_decode(arr,sizeof(arr) / sizeof(int), &dict);
    cout << "Hello World!" << endl;
    return 0;
}
