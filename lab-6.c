/******************************************************************************
 * File: lab-6.c
 * 
 * Personal Fitness Tracker - Dynamic Memory Implementation
 * This program implements a fitness tracking system using dynamic memory
 * allocation for storing workout data. It includes functionality for managing
 * workouts, sorting them by various criteria, and searching for specific
 * workouts.
 * 
 * CSCE 206: Structured Programming in C
 * Fall 2024
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define MIN_CAPACITY 4

// Structure to store date information
struct Date {
    int day;
    int month;
    int year;
};

// Structure to store workout information
struct Workout {
    struct Date date;
    char type[20];
    int duration;      // in minutes
    int calories_burned;
};

// Structure to manage the dynamic array of workouts
struct WorkoutList {
    struct Workout* workouts;  // Dynamic array of workouts
    int capacity;             // Current capacity of the array
    int count;               // Current number of workouts
};

// Function prototypes
void printMenu(void);
int initializeWorkoutList(struct WorkoutList* list, int initialCapacity);
int resizeWorkoutList(struct WorkoutList* list, int newCapacity);
void freeWorkoutList(struct WorkoutList* list);
int addWorkout(struct WorkoutList* list);
void displayWorkout(const struct Workout* workout);
void displayAllWorkouts(const struct WorkoutList* list);
void selectionSortByDuration(struct WorkoutList* list);
void bubbleSortByCalories(struct WorkoutList* list);
void linearSearchByType(const struct WorkoutList* list, const char* type);
int binarySearchByDuration(const struct WorkoutList* list, int targetDuration);

/**
 * Main function that runs the fitness tracker program.
 * Initializes the workout list, handles the menu loop, and manages program flow.
 * Ensures proper cleanup of allocated memory before program termination.
 * 
 * @return 0 for successful execution, 1 for initialization failure
 */
int main() {
    struct WorkoutList list;
    int choice;

    if (!initializeWorkoutList(&list, INITIAL_CAPACITY)) {
        printf("Failed to initialize workout list. Exiting.\n");
        return 1;
    }

    printf("Welcome to the Personal Fitness Tracker!\n");

    do {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (!addWorkout(&list)) {
                    printf("Failed to add workout.\n");
                }
                break;
            case 2:
                displayAllWorkouts(&list);
                break;
            case 3:
                selectionSortByDuration(&list);
                printf("Workouts sorted by duration.\n");
                displayAllWorkouts(&list);
                break;
            case 4:
                bubbleSortByCalories(&list);
                printf("Workouts sorted by calories burned.\n");
                displayAllWorkouts(&list);
                break;
            case 5:
                {
                    char searchType[20];
                    printf("Enter workout type to search: ");
                    scanf("%19s", searchType);
                    linearSearchByType(&list, searchType);
                }
                break;
            case 6:
                {
                    int targetDuration;
                    printf("Enter duration to search for: ");
                    scanf("%d", &targetDuration);
                    selectionSortByDuration(&list);
                    int result = binarySearchByDuration(&list, targetDuration);
                    if (result != -1) {
                        printf("Workout found:\n");
                        displayWorkout(&list.workouts[result]);
                    } else {
                        printf("No workout found with duration %d minutes.\n", targetDuration);
                    }
                }
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    freeWorkoutList(&list);
    return 0;
}

/**
 * Displays the main menu options for the user.
 * Lists all available operations that can be performed on the workout data.
 */
void printMenu() {
    printf("\nMain Menu:\n");
    printf("1. Add New Workout\n");
    printf("2. Display All Workouts\n");
    printf("3. Sort Workouts by Duration\n");
    printf("4. Sort Workouts by Calories Burned\n");
    printf("5. Search Workouts by Type\n");
    printf("6. Search Workout by Duration\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

/**
 * Initializes a new workout list with the specified initial capacity.
 * Allocates memory for the workout array and initializes counter variables.
 * 
 * @param list Pointer to the WorkoutList to initialize
 * @param initialCapacity Initial size of the workout array
 * @return 1 if initialization successful, 0 if memory allocation failed
 */
int initializeWorkoutList(struct WorkoutList* list, int initialCapacity) {
    list->workouts = malloc(initialCapacity * sizeof(struct Workout));
    if (list->workouts == NULL) {
        return 0;
    }
    list->capacity = initialCapacity;
    list->count = 0;
    return 1;
}

/**
 * Resizes the workout array to a new capacity.
 * Creates a new array with the specified capacity and copies existing workouts.
 * 
 * @param list Pointer to the WorkoutList to resize
 * @param newCapacity New size for the workout array
 * @return 1 if resize successful, 0 if memory allocation failed
 */
int resizeWorkoutList(struct WorkoutList* list, int newCapacity) {
    if (newCapacity < MIN_CAPACITY) {
        newCapacity = MIN_CAPACITY;
    }
    
    struct Workout* newArray = malloc(newCapacity * sizeof(struct Workout));
    if (newArray == NULL) {
        return 0;
    }

    for (int i = 0; i < list->count; i++) {
        newArray[i] = list->workouts[i];
    }

    free(list->workouts);
    list->workouts = newArray;
    list->capacity = newCapacity;
    return 1;
}

/**
 * Frees all dynamically allocated memory associated with the workout list.
 * Resets capacity and count to 0.
 * 
 * @param list Pointer to the WorkoutList to free
 */
void freeWorkoutList(struct WorkoutList* list) {
    if (list->workouts != NULL) {
        free(list->workouts);
        list->workouts = NULL;
    }
    list->capacity = 0;
    list->count = 0;
}

/**
 * Adds a new workout to the workout list.
 * Resizes the array if necessary and handles user input for workout details.
 * 
 * @param list Pointer to the WorkoutList
 * @return 1 if workout added successfully, 0 if memory allocation failed
 */
int addWorkout(struct WorkoutList* list) {
    if (list->count == list->capacity) {
        if (!resizeWorkoutList(list, list->capacity * 2)) {
            return 0;
        }
    }

    printf("Enter workout date (DD MM YYYY): ");
    scanf("%d %d %d", 
          &list->workouts[list->count].date.day,
          &list->workouts[list->count].date.month,
          &list->workouts[list->count].date.year);

    printf("Enter workout type: ");
    scanf("%19s", list->workouts[list->count].type);

    printf("Enter workout duration (in minutes): ");
    scanf("%d", &list->workouts[list->count].duration);

    printf("Enter calories burned: ");
    scanf("%d", &list->workouts[list->count].calories_burned);

    list->count++;
    printf("Workout added successfully.\n");

    // Check if we should shrink the array
    if (list->count < list->capacity / 4 && list->capacity > MIN_CAPACITY) {
        resizeWorkoutList(list, list->capacity / 2);
    }

    return 1;
}

/**
 * Displays information for a single workout.
 * 
 * @param workout Pointer to the Workout to display
 */
void displayWorkout(const struct Workout* workout) {
    printf("Date: %02d/%02d/%04d, Type: %s, Duration: %d minutes, Calories burned: %d\n",
           workout->date.day, workout->date.month, workout->date.year,
           workout->type, workout->duration, workout->calories_burned);
}

/**
 * Displays all workouts in the list.
 * 
 * @param list Pointer to the WorkoutList containing all workouts
 */
void displayAllWorkouts(const struct WorkoutList* list) {
    if (list->count == 0) {
        printf("No workouts recorded yet.\n");
        return;
    }
    printf("\nAll Recorded Workouts:\n");
    for (int i = 0; i < list->count; i++) {
        printf("%d. ", i + 1);
        displayWorkout(&list->workouts[i]);
    }
}

/**
 * Sorts the workouts by duration using Selection Sort algorithm.
 * 
 * @param list Pointer to the WorkoutList to be sorted
 */
void selectionSortByDuration(struct WorkoutList* list) {
    for (int i = 0; i < list->count - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < list->count; j++) {
            if (list->workouts[j].duration < list->workouts[min_idx].duration) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            struct Workout temp = list->workouts[i];
            list->workouts[i] = list->workouts[min_idx];
            list->workouts[min_idx] = temp;
        }
    }
}

/**
 * Sorts the workouts by calories burned using Bubble Sort algorithm.
 * 
 * @param list Pointer to the WorkoutList to be sorted
 */
void bubbleSortByCalories(struct WorkoutList* list) {
    for (int i = 0; i < list->count - 1; i++) {
        for (int j = 0; j < list->count - i - 1; j++) {
            if (list->workouts[j].calories_burned > list->workouts[j + 1].calories_burned) {
                struct Workout temp = list->workouts[j];
                list->workouts[j] = list->workouts[j + 1];
                list->workouts[j + 1] = temp;
            }
        }
    }
}

/**
 * Performs a linear search to find workouts by type.
 * 
 * @param list Pointer to the WorkoutList to search
 * @param type The workout type to search for
 */
void linearSearchByType(const struct WorkoutList* list, const char* type) {
    int found = 0;
    printf("Workouts of type '%s':\n", type);
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->workouts[i].type, type) == 0) {
            displayWorkout(&list->workouts[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No workouts found of type: %s\n", type);
    }
}

/**
 * Performs a binary search to find a workout with specific duration.
 * Assumes the workout list is sorted by duration.
 * 
 * @param list Pointer to the WorkoutList to search
 * @param targetDuration The duration to search for
 * @return Index of the matching workout if found, -1 otherwise
 */
int binarySearchByDuration(const struct WorkoutList* list, int targetDuration) {
    int low = 0;
    int high = list->count - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (list->workouts[mid].duration == targetDuration) {
            return mid;
        }
        if (list->workouts[mid].duration < targetDuration) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}