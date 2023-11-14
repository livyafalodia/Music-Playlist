#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Define a structure to represent a song
struct Song {
    char title[100];
    char artist[100];
    struct Song* next;
};

// Function to create a new song node
struct Song* createSong(char title[], char artist[]) {
    struct Song* newSong = (struct Song*)malloc(sizeof(struct Song));
    if (newSong == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);
    newSong->next = NULL;
    return newSong;
}

// Function to add a song to the end of the playlist
void addToPlaylist(struct Song** playlist, char title[], char artist[]) {
    struct Song* newSong = createSong(title, artist);

    if (*playlist == NULL) {
        // If the playlist is empty, make the new song the only song in the list
        *playlist = newSong;
        newSong->next = newSong;  // Circular reference
    } else {
        // Otherwise, add the new song to the end of the playlist
        struct Song* lastSong = (*playlist)->next;
        (*playlist)->next = newSong;
        newSong->next = lastSong;
        *playlist = newSong;
    }

    printf("Added '%s' by %s to the playlist.\n", title, artist);
}

// Function to remove a song from the playlist
void removeFromPlaylist(struct Song** playlist, char title[], char artist[]) {
    if (*playlist == NULL) {
        printf("The playlist is empty.\n");
        return;
    }

    struct Song* current = (*playlist)->next;
    struct Song* prev = *playlist;

    do {
        if (strcmp(current->title, title) == 0 && strcmp(current->artist, artist) == 0) {
            // Found the song to remove
            prev->next = current->next;
            free(current);
            printf("Removed '%s' by %s from the playlist.\n", title, artist);
            return;
        }
        prev = current;
        current = current->next;
    } while (current != (*playlist)->next);

    printf("'%s' by %s not found in the playlist.\n", title, artist);
}

// Function to shuffle the playlist
void shufflePlaylist(struct Song** playlist) {
    if (*playlist == NULL) {
        printf("The playlist is empty.\n");
        return;
    }

    printf("Shuffling the playlist...\n");

    struct Song* current = (*playlist)->next;
    struct Song* temp;
    int count = 0;

    do {
        count++;
        current = current->next;
    } while (current != (*playlist)->next);

    for (int i = 0; i < count * 2; i++) {
        int rand1 = rand() % count;
        int rand2 = rand() % count;

        struct Song* song1 = (*playlist)->next;
        struct Song* song2 = (*playlist)->next;

        for (int j = 0; j < rand1; j++) {
            song1 = song1->next;
        }

        for (int j = 0; j < rand2; j++) {
            song2 = song2->next;
        }

        // Swap the titles and artists of two songs to simulate shuffling
        strcpy(temp->title, song1->title);
        strcpy(temp->artist, song1->artist);
        strcpy(song1->title, song2->title);
        strcpy(song1->artist, song2->artist);
        strcpy(song2->title, temp->title);
        strcpy(song2->artist, temp->artist);
    }
}

// Function to display the current playlist
void displayPlaylist(struct Song* playlist) {
    if (playlist == NULL) {
        printf("The playlist is empty.\n");
        return;
    }

    struct Song* current = playlist->next;

    printf("Current Playlist:\n");

    do {
        printf("'%s' by %s\n", current->title, current->artist);
        current = current->next;
    } while (current != playlist->next);
}

// Function to save the playlist to a file
void savePlaylistToFile(struct Song* playlist, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file) {
        struct Song* current = playlist->next;
        do {
            fprintf(file, "%s,%s\n", current->artist, current->title);
            current = current->next;
        } while (current != playlist->next);
        fclose(file);
        printf("Playlist saved to %s.\n", filename);
    } else {
        printf("Failed to save the playlist to %s.\n", filename);
    }
}

// Function to load a playlist from a file
struct Song* loadPlaylistFromFile(const char* filename) {
    struct Song* loadedPlaylist = NULL;
    FILE* file = fopen(filename, "r");
    if (file) {
        char title[100];
        char artist[100];
        while (fscanf(file, "%[^,],%[^\n]\n", artist, title) == 2) {
            addToPlaylist(&loadedPlaylist, title, artist);
        }
        fclose(file);
        printf("Playlist loaded from %s.\n", filename);
    } else {
        printf("Failed to load the playlist from %s.\n", filename);
    }
    return loadedPlaylist;
}

int main() {
    struct Song* playlist = NULL;
    int choice;
    char title[100];
    char artist[100];

    while (1) {
        printf("Music Playlist Manager\n");
        printf("1. Add a song to the playlist\n");
        printf("2. Remove a song from the playlist\n");
        printf("3. Shuffle the playlist\n");
        printf("4. Display the playlist\n");
        printf("5. Save playlist to a file\n");
        printf("6. Load playlist from a file\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the title of the song: ");
                scanf(" %[^\n]s", title);
                printf("Enter the artist of the song: ");
                scanf(" %[^\n]s", artist);
                addToPlaylist(&playlist, title, artist);
                break;
            case 2:
                printf("Enter the title of the song to remove: ");
                scanf(" %[^\n]s", title);
                printf("Enter the artist of the song to remove: ");
                scanf(" %[^\n]s", artist);
                removeFromPlaylist(&playlist, title, artist);
                break;
            case 3:
                shufflePlaylist(&playlist);
                break;
            case 4:
                displayPlaylist(playlist);
                break;
            case 5:
                printf("Enter the filename to save the playlist: ");
                char saveFilename[100];
                scanf(" %[^\n]s", saveFilename);
                savePlaylistToFile(playlist, saveFilename);
                break;
            case 6:
                printf("Enter the filename to load the playlist from: ");
                char loadFilename[100];
                scanf(" %[^\n]s", loadFilename);
                playlist = loadPlaylistFromFile(loadFilename);
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}