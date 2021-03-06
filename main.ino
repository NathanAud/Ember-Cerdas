// #define DEBUG
#define DITEKAN LOW

enum Pin{
    buzzer=2,tombol,
    trig=8,echo,
    kenop=A5
};

unsigned long maxDistancePulseLength = 0;
constexpr unsigned long closestDistancePulseLength = 416;
constexpr unsigned long analogInputOffset = 14;
constexpr unsigned long maxAnalogInput = 1023 - analogInputOffset;

void setup()
{
    pinMode(tombol,INPUT_PULLUP);
    pinMode(echo,INPUT_PULLUP);
    pinMode(kenop,INPUT_PULLUP);
    pinMode(buzzer,OUTPUT);
    pinMode(trig,OUTPUT);

    while(digitalRead(tombol) != DITEKAN);
    updateMaxDistance();

    #ifdef DEBUG
    Serial.begin(9600);
    #endif
}

void loop()
{
    #ifdef DEBUG
    Serial.print((double)(analogRead(kenop)-analogInputOffset)/maxAnalogInput);
    Serial.print('\t');
    Serial.print((((double)(analogRead(kenop)-analogInputOffset)/maxAnalogInput) * (maxDistancePulseLength-closestDistancePulseLength))+closestDistancePulseLength); //targetDistancePulseLength
    Serial.print('\t');
    Serial.print(checkDistance());
    Serial.print('\t');
    Serial.println(maxDistancePulseLength);
    #endif

    if(digitalRead(tombol) == DITEKAN) updateMaxDistance();
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
    auto pulse = pulseIn(echo, HIGH);
    return max(pulse,closestDistancePulseLength);
}