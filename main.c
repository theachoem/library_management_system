#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "header.h"

int main(){
    DATA *array = init_array();
    char ch;
    if(!if_file_empty("dataTMP.txt")){
        retry:
        printf("  Do you want to load unsaved data? (Y/n): ");
        scanf("%c", &ch);
        if(ch == 'Y' || ch == 10 || ch == 'y') load_file(array, "dataTMP.txt");
        else if(ch == 'n' || ch == 'N') load_file(array, "data.txt");
        else goto retry;
    }
    else load_file(array, "data.txt");

    again1:
    printf("\n");
    printf("  ==============================\n");
    printf("        LIBRARY MANAGEMENT      \n");
    printf("  ==============================\n");
    printf("\n  1. Add new book to library.");
    printf("\n  2. Modify a book (using ISBN).");
    printf("\n  3. Delete a book (using ISBN).");
    printf("\n  4. List all books");
    printf("\n  5. Sort books by their titles.");
    printf("\n  6. Search for a book (using title)");
    printf("\n  7. List books which is published later (by year).");
    printf("\n  8. Save data to file.");
    printf("\n  9. Load data from file.");
    printf("\n  0. Exit\n");

    int choice;
    printf("\n  Enter your choice (menu above): ");
    scanf("%d", &choice);

    switch(choice){
        case 1:add_new_book(array);
            break;
        case 2:modify_book(array);
            break;
        case 3:delete_book(array);
            break;
        case 4:list_books(array);
            break;
        case 5:sort_books(array);
            break;
        case 6:search_book_hashing(array);
            break;
        case 7:list_books_later(array);
            break;
        case 8:save_file(array, "data.txt");
            break;
        case 9:load_file(array, "data.txt");
            break;
        case 0:
            printf("  Do you want to save changed? (Y/n)");
            char choice2;
            scanf("%s", &choice2);
            if(choice2 == 'y' || choice2 == 'Y'){
                save_file(array, "data.txt");
                return 0;
            }
            else if(choice == 'n') printf("  Not saved!");
            else return 0;
        case 10: system("cls");
        default: goto again1;
    }
    goto again1;

    free(array);
    free(store_index);
    printf("\n\n");
    return 0;
}

void save_file(DATA *array, char path[50]){
    int v, i, j;
    FILE *f = fopen(path, "w");
    for (i = 0; i < capacity; i++){
        if (array[i].value != 0){
            fprintf(f, "%s %s %s %s %s %d %d\n",array[i].isbn, array[i].title, array[i].author1, array[i].author2, array[i].author3, array[i].year, array[i].cp);
        }
    }
	fclose(f);
	printf("  Saved");
}

void load_file(DATA *array, char path[50]){
    int year, cp;
    char isbn[20], title[50];
    char author1[50], author2[50], author3[50];

    FILE *f = fopen(path, "r");
    while(fscanf(f, "%s %s %s %s %s %d %d\n", isbn, title, author1, author2, author3, &year, &cp)!= EOF){
        if(is_valid_isbn(isbn))
            insert(array, isbn, title, author1, author2, author3, year, cp);
    }
    save_file(array, "dataTMP.txt");
    printf("  DONE!\n");
}

void add_new_book(DATA *array){
    int key, year = 0, cp;
    char isbn[20], title[50];
    char author1[50], author2[50], author3[50];
    strcpy(isbn, "0");
    do{
        if(strcmp(isbn, "0")) printf("  Invalid ISBN!");
        printf("\n  Enter ISBN: ");
        scanf("%s", &isbn);
    } while(!is_valid_isbn(isbn));

    printf("\n  Enter book title: ");
    gets(title); gets(title);
    printf("\n  Enter name of author 1: ");
    gets(author1);
    printf("\n  Enter name of author 2: ");
    gets(author2);
    printf("\n  Enter name of author 3: ");
    gets(author3);
    do{
        if(year != 0) printf("  Invalid year!");
        printf("\n  Enter year of publication: ");
        scanf("%d", &year);
    } while(year < 1500);

    printf("\n  Enter number of copies: ");
    scanf("%d", &cp);
    insert(array, isbn, remove_space(title), remove_space(author1), remove_space(author2), remove_space(author3), year, cp);
    save_file(array, "dataTMP.txt");
}

void modify_book(DATA *array){
    int year = 0, cp;
    char isbn[20], title[50];
    char author[50];

    printf("\n  Enter ISBN: ");
    scanf("%s", &isbn);
    int isbn_key = hash_string(isbn);
    int index = array[isbn_key].isbn_key;
    if(array[index].value == 0){
        printf("  ISBN not found!\n");
        return;
    }
    printf("  What information do you want to change?\n");
    printf("  1. Book title\n  2. Author Name\n  3. ISBN\n  4. Published Year\n  5. Number of Copy\n");
    int choice, c2;
    again:
    printf("\n  Your choice: ");
    scanf("%d", &choice);
    switch(choice){
        case 1:
            printf("\n  Enter new book title: ");
            gets(title); gets(title);
            remove_data(array, index);
            insert(array, array[index].isbn, remove_space(title), array[index].author1, array[index].author2, array[index].author3, array[index].year, array[index].cp);
        break;
        case 2:
            printf("  Enter new author name: ");
            scanf("%s", &author);
            printf("  Make change on author 1 or 2 or 3: ");
            scanf("%d", &c2);
            if(c2 == 1) strcpy(array[index].author1, author);
            if(c2 == 2) strcpy(array[index].author2, author);
            if(c2 == 3) strcpy(array[index].author3, author);
        break;
        case 3:
            strcpy(isbn, "0");
            do{
                if(strcmp(isbn, "0")) printf("  Invalid ISBN!");
                printf("\n  Enter new ISBN: ");
                scanf("%s", &isbn);
            } while(!is_valid_isbn(isbn));
            remove_data(array, index);
            insert(array, isbn, array[index].title, array[index].author1, array[index].author2, array[index].author3, array[index].year, array[index].cp);
        break;
        case 4:
            do{
                if(year != 0) printf("  Invalid year!");
                printf("\n  Enter update of published year: ");
                scanf("%d", &year);
            } while(year < 1500);
            array[index].year = year;
        break;
        case 5:
            printf("  Enter update of copies number: ");
            scanf("%d", &cp);
            array[index].cp = cp;
        break;
        default:
            goto again;
    }
    save_file(array, "dataTMP.txt");
}

void delete_book(DATA *array){
    char isbn[20], c1;
    printf("\n  Enter ISBN: ");
    scanf("%s", &isbn);
    int isbn_key = hash_string(isbn);
    int index = array[isbn_key].isbn_key;
    if(array[index].value == 0){
        printf("  ISBN not found!\n");
        return;
    } else{
        printf("  Are you sure to delete(Y/n): ");
        scanf("%s", &c1);
        if(c1 == 'Y'){
            remove_data(array, index);
            printf("  Index[%d] has been deleted!\n", index);
        }
        else return;
    }
    save_file(array, "dataTMP.txt");
}


void sort_books(DATA *array){
    char isbn[20];
    int year, cp;
    char title[50];
    char author1[50];
    char author2[50];
    char author3[50];

    char cmp1[50], cmp2[50];

    printf("  Do you want to sort by ISBN or TITLE? (1 or 2): ");
    scanf("%s",&sorted_check);
    int i,j, index, index1;

    for(i=0; i<size; i++){
        for(j=i+1; j<size; j++){
            index = store_index[i];
            index1 = store_index[j];
            if(sorted_check == '1'){
                strcpy(cmp1, array[index].isbn);
                strcpy(cmp2, array[index1].isbn);
            }
            if(sorted_check == '2'){
                strcpy(cmp1, array[index].title);
                strcpy(cmp2, array[index1].title);
            }
            if(strcmp(cmp1, cmp2)>0 && array[index].value == 1 && array[index].value == 1){
                store_index[i] = store_index[j];
                store_index[j] = index;
            }
        }
    }
    DATA *array2 = array;
    save_sort(array2);
}

void list_books(DATA *array){
    int i, index;
    char tmp[50], t;
    for (i = 0; i < size; i++){
        index = store_index[i];
        if(array[index].value == 1) printf("  ISBN: %s - title: %s, %d\n", array[index].isbn, add_space(array[index].title), array[index].value);
    }
    printf("\n");
    again:
    printf("  Do you want to detail a book(Y/n): ");
    scanf("%s", &t);
    if(t == 'Y') search_book_hashing(array);
    else if(t == 'n') return;
    else goto again;
}

void search_book_hashing(DATA *array){
    char title[50];
    printf("  Enter book title: ");
    gets(title); gets(title);
    int index = hash_string(remove_space(title));
    if(array[index].value == 0){
        printf("  Book not found!\n");
        return;
    }
    printf("\n  | Title: %s\n", add_space(array[index].title));
    printf("  | Authur 1: %s\n", add_space(array[index].author1));
    printf("  | Authur 2: %s\n", add_space(array[index].author2));
    printf("  | Authur 3: %s\n", add_space(array[index].author3));
    printf("  | ISBN: %s\n", array[index].isbn);
    printf("  | Year Publishes: %d\n", array[index].year);
    printf("  | Number of copies: %d\n", array[index].cp);
}

void list_books_later(DATA *array){
    int year, index;
    char tmp[50];
    printf("  Enter released year: ");
    scanf("%d", &year);
    for (int i = 0; i < size; i++){
        index = store_index[i];
        if (array[index].year >= year && array[index].value == 1)
            printf("  Year: %d - title: %s\n",array[index].year ,add_space(array[index].title));
    }
}
