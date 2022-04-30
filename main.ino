// #define DEBUG

enum Pin{
    buzzer=LED_BUILTIN,
    tombol=3,
    echo=8,trig,
    kenop=A5
};

unsigned long maxDistancePulseLength = 19533;
constexpr unsigned long closestDistancePulseLength = 594;
constexpr unsigned long analogInputOffset = 14;
constexpr unsigned long maxAnalogInput = 1023 - analogInputOffset;
unsigned long targetDistancePulseLength;

void setup()
{
    pinMode(tombol,INPUT_PULLUP);
    pinMode(echo,INPUT_PULLUP);
    pinMode(kenop,INPUT_PULLUP);
    pinMode(buzzer,OUTPUT);
    pinMode(trig,OUTPUT);

    #ifdef DEBUG
    Serial.begin(9600);
    #endif
}

void loop()
{
    #ifdef DEBUG
    Serial.print((analogRead(kenop)-analogInputOffset)/maxAnalogInput);
    Serial.print('\t');
    Serial.print(targetDistancePulseLength);
    Serial.print('\t');
    Serial.println(maxDistancePulseLength);
    #endif

    if(digitalRead(tombol) == LOW) updateMaxDistance();
    if((checkDistance()-closestDistancePulseLength) * maxAnalogInput <= 
            (analogRead(kenop)-analogInputOffset) * (maxDistancePulseLength-closestDistancePulseLength))
                triggeredBeep();
    delay(500);
}

void beep(unsigned long halfPeriod){
    digitalWrite(buzzer, HIGH);
    delay(halfPeriod);
    digitalWrite(buzzer, LOW);
    delay(halfPeriod);
}

void updateMaxDistance(){
    maxDistancePulseLength = checkDistance();
    beep(125); beep(125);
}

void triggeredBeep(){
    beep(50); beep(50); beep(50); beep(50);
}

unsigned long checkDistance(){
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    return pulseIn(echo, HIGH);
}