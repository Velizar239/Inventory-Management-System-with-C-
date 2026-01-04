#include <stdio.h>
#include <string.h> 

#define FILE_NAME "inventory.txt" // Файл за съхранение на данните

struct Item {
    int id;             // Уникален идентификатор на артикула
    char name[30];      // Име на артикула
    char type[20];      // Тип на артикула: RawMaterial / FinishedProduct / Tool
    float quantity;     // Количество на артикула
    char unit[5];       // Мярка: pcs / m / kg / m2
};

// Прототипи на функциите
void addItem();
void viewItems();
void updateItem();
void deleteItem();
void searchItem();

int main() {
    int choice;

    do {
        // Меню за потребителя
        printf("\n===== INVENTORY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Item\n2. View Items\n3. Update Item\n4. Delete Item\n5. Search Item\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Изпълнение на съответната функция
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
// Проверка за валиден тип
int validType(char *type) {
    return strcmp(type, "RawMaterial") == 0 || strcmp(type, "FinishedProduct") == 0 || strcmp(type, "Tool") == 0;
}

// Проверка за валидна единица
int validUnit(char *unit) {
    return strcmp(unit, "pcs") == 0 || strcmp(unit, "m") == 0 || strcmp(unit, "m2") == 0 || strcmp(unit, "kg") == 0;
}

// Добавяне на нов артикул и запис във файла
void addItem() {
    FILE *fp = fopen(FILE_NAME, "a");
    struct Item item;

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Item ID: ");
    scanf("%d", &item.id);

    printf("Enter Item Name (one word): ");
    scanf("%s", item.name);

    // Проверка за валиден тип
    do {
        printf("Enter Item Type (RawMaterial / FinishedProduct / Tool): ");
        scanf("%s", item.type);
        if (!validType(item.type)) {
            printf("Invalid type! Try again.\n");
        }
    } while (!validType(item.type));

    printf("Enter Quantity: ");
    scanf("%f", &item.quantity);

    // Проверка за валидна единица
    do {
        printf("Enter Unit (pcs / m / m2 / kg): ");
        scanf("%s", item.unit);
        if (!validUnit(item.unit)) {
            printf("Invalid unit! Try again.\n");
        }
    } while (!validUnit(item.unit));

    fprintf(fp, "%d %s %s %.2f %s\n", item.id, item.name, item.type, item.quantity, item.unit);
    fclose(fp);

    printf("Item added and saved successfully!\n");
}

// Преглед на всички артикули
void viewItems() {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Item item;

    if (fp == NULL) { // Проверка дали има данни
        printf("No inventory data found.\n");
        return;
    }

    // Заглавие на таблицата
    printf("\nID   Name                 Type                 Quantity Unit\n");
    printf("-------------------------------------------------------------\n");

    // Четене на всеки ред и отпечатване
    while (fscanf(fp, "%d %s %s %f %s", &item.id, item.name, item.type, &item.quantity, item.unit) != EOF) {
        printf("%-4d %-20s %-20s %-8.2f %-4s\n", item.id, item.name, item.type, item.quantity, item.unit);
    }

    fclose(fp);
}

// Актуализиране на количество на артикул
void updateItem() {
    FILE *fp = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w"); // Временен файл за обновяване
    struct Item item;
    int id, found = 0;

    if (fp == NULL || temp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Item ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %s %f %s", &item.id, item.name, item.type, &item.quantity, item.unit) != EOF) {
        if (item.id == id) { // Ако е намерен артикулът
            printf("Current Quantity: %.2f %s\n", item.quantity, item.unit);
            printf("Enter New Quantity: ");
            scanf("%f", &item.quantity); // Актуализиране на количеството
            found = 1;
        }
        fprintf(temp, "%d %s %s %.2f %s\n", item.id, item.name, item.type, item.quantity, item.unit); // Запис във временния файл
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

// Изтриване на артикул
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

    while (fscanf(fp, "%d %s %s %f %s", &item.id, item.name, item.type, &item.quantity, item.unit) != EOF) {
        if (item.id == id) {
            found = 1; // Пропускаме записа, за да се изтрие
        } else {
            fprintf(temp, "%d %s %s %.2f %s\n", item.id, item.name, item.type, item.quantity, item.unit);
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

// Търсене на артикул по ID
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

    while (fscanf(fp, "%d %s %s %f %s", &item.id, item.name, item.type, &item.quantity, item.unit) != EOF) {
        if (item.id == id) {
            printf("\nItem Found!\nID: %d\nName: %s\nType: %s\nQuantity: %.2f %s\n",
                   item.id, item.name, item.type, item.quantity, item.unit);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Item not found.\n");

    fclose(fp);
}
