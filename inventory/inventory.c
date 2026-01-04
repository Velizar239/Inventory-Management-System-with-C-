/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>

/*
    INVENTORY MANAGEMENT SYSTEM (FILE SAVING VERSION)
    -------------------------------------------------
    - Uses text file storage (inventory.txt)
    - Data persists after program ends
    - Supports Add, View, Update, Delete, Search
    - Suitable for university C project
*/

#define FILE_NAME "inventory.txt"

struct Item {
    int id;
    char name[30];
    float quantity;
};

void addItem();
void viewItems();
void updateItem();
void deleteItem();
void searchItem();

int main() {
    int choice;

    do {
        printf("\n===== INVENTORY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Item\n");
        printf("2. View Items\n");
        printf("3. Update Item\n");
        printf("4. Delete Item\n");
        printf("5. Search Item\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addItem(); break;
            case 2: viewItems(); break;
            case 3: updateItem(); break;
            case 4: deleteItem(); break;
            case 5: searchItem(); break;
            case 6: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);

    return 0;
}

// Add new item and save to file
void addItem() {
    FILE *fp = fopen(FILE_NAME, "a");
    struct Item item;

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Item ID: ");
    scanf("%d", &item.id);

    printf("Enter Item Name: ");
    scanf("%s", item.name);

    printf("Enter Quantity: ");
    scanf("%f", &item.quantity);

    fprintf(fp, "%d %s %.2f\n", item.id, item.name, item.quantity);
    fclose(fp);

    printf("Item added and saved successfully!\n");
}

// View all saved items
void viewItems() {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Item item;

    if (fp == NULL) {
        printf("No inventory data found.\n");
        return;
    }

    printf("\nID   Name               Quantity\n");
    printf("---------------------------------\n");

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        printf("%-4d %-20s %.2f\n", item.id, item.name, item.quantity);
    }

    fclose(fp);
}

// Update item quantity using temp file
void updateItem() {
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Item item;
    int id, found = 0;

    if (fp == NULL || temp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Item ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        if (item.id == id) {
            printf("Current Quantity: %.2f\n", item.quantity);
            printf("Enter New Quantity: ");
            scanf("%f", &item.quantity);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.quantity);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("Item updated successfully!\n");
    else
        printf("Item not found.\n");
}

// Delete item using temp file
void deleteItem() {
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Item item;
    int id, found = 0;

    if (fp == NULL || temp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Item ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        if (item.id == id) {
            found = 1;
        } else {
            fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.quantity);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("Item deleted successfully!\n");
    else
        printf("Item not found.\n");
}

// Search item by ID
void searchItem() {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Item item;
    int id, found = 0;

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    printf("Enter Item ID to search: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        if (item.id == id) {
            printf("\nItem Found!\n");
            printf("ID: %d\n", item.id);
            printf("Name: %s\n", item.name);
            printf("Quantity: %.2f\n", item.quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Item not found.\n");

    fclose(fp);
}
