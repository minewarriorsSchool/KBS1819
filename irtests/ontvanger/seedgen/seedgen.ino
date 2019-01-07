

#include <Arduino.h>

int main(){

  srand(200);
  Serial.begin(9600);
  
	while(1)
	{
    Serial.println(rand());
    _delay_ms(1000);
		
	}
	
	
	return 0;
}
