int buttonState = 0;
int trap = 0;
int pointer = 0;
int study = 1;
int relax = 0;
int studyAlarm = 0;
int relaxAlarm = 0;
int enjoyStudy = 0;

unsigned long int times[50] = {};
const unsigned long int studyTime = 1800000;
const unsigned long int relaxTime = 600000;

unsigned long int studyTimeDeveloped(unsigned long int timeDeveloped){
    unsigned long int studyTime = 0;
    // Sum all intervals studied
    for(int i = enjoyStudy; i < pointer; i+=2){
        studyTime += times[i+1] - times[i];
    }

    // Last interval studied
    studyTime += timeDeveloped - times[pointer];

    return studyTime;
}

void setup() {
    // Setup pins
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(2, INPUT);
}

void loop() {
    unsigned long currentTime = millis();
    buttonState = digitalRead(2);

    // Trap button system
    if(buttonState && !trap){
        // Remove tone
        noTone(12);
        trap++;
        
        // Every time pressed, add new time
        pointer++;
        times[pointer] = currentTime;

        if (studyAlarm) {
            relax++;
            studyAlarm--;
        }
        if (relaxAlarm){
            relax--;
            enjoyStudy = pointer;
            relaxAlarm--;
        }
        
        study = (study) ? 0 : 1;
    }
    if(!buttonState) trap = 0;

    // Tone if the user has already studied his time
    if(study && studyTimeDeveloped(currentTime) > studyTime){
        tone(12, 330);
        studyAlarm = 1;
    }

    // Tone if the user has already relaxed his time
    if(relax && currentTime - times[pointer] > relaxTime){
        tone(12, 330);
        relaxAlarm = 1;
    }

    // Pin 10: studying | Pin 11: relaxing
    if(study){
        digitalWrite(10, HIGH);
        digitalWrite(11, LOW);
    }else{
        digitalWrite(10, LOW);
        digitalWrite(11, HIGH);
    }
}
