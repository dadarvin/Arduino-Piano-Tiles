#include <Wire.h>                  // Include Wire library for I2c Devices
#include <LiquidCrystal_I2C.h>     // Include LiquidCrystal_I2C library 
#include <avr/sleep.h> // Include sleep
#define interruptPin 2 // define pin for interrupt
#define piezoPin 9 //define pin for piezo

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows

// Mendefinsikan pin yang digunakan
int ledPin[5] = {8, 10, 11, 12, 13}; 
int sensorPin[5] = {3, 4, 5, 6, 7};
int menuSelect[3] = {A1, A2, A3};

//Mendefinisikan variabel global yang digunakan
int long ranDelay = 0;
int pinrand = 0 ;
int sensorValue = 0;
float realTime;
int i,j,score,waktu,temp,temprand, flag = 0;

// -- Variabel untuk mode PianoTiles --
//Mendefinisikan musik Piezo untuk mode PianoTiles
float imperial[] = {
  //iconic theme
  440,440,440,
  349,523,440,349,523,440,
  659,659,659,
  698,523,415,349,523,440,

  //mid
  880,440,440,880,
  830,784,740,698,740,
  455,622,587,
  554,523,466,523,

  349,415,349,440,523,440,523,659,

  880,440,440,880,
  830,784,740,698,740,
  455,622,587,
  554,523,466,523,

  349,415,349,523,440,349,523,440
  
};
//Mendefinisikan delay Piezeo untuk mode Pianotiles
int ImperialDelay[] = {
  500, 500, 500, 
  350,150,500,350,150,650,
  500, 500, 500, 
  350,150,500,350,150,650,

  500,300,150,500, 
  325,175,150,150,250,
  250,500,325, 
  175,150,150,250, 

  250,500,350,150,500,375,150,650,
  
  500,300,150,500, 
  325,175,150,150,250,
  250,500,325, 
  175,150,150,250,

  250,500,375,150,500,375,150,650
};
// ---------------------------------------------

// -- Variabel untuk mode Arcade --

//Mendefinisikan musik Piezo untuk mode Arcade
float melody[] = {
  329.63, 311.13, 329.63, 311.13,
  329.63, 246.94, 293.66, 261.63,
  220,
  146.83, 174.61, 220,
  246.94,
  174.61, 233.08, 246.94,
  261.63,
  
  329.63, 311.13, 329.63, 311.13,
  329.63, 246.94, 293.66, 261.63,
  220,
  146.83, 174.61, 220,
  246.94,
  174.61, 261.63, 246.94,
  220,
  246.94, 261.63, 293.66,
  329.63,
  196, 349.23, 329.23, 
  293.63,
  164.81, 329.63, 293.63,
  261.63,
  146.83, 293.63, 261.63,
  246.94,
    
  329.63, 311.13, 329.63, 311.13,
  329.63, 246.94, 293.66, 261.63,
  220,
  146.83, 174.61, 220,
  246.94,
  174.61, 233.08, 246.94,
  261.63,
  
  329.63, 311.13, 329.63, 311.13,
  329.63, 246.94, 293.66, 261.63,
  220,
  146.83, 174.61, 220,
  246.94,
  174.61, 261.63, 246.94,
  220
};
//Mendefinisikan delay musik untuk mode Arcade
int durasi[] = {
  300, 300, 300, 300,
  300, 300, 300, 300,
  900,
  300, 300, 300,
  900,
  300, 300, 300,
  900,
  
  300, 300, 300, 300,
  300, 300, 300, 300,
  900,
  300, 300, 300,
  900,
  300, 300, 300,
  900,
  300, 300, 300,
  900,
  300, 300, 300, 
  900,
  300, 300, 300,
  900,
  300, 300, 300,
  900,
    
  300, 300, 300, 300,
  300, 300, 300, 300,
  900,
  300, 300, 300,
  900,
  300, 300, 300,
  900,
  
  300, 300, 300, 300,
  300, 300, 300, 300,
  900,
  300, 300, 300,
  900,
  300, 300, 300,
  900
};
// ------------------------------------------

void setup()
{
  Serial.begin(9600);
  //Mengatur mode pin untuk Piezo
  pinMode(piezoPin, OUTPUT);
  //mengatur mode pin untuk LED
  pinMode(ledPin[0], OUTPUT);
  pinMode(ledPin[1], OUTPUT);
  pinMode(ledPin[2], OUTPUT);
  pinMode(ledPin[3], OUTPUT);
  pinMode(ledPin[4], OUTPUT);
  //Mengatur mode pin untuk tombol menu
  pinMode(menuSelect[0], INPUT);
  pinMode(menuSelect[1], INPUT);
  pinMode(menuSelect[2], INPUT);
  //Mengatur mode pin untuk tombol piano
  pinMode(sensorPin[0], INPUT);
  pinMode(sensorPin[1], INPUT);
  pinMode(sensorPin[2], INPUT);
  pinMode(sensorPin[3], INPUT);
  pinMode(sensorPin[4], INPUT);
  //Mengatur mode pin untuk tombol interrupt
  pinMode(interruptPin,INPUT_PULLUP);
  //Menginisialisasi LCD dan menampilkan sambutan
  lcd.init();                        // Initialize I2C LCD module
  lcd.backlight();                   // Turn backlight ON
  lcd.setCursor(3, 0);               // Go to column 0, row 0
  lcd.print("Welcome To");
  lcd.setCursor(1, 1);               // Go to column 0, row 1
  lcd.print("- Fun in One -");
  delay(2000);
  printMenu();
}

int goSleep = 0;

void loop(){
  //Memilih Permainan Reflex
  delay (1000);
  if(digitalRead(menuSelect[0]) == 1 ){
    pianoTiles();
  } else if(digitalRead(menuSelect[1]) == 1 ) {
    arcade();
  } else if(digitalRead(menuSelect[2]) == 1 ){
    reflex();
  } 

  //Sleep jika tidak memilih menu selama 5 detik
  if(goSleep > 5){
    gonnaSleep();
    goSleep = 0;
  }
  goSleep++;
}

//Fungsi unutk mengatur arduino pada sleep mode
void gonnaSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Mengatur Sleep mode
  sleep_enable(); //Melakukan enable pada sleep bit
  lcd.clear();
  Serial.println("Zzz");
  attachInterrupt(0, wakeUp, LOW);//Menjalankan WakeUp ketika LOW
  delay(1000);
  sleep_cpu();//Mengatur arduino pada mode Sleep ketika Dawn
  printMenu();
}

//Fungsi untuk melakukan interrupt pada arduino
void wakeUp(){
   sleep_disable(); //disable Sleep pada 
   Serial.println("Hi! I'm Awake");
   
   detachInterrupt(0); //remove interrupt
}

//Menampilkan menu utama pemilihan permainan
void printMenu(){
  lcd.clear();
  lcd.init();
  lcd.setCursor(0, 0);               // Go to column 0, row 0
  lcd.print("1.Piano 2.Arcade");
  lcd.setCursor(3, 1);               // Go to column 0, row 1
  lcd.print("3. Reflex");
}

void receiveInput(){
  while (true) {
    if(digitalRead(sensorPin[0]) == 1 || digitalRead(sensorPin[1]) == 1 || digitalRead(sensorPin[2]) == 1 || digitalRead(sensorPin[3]) == 1 || digitalRead(sensorPin[4]) == 1){
      break;
    }
  }
}

// -- Permainan Mode Piano Tiles --
void pianoTiles(){

  //Menampilkan Mode yang dipilih
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("--PIANO TILES--");
  delay(1000);
  lcd.clear();
  lcd.print("Press to start");
  delay(1000);
  receiveInput();

  score = 0; //Menginisialisasi Score permainan

  //Mempersiapkan sebelum permainan dimulai

  lcd.print("");
  lcd.print("");
  lcd.print("");
  lcd.clear();
  lcd.print("PLAYING: ");
  lcd.setCursor(0, 1);
  lcd.print("IMPERIAL MARCH");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Get Ready!");
  delay(1000);
  lcd.clear();
  lcd.print("Get Set!");
  delay(1000);
  lcd.clear();
  lcd.print("Go!");

  //Memulai permainan sebanyak note yang diinisialisasi
  for(j=0;j<66;j++){
    //Menyalakan salah satu LED secara acak 
    pinrand = random(0,5);
    temp = 0;
    digitalWrite(ledPin[pinrand], HIGH); 
    
    //Memainkan nada piezo sesuai dengan indeks j
    tone(piezoPin,imperial[j]);
    
    //Menambah score bila input button sesuai dengan posisi LED
    for(i=0;i<(ImperialDelay[j]/6);i++){
      delay(1);
      if(digitalRead(sensorPin[pinrand]) == 1 && temp == 0){
        score++;
        temp = 1;
    }

    //Menampilkan score yang diperoleh
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);
  }
  //Mematikan Piezo dan LED
  noTone(piezoPin);
  digitalWrite(ledPin[pinrand], LOW);
  delay(ImperialDelay[j]/2);  //Delay antar note
  }
  delay(2000);
  lcd.clear();
  lcd.print("Done!");

  //Menerima input untuk kembali ke menu
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Press to cont");
  receiveInput();
  delay(500);
  printMenu();
  goSleep = 0;
}
//----------------------------------------------------------------

// -- Permainan Mode Arcade --
void arcade(){
  //Menampilkan Mode yang dipilih
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("--ARCADE--");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.print("Press to start");
  receiveInput();

  //Menginisialisasi nilai score awal
  score = 0;
  
  lcd.print("");
  lcd.print("");
  lcd.print("");
  lcd.clear();
  lcd.print("PLAYING:");
  lcd.setCursor(0, 1);
  lcd.print("FUR ELISE");
  delay(2000);
  lcd.clear();
  lcd.print("Get Ready!");
  delay(1000);
  lcd.clear();
  lcd.print("Get Set!");
  delay(1000);
  lcd.clear();
  lcd.print("Go!");

  //Melakukan looping selama tombol yang ditekan benar
  j = 0;
  while(true){
    if(j == 84){
      j = 0;
    }
    //Menyalakan LED secara random untuk ditekan
    pinrand = random(0,5);
    temp = 0;
    digitalWrite(ledPin[pinrand], HIGH);
    //Memainkan piezo
    tone(piezoPin,melody[j]);
    //Looping untuk membaca input yang diberikan
    for(i=0;i<(durasi[j]/5);i++){
      delay(1);
      //Jika tombol berhasil ditekan tambahkan score dan temp menjadi 1
      if(digitalRead(sensorPin[pinrand]) == 1 && temp == 0){
        score++;
        temp = 1;
    }
    
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);
    }
    //Mematikan piezo dan LED
    noTone(piezoPin);
    digitalWrite(ledPin[pinrand], LOW);
    delay(durasi[j]/2);
    j++;
    
    //Jika tombol tidak ditekan maka permainan selesai
    if(temp == 0){
     
      break;
    }
  }
  //Permainan selesai
  delay(3000);
  lcd.clear();
  lcd.print("Done!");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Press to cont");

  receiveInput();
  delay(500);
  printMenu();
  goSleep = 0;
}

// -- Permainan Mode Refleks --
void reflex() {
  //Menampilkan Mode yang dipilih
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("--REFLEX TEST--");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.print("Press to start");
  receiveInput();

  lcd.print("");
  lcd.print("");
  lcd.print("");
  lcd.clear();
  lcd.print("Get Ready!");
  delay(1000);
  lcd.clear();
  lcd.print("Get Set!");
  delay(1000);

  //Memunculkan lampu secara random
  ranDelay = random(5000);
  delay(ranDelay);
  lcd.clear();
  //Menyalakan lampu secara acak
  pinrand = random(0,5);
  lcd.print("Go!");
  //Menginisialisasi perhitungan waktu
  realTime = millis();
  digitalWrite(ledPin[pinrand], HIGH);
  tone(piezoPin,880,200);
  //Menunggu input dari pemain
  while (digitalRead(sensorPin[pinrand]) == 0)
  {
    //wait here
  }

  //Jika sudah ditekan akan ditampilkan waktu refleks
  digitalWrite(ledPin[pinrand], LOW); 
  lcd.setCursor(0, 1);
  lcd.print("Result:");
  realTime = millis() - realTime; //Memperoleh selisih waktu awal dan akhir
  lcd.setCursor(8, 1);
  
  //Menampilkan waktu dalam ms
  lcd.print(realTime);
  lcd.print("ms");
  delay(1000);
  
  //Mengembalikan ke menu pemilihan menu
  lcd.setCursor(0, 0);
  lcd.print("Press to cont");
  receiveInput();
  delay(500);
  printMenu();
  goSleep = 0;
}
