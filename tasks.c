#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include "lib/commander.h"

static const int BUFFSIZE = 1024;

static char*    find_root();
static FILE*    get_root_file();
static int      get_count();


static void init(command_t *self){

    FILE *fp = fopen(".tasks", "w");

    if(fp == NULL){
        fprintf(stderr, "Can't write to this directory, do you have permission?\n");
        exit(1);
    }
    fclose(fp);
    printf("Tasks initilized in this directory\n");
}

static void add_task(command_t *self) {
    FILE *fp = get_root_file("a+");
    fprintf(fp, "%s\n",self->arg );
    fclose(fp);
}

static void list(command_t *self){
    FILE *fp = get_root_file("r");

    char buff[BUFFSIZE];
    int i = 0;

    while(fgets(buff, sizeof buff, fp) != NULL){
        printf("%d: %s", i, buff);
        i++;
    }
    fclose(fp);
}

static void remove_task(command_t *self){

    FILE *fp = get_root_file("r");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *file = malloc(sizeof(char) * (size + 1));
    char buff[BUFFSIZE];
    int i = 0;

    while(fgets(buff, sizeof buff, fp) != NULL){
        if(i != (int)(*self->arg - '0')) {
            strncat(file, buff, sizeof buff);
        } else {
            printf("%s %d\n", "Removed task", i);
        }
        i++;
    }

    fclose(fp);

    FILE *fwp = get_root_file("w");
    fprintf(fwp, "%s", file);
    fclose(fwp);
    free(file);
}

static void clear(command_t *self){
    FILE *fp = get_root_file("w");
    fclose(fp);
}

static void count(command_t *self){
    FILE *fp = get_root_file("r");
    printf("%d\n", get_count(fp));
    fclose(fp);
}

static int get_count(FILE *fp){
    int count = 0;
    char buff[BUFFSIZE];
    while(fgets(buff, sizeof buff, fp) != NULL){
        count += 1;
    }
    return count;
}

static FILE* get_root_file(char* mode){
    char *path = find_root();
    if(path == NULL){
        printf("%s\n", "Tasks not initilized, Use tasks init first");
        exit(1);
    }
    int len = strlen(path);
    char* file_path = strncat(path, "/.tasks", len);
    FILE *fp = fopen(file_path, mode);
    return fp;
}

static int dir_contains_tasks(const char* dir_name){
    DIR *dir = opendir(dir_name);
    struct dirent *ent = NULL;
    if ((dir = opendir (dir_name)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(strcmp(ent->d_name, ".tasks") == 0){
                return 0;
            }
        }
        closedir(dir);
    }
    return -1;
}

static char* get_parent_dir(char* dir_name){
    if(strcmp(dir_name, "/") == 0){
        return NULL;
    }

    char* lsp = strrchr(dir_name, '/');

    if(lsp){
        int last_slash = lsp - dir_name;
        dir_name[last_slash] = '\0';
        return dir_name;
    }

    return NULL;

}

static char* find_root(){
    char * path = getcwd(0, 0);
    while(1){
        if(dir_contains_tasks(path) == 0){
            break;
        }
        path = get_parent_dir(path);
        if(path == NULL){
            break;
        }
    }
    return path;
}

int main(int argc, char *argv[])
{
    command_t cmd;
    command_init    (&cmd, argv[0], "0.0.1");
    command_option  (&cmd, "init", "","Initilize tasks in folder", init);
    command_option  (&cmd, "list", "","List all tasks added", list);
    command_option  (&cmd, "count", "","Counts number of tasks in .tasks", count);
    command_option  (&cmd, "add", "add <Task>", "Add task", add_task);
    command_option  (&cmd, "rm", "rm <Task>", "Remove task", remove_task);
    command_option  (&cmd, "clear", "", "Clear all tasks", clear);
    command_parse   (&cmd, argc, argv);

    command_free(&cmd);
    return 0;
}