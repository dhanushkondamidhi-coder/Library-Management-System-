#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "events.dat"

typedef struct {
    int id;
    char name[50];
    char date[20];
    char venue[50];
    char coordinator[50];
} Event;

void addEvent();
void viewEvents();
void searchEvent();
void deleteEvent();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while (1) {
        printf("\n===== College Event Management System =====\n");
        printf("1. Add New Event\n");
        printf("2. View All Events\n");
        printf("3. Search Event by ID\n");
        printf("4. Delete Event by ID\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                addEvent();
                break;
            case 2:
                viewEvents();
                break;
            case 3:
                searchEvent();
                break;
            case 4:
                deleteEvent();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}

void addEvent() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Event e;
    printf("\nEnter Event ID: ");
    scanf("%d", &e.id);
    getchar();

    printf("Enter Event Name: ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = 0;

    printf("Enter Event Date (DD/MM/YYYY): ");
    fgets(e.date, sizeof(e.date), stdin);
    e.date[strcspn(e.date, "\n")] = 0;

    printf("Enter Venue: ");
    fgets(e.venue, sizeof(e.venue), stdin);
    e.venue[strcspn(e.venue, "\n")] = 0;

    printf("Enter Coordinator Name: ");
    fgets(e.coordinator, sizeof(e.coordinator), stdin);
    e.coordinator[strcspn(e.coordinator, "\n")] = 0;

    fwrite(&e, sizeof(Event), 1, fp);
    fclose(fp);

    printf("\nEvent added successfully!\n");
}

void viewEvents() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No events found!\n");
        return;
    }

    Event e;
    printf("\n%-5s %-20s %-15s %-20s %-20s\n", "ID", "Name", "Date", "Venue", "Coordinator");
    printf("-----------------------------------------------------------------------------\n");

    while (fread(&e, sizeof(Event), 1, fp)) {
        printf("%-5d %-20s %-15s %-20s %-20s\n", e.id, e.name, e.date, e.venue, e.coordinator);
    }

    fclose(fp);
}

void searchEvent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No events found!\n");
        return;
    }

    int id, found = 0;
    Event e;
    printf("\nEnter Event ID to search: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Event), 1, fp)) {
        if (e.id == id) {
            printf("\nEvent Found!\n");
            printf("ID: %d\nName: %s\nDate: %s\nVenue: %s\nCoordinator: %s\n",
                   e.id, e.name, e.date, e.venue, e.coordinator);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Event not found!\n");

    fclose(fp);
}

void deleteEvent() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No events to delete!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    int id, found = 0;
    Event e;
    printf("\nEnter Event ID to delete: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Event), 1, fp)) {
        if (e.id == id) {
            found = 1;
            continue; // skip writing this record
        }
        fwrite(&e, sizeof(Event), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Event deleted successfully!\n");
    else
        printf("Event not found!\n");
}
