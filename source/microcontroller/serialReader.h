const byte maxLen = 255;
const char endMarker = '\n';

char receivedChars[maxLen];
byte ndx = 0;

char *getSerialMessage() {
    char rc;
   
    while (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx++] = rc;
            
            if (ndx >= maxLen) {
                ndx = maxLen - 1;
            }
        } else {
            receivedChars[ndx] = '\0';
            ndx = 0;
            return receivedChars;
        }
    }
    
    return NULL;
}
