#include <stdio.h>
#include <string.h>

#define STR_MAX_LEN 1000
#define ALPHABET_SIZE 52

char alphabet[ALPHABET_SIZE];
char tabula_recta[ALPHABET_SIZE][ALPHABET_SIZE];

char plainText[STR_MAX_LEN];
char key[STR_MAX_LEN];
char encriptedText[STR_MAX_LEN];
char decryptedText[STR_MAX_LEN];


void fill_tabula_recta();
void fill_alphabet();


void encrypt(char* msg, char* key, char* encriptedText);

static void print_string(char* str);
static int inside_alphabet(char c);
static int alphabet_index(char c);
static int count_string(char* msg);
static int validate_key(int len_msg, int len_key);
static int is_special_caracter(char c);
static void string_remove_especial(char* str);
static int reverse_table_querry(char key_c, char cripted_c);

int main()
{
    printf("Inicializando...\n");
    fill_alphabet();
    fill_tabula_recta();

    //Coloca o texto e depois poe o \0 no final
    strcpy(plainText, "vasco da gama\0");
    strcpy(key, "chave hype\0");
    string_remove_especial(key);

    printf("\nMensagem: %s \nChave: %s", plainText, key);
    printf("\nCriptografando...\n");

    encrypt(plainText, key, encriptedText);
    decrypt(encriptedText, key, decryptedText);

    return 0;
}

/*
    Função que preenche de maneira automática o vetor alphabeto com os caracteres
    de 'a' à 'z' e de 'A' à 'Z'

    Como o C nos permite fazer operações com os char como se fossem inteiros 
    - por que de fato, são. Sendo valores que representam caracteres em ASCII -,
    Podemos criar laços de repetição utilizando o char para interar sobre os elementos
    da como um indice qualquer.

    Aqui por exemplo, c recebe 'a' que tem um valor inteiro de 97, 'b' tem o valor 98, 
    Sendo os números inteiros que representam o alphabeto também mantendo a ordenação do mesmo.
*/
void fill_alphabet()
{
    // Preenchendo as letras minúsculas
    for (char c = 'a'; c <= 'z'; c++) alphabet[c - 'a'] = c;

    // Preenchendo as letras maiúsculas
    for (char c = 'A'; c <= 'Z'; c++) alphabet[c - 'A' + 26] = c;

    // Imprimindo o vetor
    printf("\n O alfabeto é: \t");
    for (int i = 0; i < 52; i++) printf("%c ", alphabet[i]);
}

// Preenche a tabula recta
void fill_tabula_recta()
{
    int i, j, index;
    for (i = 0; i < ALPHABET_SIZE; i++)
    {        
        for (j = 0; j < ALPHABET_SIZE; j++)
        {
            index = i+j;
            if(index >= ALPHABET_SIZE) index -= ALPHABET_SIZE;
            tabula_recta[i][j] = alphabet[index];
        }
    }

    printf("\n A tabula recta é: \n");    
    for (i = 0; i < ALPHABET_SIZE; i++)
    {        
        printf("\n");
        for (j = 0; j < ALPHABET_SIZE; j++)
        {
            printf(" %c ", tabula_recta[i][j]);
        }
    }
}

// Ignora espaços.
void encrypt(char* msg, char* key, char* encriptedText)
{
    int len_msg = count_string(msg);
    int len_key = count_string(key);

    if(!validate_key(len_msg, len_key))
    {
        printf("\n %i, %i A chave nao eh valida! \n Abortando...", len_key, len_msg);
    }

    int i = 0, j = 0;
    do { 
        if(msg[i] == '\0')
        {
            encriptedText[i] = '\0';
            break;
        }

        if(msg[i] == ' ') 
        {
            encriptedText[i] = ' ';
            i++;
            continue;
        }

        if(!inside_alphabet(msg[i]))
        {
            printf("Foi encontrado um caractere que não pertence ao alfabeto! \n Abortando...");
        }

        if(j >= len_key) j = 0;

        encriptedText[i] = tabula_recta[alphabet_index(msg[i])][alphabet_index(key[j])];

        printf(" %c ", tabula_recta[alphabet_index(msg[i])][alphabet_index(key[j])]);

        i++;
        j++;
    } while(1);

}


// Ignora espaços.
void decrypt(char* encriptedText, char* key, char* decriptedText)
{
    int len_msg = count_string(encriptedText);
    int len_key = count_string(key);

    if(!validate_key(len_msg, len_key))
    {
        printf("\n %i, %i A chave nao eh valida! \n Abortando...", len_key, len_msg);
    }

    int i = 0, j = 0;
    do { 
        if(encriptedText[i] == '\0')
        {
            decriptedText[i] = '\0';
            break;
        }

        if(encriptedText[i] == ' ') 
        {
            decriptedText[i] = ' ';
            i++;
            continue;
        }

        if(!inside_alphabet(encriptedText[i]))
        {
            printf("Foi encontrado um caractere que não pertence ao alfabeto! \n Abortando...");
        }

        if(j >= len_key) j = 0;

        decriptedText[i] = alphabet[reverse_table_querry(key[j], encriptedText[i])];

        printf(" %c ", decriptedText[i]);

        i++;
        j++;
    } while(1);
    printf("\n\n\n Valor do I: %i \n\n\n", i);
    printf("\nTexto criptografado: \"");
    print_string(encriptedText);
    printf("\" \n");
    printf("\nChave: \"");
    print_string(key);
    printf("\" \n\n");
    printf("\nTexto decriptografado: \"");
    print_string(decriptedText);
    printf("\" \n");

}


/******** FUNÇÕES AUXILIARES *********/

//Conta até achar o nulo, caso contrário, pode haver acesso inadequado da memória caso não use o nulo.
static int count_string(char* msg)
{
    int len = 0;
    char c;
    
    while(msg[len] != '\0')
    {
        len++;
    }

    return len;
}

static int is_special_caracter(char c)
{
    if(c == ' ') return 1;

    return 0;
}
static void string_remove_especial(char* str)
{
    int i = 0, j = 0;
    
    while(str[i] != '\0')
    {
        if(is_special_caracter(str[i]))
        {
            int k = i;
            while(str[k] != '\0')
            {                
                str[k] = str[k+1];
                k++;
            }
        }
        else
        {
            // Caso seja, o i se mantém o mesmo pois deve ser avaliado novamente.
            i++;
        }
    }
}

static void print_string(char* str)
{
    int i = 0;
    
    while(str[i] != '\0')
    {
        printf("%c", str[i]);
        i++;
    }
}

static int inside_alphabet(char c)
{
    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        if(alphabet[i] == c) return 1;
    }
    return 0;
}

static int alphabet_index(char c)
{
    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        if(alphabet[i] == c) return i;
    }
    return -1;
}

static int reverse_table_querry(char key_c, char cripted_c)
{
    char c;
    int i;
    for(i = 0; i < ALPHABET_SIZE; i++)
    {
        c = tabula_recta[alphabet_index(key_c)][i];
        if(c == cripted_c) return i;
    }
    return -1;
}

static int validate_key(int len_msg, int len_key)
{
    if(len_key >= 1 && len_key <= len_msg)
    {
        return 1;
    }
    return 0;
}