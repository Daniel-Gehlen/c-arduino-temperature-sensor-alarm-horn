int temp_sens = 0;
int ledState = 0; // 0: verde, 1: amarelo, 2: vermelho

// Melody
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C5  523  // Change NOTE_C4 to NOTE_C5 to avoid conflict

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(0, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // temperature sensor
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  temp_sens = (-40 + 0.488155 * (analogRead(A0) - 20));

  if (temp_sens >= 30) {
    digitalWrite(0, HIGH); // Aciona o motor do ventilador
    if (temp_sens > 50) {
      // Condição de emergência
      changeLEDState(); // Altera o estado do LED
      if (ledState == 2) {
        playMelody(); // Toca a melodia quando o LED está vermelho
      }
      delay(5000); // Aguarda 5 segundos
    } else {
      digitalWrite(5, LOW); // Desliga o LED vermelho
      noTone(6); // Desliga a buzina
    }
  } else {
    // Temperatura abaixo de 30°C, desliga o motor, o LED e a buzina
    digitalWrite(0, LOW);
    digitalWrite(5, LOW);
    noTone(6); // Desliga a buzina
  }

  // Passa pelos estágios de LED (verde, amarelo, vermelho) com 3 segundos para cada estágio
  changeLEDState();
  delay(3000);
}

void changeLEDState() {
  // Altera o estado do LED (verde, amarelo, vermelho)
  if (ledState == 0) {
    digitalWrite(3, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    ledState = 1; // Próximo estado: amarelo
  } else if (ledState == 1) {
    digitalWrite(3, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    ledState = 2; // Próximo estado: vermelho
  } else if (ledState == 2) {
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    ledState = 0; // Próximo estado: verde
  }
}

void playMelody() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(6, melody[thisNote], noteDuration); // Use o pino 6 para a buzina

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(6);
  }
}
