#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin = 2; 
const int buzzerPin = 4; 
int score = 0; 
int jumpHeight = 0;  
bool isJumping = false; 

// dino
byte dino[8] = {
  0b00011,
  0b00010,
  0b00010,
  0b00010,
  0b11111,
  0b01111,
  0b01001,
  0b01001,
};

// kaktus
byte cactus[8] = {
  0b00100,
  0b00101,
  0b00101,
  0b10111,
  0b10100,
  0b11100,
  0b00100,
  0b00100,
};

bool gameOver = false;  

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(buzzerPin, OUTPUT);      
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, dino);   
  lcd.createChar(1, cactus); 

  Serial.begin(9600); 
}

void loop() {
  // game over
  if (gameOver) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Game Over");
    lcd.setCursor(4, 1);
    lcd.print("Score: ");
    lcd.print(score);
    
    // Tunggu hingga tombol ditekan untuk memulai ulang
    while (digitalRead(buttonPin) == HIGH) {
      // Menunggu tombol ditekan
    }
    
    // Reset game dan skor
    score = 0;
    gameOver = false;
    jumpHeight = 0; 
    delay(500); 
  } else {
    // loop kaktus bergerak
    for (int position = 15; position >= 0; position--) {
      lcd.clear(); 

      // Cek jika tombol ditekan untuk membuat dino melompat
      if (digitalRead(buttonPin) == LOW && !isJumping) {
        isJumping = true; 
        tone(buzzerPin, 1000, 200); 
      }

      // Logika lompat max 3
      if (isJumping) {
        if (jumpHeight < 3) {
          lcd.setCursor(3, 0);  
          lcd.write((byte)0);
          jumpHeight++;    
        } else {
          // Setelah mencapai puncak, turunkan dino ke bawah
          lcd.setCursor(3, 1);   
          lcd.write((byte)0);    
          jumpHeight = 0;        
          isJumping = false;     
          noTone(buzzerPin);     
        }
      } else {
        lcd.setCursor(3, 1);   
        lcd.write((byte)0);    
      }

      // Jika posisi kaktus mencapai 4, tambahkan skor
      if(position == 4) {
        score += 1;
      }

      // Tampilkan skor di sudut kanan atas
      lcd.setCursor(15, 0);         
      lcd.print(score);    

      // Tampilkan kaktus di baris kedua
      lcd.setCursor(position, 1);    
      lcd.write((byte)1);            

      // Cek tabrakan (game over) jika posisi kaktus dan dino bertemu
      if (position == 3 && !isJumping) {
        gameOver = true;
        break; 
      }

      delay(200); 
    }
  }
}
