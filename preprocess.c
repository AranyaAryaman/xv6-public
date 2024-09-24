#include "types.h"
#include "user.h"
#include "fcntl.h"

char temp_buf[1024];
int temp_size = 0;

void printString(char *str)
{
    if (str == 0)
    {
        printf(1, "Null string\n");
        return;
    }
    printf(1, "%s\n", str);
}

int ispunct(int ch)
{
    return (ch == '!' || ch == '?' || ch == ',' || ch == '.' || ch == ';' || ch == ':' || ch == '"');
}

char* my_strtok(char* str, const char* delim) {
    static char* next;
    if (str) next = str;
    if (!next || *next == '\0') return 0;

    char* start = next;
    while (*next && !strchr(delim, *next)) {
        next++;
    }

    if (*next) {
        *next = '\0';
        next++;
    }

    return start;
}

char* strdup(const char* s) {
    char* dup = malloc(strlen(s) + 1);
    if (dup) {
        strcpy(dup, s);
    }
    return dup;
}

char *strncpy(char *s, const char *t, int n)
{
    char *os = s;
    while (n-- > 0 && (*s++ = *t++) != 0)
        ;
    while (n-- > 0)
        *s++ = 0;
    return os;
}

int strncmp(const char *p, const char *q, uint n)
{
    while (n > 0 && *p && *p == *q)
    {
        n--;
        p++;
        q++;
    }
    if (n == 0)
        return 0;
    return (uchar)*p - (uchar)*q;
}


char *findVariable(char *str)
{
    int len = strlen(str);
    if (len <= 2 || str[0] != '-' || str[1] != 'D')
    {
        printf(1, "Wrong Format Variable: %s\n", str);
        return (char *)'1';
    }
    int i = 2;
    while (i < len && str[i] != '=')
    {
        if (ispunct(str[i]))
        {
            printf(1, "Variable can't have punctuation mark as per GCC-14 standards\n");
            return (char *)'1';
        }
        i++;
    }
    if (i == len)
    {
        printf(1, "No equal sign in: %s\n", str);
        return (char *)'1';
    }
    char *res = malloc(sizeof(char) * (i - 1));
    if (res == 0)
    {
        printf(1, "Memory allocation failed\n");
        return (char *)'1';
    }

    strncpy(res, &str[2], i - 2);
    res[i - 2] = '\0';
    return res;
}

char *findValue(char *str)
{
    int len = strlen(str);
    if (len <= 2 || str[0] != '-' || str[1] != 'D')
    {
        printf(1, "Wrong Format Value: %s\n", str);
        return (char *)'1';
    }
    int i = 2;
    while (i < len && str[i] != '=')
        i++;
    if (i == len)
    {
        printf(1, "No equal sign in: %s\n", str);
        return (char *)'1';
    }
    char *res = malloc(sizeof(char) * (len - i));
    if (res == 0)
    {
        printf(1, "Memory allocation failed\n");
        return (char *)'1';
    }
    strncpy(res, &str[i + 1], len - i - 1);
    res[len - i - 1] = '\0';
    return res;
}

char *replaceValues(char *buf, int size, char *variable, char *value)
{
    int variable_length = strlen(variable);
    int value_length = strlen(value);
    temp_size = 0; 

    for (int i = 0; i < size;)
    {
        if (buf[i] == ' ' || ispunct(buf[i]) || buf[i] == '\n')
        {
            temp_buf[temp_size++] = buf[i++];
            continue;
        }

        int curr_word_start = i;
        int curr_word_length = 0;

        while (i < size && !(buf[i] == ' ' || ispunct(buf[i]) || buf[i] == '\n'))
        {
            curr_word_length++;
            i++;
        }

        if (curr_word_length == variable_length && strncmp(&buf[curr_word_start], variable, variable_length) == 0)
        {
            strncpy(&temp_buf[temp_size], value, value_length);
            temp_size += value_length;
        }
        else
        {
            strncpy(&temp_buf[temp_size], &buf[curr_word_start], curr_word_length);
            temp_size += curr_word_length;
        }
    }

    temp_buf[temp_size] = '\0';
    return temp_buf;
}

int replaceAndWrite(int i, char *buf, int size, int argc, char *argv[])
{
    char* replaced_buf = replaceValuesFromDefines(buf, size);
    // strncpy(replaced_buf, buf, size);
    // replaced_buf[size] = '\0'; 

    for (int j = 2; j < argc; j++)
    {
        char *variable = findVariable(argv[j]);
        if (variable == (char *)'1')
        {
            printf(1, "Error parsing variable.\n");
            return -1;
        }
        char *value = findValue(argv[j]);
        if (value == (char *)'1')
        {
            printf(1, "Error parsing value.\n");
            free(variable);
            return -1;
        }
        
        replaced_buf = replaceValues(replaced_buf, strlen(replaced_buf), variable, value);
        free(variable);
        free(value);
    }

    int result = write(i, replaced_buf, strlen(replaced_buf));
    free(replaced_buf);
    return result;
}


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf(1, "Usage style: ./preprocess <input_file> -D<var1>=<val1> -D<var2>=<val2> … -D<varN>=<valN> \n");
        exit();
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf(2, "Error: Cannot open file %s\n", argv[1]);
        exit();
    }

    char buf[1024];
    int n;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        char* line = my_strtok(buf, "\n");
        while (line != 0) {
            if (strncmp(line, "#define", 7) == 0) {
                processDefine(line);
            }
            line = my_strtok(0, "\n");
        }
    }
    
    close(fd);

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf(2, "Error: Cannot reopen file %s\n", argv[1]);
        exit();
    }

    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        if (replaceAndWrite(1, buf, n, argc, argv) < 0)
        {
            printf(2, "write error\n");
            close(fd);
            exit();
        }
    }
    if (n < 0)
    {
        printf(2, "read error\n");
        close(fd);
        exit();
    }
    printf(1, "\n");
    close(fd);
    exit();
}