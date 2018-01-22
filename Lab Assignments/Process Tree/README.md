Process Tree
============

## To compile -   
* `gcc processtree.c -o processtree -lreadline`
## To Run -
1. Open a terminal and run `./processtree`
2. Open another terminal
3. Run `ps -efj` and note the PIDs and PPIDs of the processes you created.
4. Use `kill [PID]` to kill a process and then note how its children were assigned to **init**(PID=1).
