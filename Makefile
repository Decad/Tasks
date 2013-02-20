tasks: tasks.c lib/commander.c
	$(CC) $^ -std=c99 -o $@

clean:
	rm -f tasks

.PHONY: clean