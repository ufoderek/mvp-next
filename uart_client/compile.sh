rm uart_client
gcc -O2 -march=native -fomit-frame-pointer uart_client.c -lpthread -o uart_client
