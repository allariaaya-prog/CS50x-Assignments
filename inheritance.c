#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct for a person
typedef struct person
{
    char *name;
    struct person *parents[2];   // Up to 2 parents
    struct person *children[10]; // Up to 10 children (adjust as needed)
    int num_children;
    long inheritance;
    bool alive;
} person;

// Global variables
person *first_person = NULL; // Head of the linked list

// Function prototypes
person *load(char *filename);
void unload(person *p);
person *find_person(person *p, char *name);
person *insert_person(person *p, char *name, char *parent1, char *parent2);
void delete_person(person *p, char *name);
long inheritance(person *p, char *name);
void print_family(person *p, int depth);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./inheritance filename.csv\n");
        return 1;
    }

    // Load family tree
    person *family = load(argv[1]);
    if (family == NULL)
    {
        printf("Error loading family tree.\n");
        return 1;
    }

    // Example usage (you can expand this for interactive queries)
    printf("Family tree loaded.\n");
    print_family(family, 0);

    // Example: Calculate inheritance for a person
    char *test_name = "Alice"; // Change as needed
    long amount = inheritance(family, test_name);
    printf("%s's inheritance: %ld\n", test_name, amount);

    // Unload memory
    unload(family);
    return 0;
}

person *load(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char line[1024];
    fgets(line, sizeof(line), file); // Skip header

    person *head = NULL;
    while (fgets(line, sizeof(line), file))
    {
        char *name = strtok(line, ",");
        char *parent1 = strtok(NULL, ",");
        char *parent2 = strtok(NULL, ",");

        // Remove newline from parent2 if present
        if (parent2)
        {
            parent2[strcspn(parent2, "\n")] = 0;
        }

        head = insert_person(head, name, parent1, parent2);
    }

    fclose(file);
    return head;
}

void unload(person *p)
{
    if (p == NULL)
    {
        return;
    }

    // Recursively free children
    for (int i = 0; i < p->num_children; i++)
    {
        unload(p->children[i]);
    }

    // Free name and struct
    free(p->name);
    free(p);
}

person *find_person(person *p, char *name)
{
    if (p == NULL || strcmp(p->name, name) == 0)
    {
        return p;
    }

    // Search children recursively
    for (int i = 0; i < p->num_children; i++)
    {
        person *found = find_person(p->children[i], name);
        if (found != NULL)
        {
            return found;
        }
    }

    return NULL;
}

person *insert_person(person *p, char *name, char *parent1, char *parent2)
{
    // Check if person already exists
    person *existing = find_person(p, name);
    if (existing != NULL)
    {
        return p; // Already exists
    }

    // Create new person
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        return p;
    }

    // new_person->name = strcmp(name);
    new_person->parents[0] = NULL;
    new_person->parents[1] = NULL;
    new_person->num_children = 0;
    new_person->inheritance = 0;
    new_person->alive = true;

    // Set parents if they exist
    if (parent1 && strcmp(parent1, "") != 0)
    {
        new_person->parents[0] = find_person(p, parent1);
        if (new_person->parents[0] == NULL)
        {
            // Parent doesn't exist, create them
            p = insert_person(p, parent1, NULL, NULL);
            new_person->parents[0] = find_person(p, parent1);
        }
        // Add as child to parent
        if (new_person->parents[0]->num_children < 10)
        {
            new_person->parents[0]->children[new_person->parents[0]->num_children++] = new_person;
        }
    }

    if (parent2 && strcmp(parent2, "") != 0)
    {
        new_person->parents[1] = find_person(p, parent2);
        if (new_person->parents[1] == NULL)
        {
            p = insert_person(p, parent2, NULL, NULL);
            new_person->parents[1] = find_person(p, parent2);
        }
        if (new_person->parents[1]->num_children < 10)
        {
            new_person->parents[1]->children[new_person->parents[1]->num_children++] = new_person;
        }
    }

    // If no head, this is the first person
    if (p == NULL)
    {
        return new_person;
    }

    // Otherwise, return the head (tree is built via pointers)
    return p;
}

void delete_person(person *p, char *name)
{
    person *to_delete = find_person(p, name);
    if (to_delete == NULL)
    {
        return;
    }

    // Remove from parents' children lists
    for (int i = 0; i < 2; i++)
    {
        if (to_delete->parents[i])
        {
            for (int j = 0; j < to_delete->parents[i]->num_children; j++)
            {
                if (to_delete->parents[i]->children[j] == to_delete)
                {
                    // Shift remaining children
                    for (int k = j; k < to_delete->parents[i]->num_children - 1; k++)
                    {
                        to_delete->parents[i]->children[k] = to_delete->parents[i]->children[k + 1];
                    }
                    to_delete->parents[i]->num_children--;
                    break;
                }
            }
        }
    }

    // Recursively delete children (or reassign, but for simplicity, delete)
    for (int i = 0; i < to_delete->num_children; i++)
    {
        delete_person(to_delete->children[i], to_delete->children[i]->name);
    }

    // Free memory
    free(to_delete->name);
    free(to_delete);
}

long inheritance(person *p, char *name)
{
    person *person = find_person(p, name);
    if (person == NULL || !person->alive)
    {
        return 0;
    }

    long total = person->inheritance;

    // Add inheritance from children (recursive)
    for (int i = 0; i < person->num_children; i++)
    {
        total += inheritance(person->children[i], person->children[i]->name) / person->num_children;
    }

    return total;
}

void print_family(person *p, int depth)
{
    if (p == NULL)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("%s\n", p->name);

    for (int i = 0; i < p->num_children; i++)
    {
        print_family(p->children[i], depth + 1);
    }
}
