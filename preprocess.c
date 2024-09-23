#include "types.h"
#include "user.h"
#include "fcntl.h"

void printString(char* str) {
    if (str == 0) {
        printf(1, "Null string\n");
        return;
    }
    
    printf(1, "%s\n", str);
}

char* strncpy(char *s, const char *t, int n)
{
  char *os;

  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = 0;
  return os;
}

int
strncmp(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

char* findVariable(char* str){
    int len = strlen(str);
    if(len<=2){
        printf(1,"Wrong Format: %s",str);
        return (char*)'1';
    }
    if(*str!='-' || *(str+1)!='D'){
        printf(1,"Wrong Format: %s",str);
        return (char*)'1';
    }
    // printf(1,"len: %d",len);
    int i=2;
    for(i=2;i<len;i++){
        if(str[i]=='=')
            break;
    }
    // printf(1,"i: %d",i);
    if(i==len){
        printf(1,"No equal sign in: %s\n", str);
        return (char*)'1';
    }
    char* res = malloc(sizeof(char) * (i-1));
    if (res == 0) {
        printf(1, "Memory allocation failed\n");
        return (char*)'1';
    }

    strncpy(res, &str[2], i - 2);  
    res[i - 2] = '\0';  

    return res;
}

char* findValue(char* str){
    int len = strlen(str);
    if(len<=2){
        printf(1,"Wrong Format: %s",str);
        return (char*)'1';
    }
    if(*str!='-' || *(str+1)!='D'){
        printf(1,"Wrong Format: %s",str);
        return (char*)'1';
    }
    int i=2;
    for(i=2;i<len;i++){
        if(str[i]=='=')
            break;
    }
    // printf(1,"i: %d",i);
    if(i==len){
        printf(1,"No equal sign in: %s\n", str);
        return (char*)'1';
    }
    char* res = malloc(sizeof(char) * (len-i));
    if (res == 0) {
        printf(1, "Memory allocation failed\n");
        return (char*)'1';
    }

    strncpy(res, &str[i+1], len - i-1);  
    res[len-i] = '\0';  
    return res;
}

int ispunct(int ch){
    if(ch=='!' || ch=='?' || ch==',' || ch=='.' || ch ==';' || ch==':' || ch=='"')
        return 1;
    else
        return 0;
}

char* formString(char* buf, int start, int length) {
    char* str = malloc(sizeof(char) * (length + 1));
    strncpy(str, &buf[start], length);
    str[length] = '\0';
    return str;
}

char* replaceValues(char* buf, int size, char* variable, char* value) {
    int variable_length = strlen(variable); 
    int value_length = strlen(value);
    
    char temp_buf[1024];  
    int temp_size = 0;   
    
    for (int i = 0; i < size; ) {
        
        if (buf[i] == ' ' || ispunct(buf[i]) || buf[i]=='\n' || buf[i]=='t') {
            temp_buf[temp_size++] = buf[i++];
            continue;
        }
        
        int curr_word_start = i;
        int curr_word_length = 0;
        
        while (i < size && !(buf[i] == ' ' || ispunct(buf[i]) || buf[i]=='\n' || buf[i]=='t')) {
            curr_word_length++;
            i++;
        }
        
        if (curr_word_length == variable_length && 
            strncmp(&buf[curr_word_start], variable, variable_length) == 0) {
            strncpy(&temp_buf[temp_size], value, value_length);
            temp_size += value_length;
        } else {
            strncpy(&temp_buf[temp_size], &buf[curr_word_start], curr_word_length);
            temp_size += curr_word_length;
        }
    }

    temp_buf[temp_size] = '\0';  

    strncpy(buf, temp_buf, temp_size);
    buf[temp_size] = '\0';  

    return buf;
}


int replaceAndWrite(int i, char* buf, int size, int argc, char* argv[]){
    for(int i=2;i<argc;i++){
        char* variable = findVariable(argv[i]);
        if (variable == (char*)'1') {
            printf(1, "Error parsing variable.\n");
            continue;
        }
        char* value = findValue(argv[i]);
        if(value == (char*)'1'){
            printf(1, "Error parsing value.\n");
        }
        // printf(1, "Variable: %s\n",variable);
        // printf(1,"Value: %s\n",value);
        replaceValues(buf,size,variable,value);
    }
    return write(i,buf,size);
}

int main(int argc, char *argv[])
{

    if(argc < 3){
        printf(1,"Usage style: ./preprocess <input_file> -D<var1>=<val1> -D<var2>=<val2> … -D<varN>=<valN> \n");
        exit();
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf(2, "Error: Cannot open file %s\n", argv[1]);
        exit();
    }
    //printf(1,"We are here\n");
    char buf[1024];
    int n;
    for (;;)
    {
        n = read(fd, buf, sizeof buf);
        if (n == 0)
            break;
        if (n < 0)
        {
            printf(2, "read error\n");
            exit();
        }
        if (replaceAndWrite(1, buf, n, argc, argv) != n)
        {
            printf(2, "write error\n");
            exit();
        }
    }
    printf(1,"\n");

    exit();
}