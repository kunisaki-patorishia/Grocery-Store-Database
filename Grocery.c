#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for each grocery item
struct item {
    int id;
    char name[20];
    double cost;
    char category;
    struct item *next;
};

// Function prototypes
void loadDatabase(struct item **head, char *filename, int *saveFlag);
void saveDatabase(struct item *head, char *filename);
void displayMenu(struct item **head, int *saveFlag);
void addItem(struct item **head, int *saveFlag);
void deleteItem(struct item **head, int *saveFlag);
void changeCost(struct item *head, int *saveFlag);
void searchItem(struct item *head);
void displayInventory(struct item *head);

int main(void) {
    // Initialize the head pointer for the linked list
    struct item *head = NULL;
    int saveFlag = 0; // Initialize save flag to false

    // Ask the user for the filename containing the database
    FILE *file;
    char filename[50];
    printf("Enter the filename containing the database: ");
    scanf("%s", filename);

    file = fopen(filename, "r+"); // Open file in read/write mode
    if (file == NULL) {
        char createNew;
        printf("File '%s' does not exist. Do you want to create a new file? (Y/N): ", filename);
        scanf(" %c", &createNew);
        if (createNew == 'Y' || createNew == 'y') {
            file = fopen(filename, "w+");
            if (file == NULL) {
                printf("Error creating file '%s'.\n", filename);
                perror("Error");
                exit(EXIT_FAILURE);
            }
            printf("New file '%s' created successfully.\n", filename);
        } else {
            printf("Exiting program.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("File '%s' opened successfully.\n", filename);
    }

    // Load the database from the file
    loadDatabase(&head, filename, &saveFlag);

    // Display the menu and perform actions based on user input
    displayMenu(&head, &saveFlag);

    // Free dynamically allocated memory before exiting
    // (if needed)

    return 0;
}

// Function to load the database from a file
void loadDatabase(struct item **head, char *filename, int *saveFlag) {
    // Implement this function to read data from the file
    // and create a linked list of items

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    struct item *current = NULL;
    struct item *prev = NULL;

    while (!feof(file)) {
        struct item *newItem = (struct item*)malloc(sizeof(struct item));
        if (newItem == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        if (fscanf(file, "%d %s %lf %c", &newItem->id, newItem->name, &newItem->cost, &newItem->category) != 4) {
            free(newItem);
            break;
        }
        newItem->next = NULL;

        if (*head == NULL) {
            *head = newItem;
        } else {
            prev->next = newItem;
        }

        prev = newItem;
        if (current == NULL) {
            current = newItem;
        }
    }

    fclose(file);

    // Database loaded, set save flag to false
    *saveFlag = 0;
}

// Function to save the database to a file
void saveDatabase(struct item *head, char *filename) {
    // Implement this function to write the linked list
    // of items to a file
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    struct item *current = head;
    while (current != NULL) {
        fprintf(file, "%d %s %.2lf %c\n", current->id, current->name, current->cost, current->category);
        current = current->next;
    }

    fclose(file);
}

// Function to display the main menu and handle user input
void displayMenu(struct item **head, int *saveFlag) {
    // Implement this function to display the menu and
    // call corresponding functions based on user input

    int choice;
    do {
        printf("\nMain Menu:\n");
        printf("1. Add new item\n");
        printf("2. Delete item\n");
        printf("3. Change the cost of an item\n");
        printf("4. Search for item\n");
        printf("5. Display inventory details\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem(head, saveFlag);
                break;
            case 2:
                deleteItem(head, saveFlag);
                break;
            case 3:
                changeCost(*head, saveFlag);
                break;
            case 4:
                searchItem(*head);
                break;
            case 5:
                displayInventory(*head);
                break;
            case 6:
                printf("Exiting program.\n");
                // Check if changes have been made to the database
                if (*saveFlag) {
                    char saveChoice;
                    printf("Save changes to the database? (Y/N): ");
                    scanf(" %c", &saveChoice);
                    if (saveChoice == 'Y' || saveChoice == 'y') {
                        char newFilename[50];
                        printf("Enter the filename to save the changes: ");
                        scanf("%s", newFilename);
                        saveDatabase(*head, newFilename);
                        printf("Changes saved to %s successfully.\n", newFilename);
                    }
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}


// Function to add a new item to the database
void addItem(struct item **head, int *saveFlag) {
    // Implement this function to add a new item to the
    // linked list
    struct item *newItem = (struct item*)malloc(sizeof(struct item));
    if (newItem == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter the ID number of the new item: ");
    scanf("%d", &newItem->id);

    // Check for duplicate ID
    struct item *current = *head;
    while (current != NULL) {
        if (current->id == newItem->id) {
            printf("Error: Item with ID %d already exists.\n", newItem->id);
            free(newItem);
            return;
        }
        current = current->next;
    }

    printf("Enter the name of the new item: ");
    scanf("%s", newItem->name);

    printf("Enter the cost of the new item: ");
    scanf("%lf", &newItem->cost);

    printf("Enter the category of the new item (M for meat, P for produce, D for dairy, C for canned goods, N for nonfoods): ");
    scanf(" %c", &newItem->category);

    newItem->next = *head;
    *head = newItem;

    printf("Item added successfully.\n");

    // Set save flag to true
    *saveFlag = 1;
}


// Function to delete an item from the database
void deleteItem(struct item **head, int *saveFlag) {
    // Implement this function to delete an item from the
    // linked list
    int id;
    printf("Enter the ID number of the item to delete: ");
    scanf("%d", &id);

    struct item *current = *head;
    struct item *prev = NULL;

    while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Item with ID %d not found.\n", id);
        return;
    }

    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("Item with ID %d deleted successfully.\n", id);

    // Set save flag to true
    *saveFlag = 1;
}


// Function to change the cost of an existing item
void changeCost(struct item *head, int *saveFlag) {
    // Implement this function to change the cost of an
    // existing item in the linked list
    int id;
    printf("Enter the ID number of the item to change the cost: ");
    scanf("%d", &id);

    struct item *current = head;
    while (current != NULL && current->id != id) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Item with ID %d not found.\n", id);
        return;
    }

    printf("Enter the new cost for the item with ID %d: ", id);
    scanf("%lf", &current->cost);

    printf("Cost for item with ID %d changed successfully.\n", id);

    // Set save flag to true
    *saveFlag = 1;
}

// Function to search for an item in the database
void searchItem(struct item *head) {
    // Implement this function to search for an item
    // in the linked list and display its details

    char name[20];
    printf("Enter the name of the item to search for: ");
    scanf("%s", name);

    struct item *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Item found:\n");
            printf("ID: %d\n", current->id);
            printf("Name: %s\n", current->name);
            printf("Cost: %.2lf\n", current->cost);
            printf("Category: %c\n", current->category);
            return;
        }
        current = current->next;
    }

    printf("Item with name %s not found.\n", name);

}

// Function to display the inventory details
void displayInventory(struct item *head) {
    // Implement this function to display the inventory
    // details in a tabular format

    printf("ID\tName\tCost\tCategory\n");
    struct item *current = head;
    while (current != NULL) {
        printf("%d\t%s\t%.2lf\t%c\n", current->id, current->name, current->cost, current->category);
        current = current->next;
    }
}

