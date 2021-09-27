CC := clang
CFLAGS := 

%.o: %.c
	$(CC) $(CFLAGS) -c $^

linked_list: linked_list.o
	$(CC) $(CFLAGS) -o $@ $^
	@echo "[+] Built linked_list"

all: linked_list
	@echo "[+] Built all projects"

clean:
	rm *.o linked_list
	@echo "[+] Cleaned"
