#include <stdio.h>
#define FILE_NAME "inventory.txt" // Файл за съхранение на данните

struct Item {
    int id;             // Уникален идентификатор на артикула
    char name[30];      // Име на артикула
    float quantity;     // Количество на артикула
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

// Добавяне на нов артикул и запис във файла
void addItem() {
    FILE *fp = fopen(FILE_NAME, "a"); // Отваряне на файла за добавяне
    struct Item item;

    if (fp == NULL) { // Проверка дали файлът е достъпен
        printf("File error!\n");
        return;
    }

    printf("Enter Item ID: ");
    scanf("%d", &item.id);

    printf("Enter Item Name: ");
    scanf("%s", item.name);

    printf("Enter Quantity: ");
    scanf("%f", &item.quantity);

    fprintf(fp, "%d %s %.2f\n", item.id, item.name, item.quantity); // Запис в файла
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
    printf("\nID   Name        Quantity\n");
    printf("--------------------------\n");

    // Четене на всеки ред и отпечатване
    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        printf("%-4d %-10s %.2f\n", item.id, item.name, item.quantity);
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

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        if (item.id == id) { // Ако е намерен артикулът
            printf("Current Quantity: %.2f\n", item.quantity);
            printf("Enter New Quantity: ");
            scanf("%f", &item.quantity); // Актуализиране на количеството
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.quantity); // Запис във временния файл
    }

    fclose(fp);
    fclose(temp);

    // Заместване на оригиналния файл
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

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        if (item.id == id) {
            found = 1; // Пропускаме записа, за да се изтрие
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

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.quantity) != EOF) {
        if (item.id == id) {
            printf("\nItem Found!\nID: %d\nName: %s\nQuantity: %.2f\n", item.id, item.name, item.quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Item not found.\n");

    fclose(fp);
}
