void serial_put(const char c);
char serial_get();
void serial_crlf();
void serial_print(char * str, const uint8_t crlf);
void serial_hex_digit(uint8_t v);
void serial_binary(const uint8_t v);
