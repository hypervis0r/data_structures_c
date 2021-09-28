CC := clang
CFLAGS := 

%.o: %.c
	$(CC) $(CFLAGS) -c $^

hash_table: hash_table.o linked_list.o
	$(CC) $(CFLAGS) -o $@ $^
	@echo "[+] Built hash_table"

linked_list: linked_list.o
	$(CC) $(CFLAGS) -o $@ $^
	@echo "[+] Built linked_list"

all: linked_list hash_table
	@echo "[+] Built all projects"

clean:
	rm *.o linked_list hash_table
	@echo "[+] Cleaned"
