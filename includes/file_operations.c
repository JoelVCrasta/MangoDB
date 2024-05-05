#include <stdio.h>
#include <stdbool.h>



int insert_record (char *usn , char *name, char *marks) {
    FILE *file;
    char temp_usn[50];
    char temp_name[50];
    char temp_marks[50];

    // Check if the file exists
    file = fopen("students.txt", "a+");
    if (file == NULL) {
        printf("Error opening file\n");
        return 2;
    }

    // Check if the USN already exists
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", temp_usn, temp_name, temp_marks)!= EOF) {
        if (strcmp(usn, temp_usn) == 0) {
            printf("USN %s already exists.\n", usn);
            fclose(file);
            return 1;
        }
    }

    // If the USN does not exist then insert the record
    fprintf(file, "%s,%s,%s\n", usn, name, marks);

    // Close the file
    fclose(file);

    return 0;
}


int delete_record (char *usn) {
    FILE *file;
    char temp_name[50];
    char temp_usn[50];
    char temp_marks[50];
    bool found = false;

    // Check if the file exists
    file = fopen("students.txt", "r");
    if(file == NULL) {
        printf("Error opening file\n");
        return 2;
    }

    // Create a temp file
    FILE *new_file = fopen("students.txt.tmp", "w");
    if (new_file == NULL) {
        printf("Error creating new file\n");
        fclose(file);
        return 2;
    }

    // Read all the records except the one to be deleted and write them to the temp file
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", temp_usn, temp_name, temp_marks)!= EOF) {
        if (strcmp(temp_usn, usn)!= 0) {
            fprintf(new_file, "%s,%s,%s\n", temp_usn, temp_name, temp_marks);
        }
        else
        {
            found = true;
        }
    }

    // Close both files
    fclose(file);
    fclose(new_file);

    // If the record was not found
    if (!found) {
        printf("Record with USN %s not found.\n", usn);
        remove("students.txt.tmp");
        return 1;
    }

    // Remove the original file and rename the new file to the original file's name
    remove("students.txt");
    rename("students.txt.tmp", "students.txt");

    return 0;
}


int update_record(char *usn, char *new_name, char *new_marks) {
    FILE *file;
    char temp_usn[50];
    char temp_name[50];
    char temp_marks[50];
    bool found = false;

    // Check if the file exists
    file = fopen("students.txt", "r");
    if(file == NULL) {
        printf("Error opening file\n");
        return 2;
    }

    // Create a temp file
    FILE *new_file = fopen("students.txt.tmp", "w");
    if (new_file == NULL) {
        printf("Error creating new file\n");
        fclose(file);
        return 2;
    }

    // Read all the records except the one to be updated and write them to the temp file
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", temp_usn, temp_name, temp_marks)!= EOF) {
        if (strcmp(temp_usn, usn)!= 0) {
            fprintf(new_file, "%s,%s,%s\n", temp_usn, temp_name, temp_marks);
        }
        else
        {
            found = true;
            fprintf(new_file, "%s,%s,%s\n", temp_usn, new_name, new_marks);
        }
    }

    // Close both files
    fclose(file);
    fclose(new_file);

    // If the record was not found
    if (!found) {
        printf("Record with USN %s not found.\n", usn);
        remove("students.txt.tmp");
        return 1;
    }

    // Remove the original file and rename the new file to the original file's name
    remove("students.txt");
    rename("students.txt.tmp", "students.txt");

    return 0;
}

char** search_record (char *usn) {

    FILE *file;
    char temp_usn[50];
    char temp_name[50];
    char temp_marks[50];
    bool found = false;

    // Check if the file exists
    file = fopen("students.txt", "r");
    if(file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // Search for the record in the file
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", temp_usn, temp_name, temp_marks)!= EOF) {
        if (strcmp(temp_usn, usn)== 0) {
            found = true;
            break;
        }
    }

    // Close the file
    fclose(file);

    // If the record was not found
    if (!found) {
        return NULL;
    }

    char **data = (char **)malloc(2 * sizeof(char *));
    data[0] = (char *)malloc(50 * sizeof(char));
    data[1] = (char *)malloc(50 * sizeof(char));

    strcpy(data[0], temp_name);
    strcpy(data[1], temp_marks);

    return data;
}  


typedef struct {
    char usn[50];
    char name[50];
    char marks[50];
} Record;


Record* display_records() {
    FILE *file;
    char temp_usn[50];
    char temp_name[50];
    char temp_marks[50];
    int count = 0;

    // Check if the file exists
    file = fopen("students.txt", "r");
    if(file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // Count the number of records
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", temp_usn, temp_name, temp_marks)!= EOF) {
        count++;
    }

    // Return pointer to the beginning of the file
    rewind(file);

    // Allocate memory for the records
    Record *data = (Record *)malloc((count) * sizeof(Record));

    // Read the records
    for (int i = 0; i < count; i++) {
        fscanf(file, "%[^,],%[^,],%[^\n]\n", data[i].usn, data[i].name, data[i].marks);
    }

    printf("USN \t Name \t Marks\n");
    for (int i = 0; i < count; i++) {
        printf("%s \t %s \t %s\n", data[i].usn, data[i].name, data[i].marks);
    }

    // Close the file
    fclose(file);

    return data;
}

int get_record_count() {
    FILE *file;
    char temp_usn[50];
    char temp_name[50];
    char temp_marks[50];
    int count = 0;

    // Check if the file exists
    file = fopen("students.txt", "r");
    if(file == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    // Count the number of records
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", temp_usn, temp_name, temp_marks)!= EOF) {
        count++;
    }

    // Close the file
    fclose(file);

    return count;
}