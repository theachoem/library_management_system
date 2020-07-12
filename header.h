#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <math.h>

typedef struct data{
    int key;
    int value;
    int isbn_key;
    char isbn[20];
    char title[50];
    char author1[50];
    char author2[50];
    char author3[50];
    int year;
    int cp;
} DATA;

int capacity = 1000;
int *store_index;
int size = 0;

void save_file(DATA *array, char path[50]);
void load_file(DATA *array, char path[50]);
void add_new_book(DATA *array);
void modify_book(DATA *array);
void delete_book(DATA *array);
void list_books(DATA *array);
void sort_books(DATA *array);
void search_book_hashing(DATA *array);
void list_books_later(DATA *array);

int hashcode(int key){ //hashing function for integer
    return (key % capacity);
}

/* hashing function for string */
int hash_string(char key[50]){
    int c, s=0;
    for(int i=0; i<strlen(key); i++){
        c = key[i];
        s = s + c;
    }
    s = s % capacity;
    return s;
}

/* to check if given input (i.e n) is prime or not*/
int if_prime(int n){
    int i;
    if ( n == 1  ||  n == 0) return 0;
    for (i = 2; i < n; i++) if (n % i == 0) return 0;
    return 1;
}

/* it returns prime number just greater than array capacity */
int get_prime(int n){
    if (n % 2 == 0) n++;
    while(!if_prime(n)) n += 2;
    return n;
}

DATA *init_array(){
    int i;
    capacity = get_prime(capacity);
    DATA *array = (DATA*) malloc(capacity * sizeof(DATA));
    store_index = malloc(capacity * sizeof(int));

    printf("CAP :%d", capacity);
    for (i = 0; i < capacity; i++){
        array[i].key = 0;
        array[i].value = 0;
    }
    return array;
}

/* to insert a key in the hash table */
void insert(DATA *array, char isbn[20], char title[50], char author1[50], char author2[50], char author3[50], int year, int cp){
    int index = hash_string(title);
    int isbn_key = hash_string(isbn);
//    printf("ISBN: %d\n", isbn_key);
    if (array[index].value == 0){
        array[index].value = 1;
        array[isbn_key].isbn_key = index;
        strcpy(array[index].title, title);
        strcpy(array[index].isbn, isbn);
        strcpy(array[index].author1, author1);
        strcpy(array[index].author2, author2);
        strcpy(array[index].author3, author3);
        array[index].year = year;
        array[index].cp = cp;
        store_index[size] = index;
        size++;
        printf("  Index[%d] has been inserted\n", index);
    }
    else if(array[index].value == 1){
        printf(" Index[%d] already exist!\n");
    }
    else{
        /* key cannot be insert as the index is already containing some other key  */
        printf("  Index[%d] cannot be inserted!\n");
    }
}

void remove_data(DATA *array, int index){
    array[index].key = 0;
    array[index].value = 0;

    size --;
}

int size_of_hashtable(DATA *array){
    return size;
}

int is_valid_isbn(char isbn[]){
    if(strlen(isbn) != 10) return 0;  //ISBN have to be 10 digit
    int sum = 0, tmp, j=10;
    if(isbn[9] == 'X' || ((isbn[9] - 48 >= 0) && (isbn[9] - 48 <= 9))){
        for(int i=0; i<10; i++){
            tmp = isbn[i] - 48;
            if(i==9 && isbn[i] == 'X') sum += 10;
            else sum += tmp*j;
            j--;
        }
        return sum%11 == 0;
    }
}

void destoy_array(DATA *array){
    int index;
    for(int i=0; i<size; i++){
        index = store_index[i];
        remove_data(array, index);
    }
}

char *remove_space(char str[]){
    for(int i=0; i<strlen(str); i++){
        if(str[i]== ' ') str[i] = '_';
    }
    return str;
}
