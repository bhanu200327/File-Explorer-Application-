#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <vector>
#include <cstdio>
using namespace std;

// List directory contents
void listDirectory(const char* path) {
    DIR *dir = opendir(path);
    if (!dir) {
        cout << "Cannot open directory: " << path << "\n";
        return;
    }
    struct dirent *entry;
    cout << "Contents of: " << path << "\n";
    while ((entry = readdir(dir)) != NULL) {
        cout << entry->d_name << "\n";
    }
    closedir(dir);
}

// Change directory
bool changeDirectory(const char* path) {
    return chdir(path) == 0;
}

// Create file
void createFile(const char* filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "New file created");
        fclose(f);
        cout << "File created: " << filename << "\n";
    } else {
        cout << "Cannot create file: " << filename << "\n";
    }
}

// Delete file
void deleteFile(const char* filename) {
    if (remove(filename) == 0) {
        cout << "Deleted file: " << filename << "\n";
    } else {
        cout << "Cannot delete file: " << filename << "\n";
    }
}

// Move/Rename file
void moveFile(const char* source, const char* dest) {
    if (rename(source, dest) == 0) {
        cout << "Moved/Renamed " << source << " to " << dest << "\n";
    } else {
        cout << "Cannot move/rename " << source << " to " << dest << "\n";
    }
}

// Copy file
void copyFile(const char* src, const char* dst) {
    FILE *in = fopen(src, "rb");
    if (!in) { cout << "Cannot open source file.\n"; return; }
    FILE *out = fopen(dst, "wb");
    if (!out) { cout << "Cannot create destination file.\n"; fclose(in); return; }
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0)
        fwrite(buffer, 1, bytes, out);
    fclose(in);
    fclose(out);
    cout << "Copied " << src << " to " << dst << "\n";
}

// Search for file in directory (non-recursive)
void searchFile(const char* path, const char* filename) {
    DIR *dir = opendir(path);
    if (!dir) { cout << "Cannot open directory for search.\n"; return; }
    struct dirent *entry;
    bool found = false;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, filename)) {
            cout << "Found: " << entry->d_name << "\n";
            found = true;
        }
    }
    if (!found) cout << "File not found in " << path << "\n";
    closedir(dir);
}

// Show permissions
void showPermissions(const char* filename) {
    struct stat info;
    if (stat(filename, &info) == 0) {
        cout << "Permissions for " << filename << ": ";
        cout << ((info.st_mode & S_IRUSR) ? "r" : "-")
             << ((info.st_mode & S_IWUSR) ? "w" : "-")
             << ((info.st_mode & S_IXUSR) ? "x" : "-") << "\n";
    } else
        cout << "File does not exist\n";
}

// Change to read-only
void setReadOnly(const char* filename) {
    if (chmod(filename, S_IRUSR | S_IRGRP | S_IROTH) == 0) {
        cout << "Changed " << filename << " to read-only\n";
    } else {
        cout << "Failed to change permissions\n";
    }
}

int main() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "Starting in directory: " << cwd << "\n";
    int choice;
    do {
        cout << "\nFile Explorer Menu:\n";
        cout << "1. List Directory\n";
        cout << "2. Change Directory\n";
        cout << "3. Create File\n";
        cout << "4. Delete File\n";
        cout << "5. Move/Rename File\n";
        cout << "6. Copy File\n";
        cout << "7. Search File\n";
        cout << "8. Show Permissions\n";
        cout << "9. Set Read-Only Permissions\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) { // handle non-numeric input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();
        if (choice == 1) {
            getcwd(cwd, sizeof(cwd));
            listDirectory(cwd);
        } else if (choice == 2) {
            char path[1024];
            cout << "Enter directory path: ";
            cin.getline(path, sizeof(path));
            if (changeDirectory(path))
                cout << "Changed directory successfully.\n";
            else
                cout << "Failed to change directory.\n";
        } else if (choice == 3) {
            char filename[1024];
            cout << "Enter new file name: ";
            cin.getline(filename, sizeof(filename));
            createFile(filename);
        } else if (choice == 4) {
            char filename[1024];
            cout << "Enter file name to delete: ";
            cin.getline(filename, sizeof(filename));
            deleteFile(filename);
        } else if (choice == 5) {
            char source[1024], dest[1024];
            cout << "Enter source file name: ";
            cin.getline(source, sizeof(source));
            cout << "Enter destination file name: ";
            cin.getline(dest, sizeof(dest));
            moveFile(source, dest);
        } else if (choice == 6) {
            char source[1024], dest[1024];
            cout << "Enter source file name: ";
            cin.getline(source, sizeof(source));
            cout << "Enter destination file name: ";
            cin.getline(dest, sizeof(dest));
            copyFile(source, dest);
        } else if (choice == 7) {
            char filename[1024];
            cout << "Enter file name to search: ";
            cin.getline(filename, sizeof(filename));
            getcwd(cwd, sizeof(cwd));
            searchFile(cwd, filename);
        } else if (choice == 8) {
            char filename[1024];
            cout << "Enter file name to show permissions: ";
            cin.getline(filename, sizeof(filename));
            showPermissions(filename);
        } else if (choice == 9) {
            char filename[1024];
            cout << "Enter file name to set read-only: ";
            cin.getline(filename, sizeof(filename));
            setReadOnly(filename);
        }
    } while (choice != 0);
    cout << "Exiting File Explorer.\n";
    return 0;
}
