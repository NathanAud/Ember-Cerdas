// #define DEBUG

enum Pin{
    buzzer=LED_BUILTIN,
    tombol=3,
    echo=8,trig,
    kenop=A5
};

double maxDistancePulseLength = 19533;
constexpr double closestDistancePulseLength = 594;
constexpr double analogInputOffset = 14;
constexpr double maxAnalogInput = 1023 - analogInputOffset;
double targetDistancePulseLength;

void setup()
{
    Pin inputPin[]{tombol,echo,kenop};
    for(Pin pin:inputPin) pinMode(pin, INPUT_PULLUP);
    Pin outputPin[]{buzzer,trig};
    for(Pin pin:outputPin) pinMode(pin, OUTPUT);

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
    targetDistancePulseLength = (((analogRead(kenop)-analogInputOffset)/maxAnalogInput) * (maxDistancePulseLength-closestDistancePulseLength)) + closestDistancePulseLength;
    if(checkDistance() <= targetDistancePulseLength) triggeredBeep();
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

double checkDistance(){
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    return pulseIn(echo, HIGH);
}