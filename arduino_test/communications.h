// Contains defs for standard packet io protocol
// To read floating point numbers, you need to change your arduino configuration settings

// This just works, OK?

#define SEND(field, value) {\
  Serial.print("@@@@@");    \
  Serial.print(millis());   \
  Serial.print(":");        \
  Serial.print(#field);     \
  Serial.print(":");        \
  Serial.print(value);      \
  Serial.println("&&&&&");  \
  Serial.flush();           \
}

#define READ_BUFFER_SIZE 50
char _buffer[READ_BUFFER_SIZE];
char _data[READ_BUFFER_SIZE - 10];

#define CHECK_SERIAL_AVAIL            \
  if (!Serial.available()) {          \
    delay(100);                       \
    if (!Serial.available()) {        \
      Serial.println("READ timeout"); \
      goto L_ENDREAD;                 \
    }                                 \
  }                                   \

// Sorry about the gotos
#define BEGIN_READ                          \
  if (Serial.available()) {                 \
    char _c = '\0';                         \
    int _i;                                 \
    for (_i = 0; _c != '\n'; _i++) {        \
      if (_i == READ_BUFFER_SIZE) {         \
        Serial.println("READ buffer overflow");\
        while (Serial.available() && Serial.read() != '\n')\
          CHECK_SERIAL_AVAIL                \
        goto L_ENDREAD;                     \
      }                                     \
      CHECK_SERIAL_AVAIL                    \
      _c = Serial.read();                   \
      _buffer[_i] = _c;                     \
      if (_c == '\r') _i--;                 \
    }                                       \
    _buffer[_i] = '\0';                     \
    if (!sscanf(_buffer, "@@@@@:%[^&]&&&&&\n", _data)) {\
      Serial.println("READ packet error");  \
      goto L_ENDREAD;                       \
    }                                       \
    if (0);

#define READ_FIELD(field, spec)   \
  else if (sscanf(_data, #field":"spec, &field))

#define READ_FLAG(field)          \
  else if (!strcmp(_data, #field":"))

#define READ_DEFAULT(field_name, field) \
  else if (sscanf(_data, "%[^:]:%s", &field_name, &field))

#define END_READ } L_ENDREAD:;
