# Tasks

A small C program for keeping track of tasks and todo's. Tasks works similarily to git meaning that you initilise it inside a folder and tasks creates a .task file inside that folder to store the tasks in. This file is then used for all sub folders of the directory tasks was initilised in.

## Install

To install tasks compile the program using `make` then move the binary `tasks` into your path.

## Usage

### tasks init

Before you can add any tasks you first have to initialise `tasks` with the command `tasks init`. Doing this creates the file `.tasks`,
in which all of your tasks get stored in a simple file format of 1 task per line. Once tasks has been initilised inside a folder any further commands from this folder or any sub folder will effect this `.tasks` file.
`tasks` determins which `.tasks` file to edit but climbing up the current working directorys path until it encounters a `.tasks` file. This means that you can have sub `.tasks` files.

### tasks add <arg>

Add a task to `tasks`, eg:

`tasks add "Clean up these files and implement that new feature"`

### tasks list

List all tasks currently added.

### tasks count

Gives a count of the number of tasks in the current file.

### tasks rm <arg>

Remove a completed task where arg is the number of the task as seen from `tasks list`

