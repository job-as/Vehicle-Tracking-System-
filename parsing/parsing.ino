void setup() {

	Serial.begin(9600);
	delay(500);
	
}

char inString[];
char *strings[10];
char *ptr = NULL;

void loop() {

	
    byte index = 0;

    ptr = strtok(inString, "\""); 
   
    while(ptr != NULL) {
 
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, "\"");

    }

    for(int i=0; i<index; i++) {

    	Serial.println(strings[10]);
        
    }
	
}