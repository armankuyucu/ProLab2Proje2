#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 32, 2); //IC2 adresi, 32 karakter, 2 satir

//Degiskenler

//Kasa bilgileri
int bes_Tl_adet = 10;
int on_Tl_adet = 10;
int yirmi_Tl_adet = 10;
int elli_Tl_adet = 10;
int yuz_Tl_adet = 10;
int kasadakiPara;

//Hizmet bilgileri

//Hizmet ID
const int kopuklemeID = 1;
const int yikamaID = 2;
const int kurulamaID = 2;
const int cilalamaID = 4;

//Hizmet Ad

const String kopuklemeAd = "kopukleme";
const String yikamaAd = "yikama";
const String kurulamaAd = "kuruluma";
const String cilalamaAd = "cilalama";

//Kalan Hizmet Ad

int kopuklemeKalan = 30;
int yikamaKalan = 50;
int kurulamaKalan = 100;
int cilalamaKalan = 20;

//Hizmet Fiyat

const int kopuklemeFiyat = 15;
const int yikamaFiyat = 10;
const int kurulamaFiyat = 5;
const int cilalamaFiyat = 50;

int eeAdress = 0; //EEPROM adress

//Buttonlar
const int BUTTON1 = 2;
const int BUTTON2 = 3;
const int BUTTON3 = 4;
const int BUTTON4 = 5;
const int BUTTON5 = 6;
const int BUTTON6 = 7;

//Ledler
int kirmiziLed = 8;
int yesilLed = 9;

//Buttonlarin degerleri
int val1 = 0; //Butonun degerini tutmak icin
int old_val1 = 0; //Butonun eski degerini tutmak icin
int val2 = 0;
int old_val2 = 0;
int val3 = 0;
int old_val3 = 0;
int val4 = 0;
int old_val4 = 0;
int val5 = 0;
int old_val5 = 0;
int val6 = 0;
int old_val6 = 0;

void setup() {
  Serial.begin(9600);
  //Lcd'yi aktiflestirme

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //yazilarin ekranin sol ust kosesinde baslamasi icin
  TerminaleYazdir();

  //Butonlarin input olarak setlenmesi
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(BUTTON5, INPUT);
  pinMode(BUTTON6, INPUT);

  //Ledlerin output olarak setlenmesi
  pinMode(kirmiziLed, OUTPUT);
  pinMode(yesilLed, OUTPUT);

  lcd.print("1.)5TL 2.)10TL 3.)20TL 4.)50TL");
  lcd.setCursor(0, 1);
  lcd.print("5.)100TL 6.)BITIS");

  //Kasadaki toplam parayi hesaplama
  kasadakiPara = yuz_Tl_adet * 100 + elli_Tl_adet * 50 + yirmi_Tl_adet * 20 + on_Tl_adet * 10 + bes_Tl_adet * 5;
}

//Butonlarin kac kere basildigini tutmak icin
int butonBasilmaSayisi1[6] = {0, 0, 0, 0, 0, 0}; //Para Yuklemesi
int butonBasilmaSayisi2[6] = {0, 0, 0, 0, 0, 0}; //Hizmet Secimi

//Programin hangi durumda oldugunu tutar
//0 para yukleme
//1 hizmet secimi
//2 programin sonlanmasi
int state = 0;

int yuklenenPara = 0;

// Rastgele sayi uretmek icin
unsigned long time;

void loop() {
  time = millis();
  if (state == 0)
    paraYukleme();
  else if (state == 1)
    hizmetSecimi();
  else if (state == 2)
    return;
}

void paraYukleme() {
  val1 = digitalRead(BUTTON1);

  if (val1 == HIGH && old_val1 == LOW) { //Su anki degeri ile eski degerini kiyaslayarak degisim var mi
    butonBasilmaSayisi1[0] += 1;        //diye kontrol ediyor.
    bes_Tl_adet++;
    TerminaleYazdir();
    delay(10);
  }
  old_val1 = val1;

  val2 = digitalRead(BUTTON2);
  if (val2 == HIGH && old_val2 == LOW) {
    butonBasilmaSayisi1[1] += 1;
    on_Tl_adet++;
    TerminaleYazdir();
    delay(10);
  }
  old_val2 = val2;

  val3 = digitalRead(BUTTON3);
  if (val3 == HIGH && old_val3 == LOW) {
    butonBasilmaSayisi1[2] += 1;
    yirmi_Tl_adet++;
    TerminaleYazdir();
    delay(10);
  }
  old_val3 = val3;

  val4 = digitalRead(BUTTON4);
  if (val4 == HIGH && old_val4 == LOW) {
    butonBasilmaSayisi1[3] += 1;
    elli_Tl_adet++;
    TerminaleYazdir();
    delay(10);
  }
  old_val4 = val4;

  val5 = digitalRead(BUTTON5);
  if (val5 == HIGH && old_val5 == LOW) {
    butonBasilmaSayisi1[4] += 1;
    yuz_Tl_adet++;
    TerminaleYazdir();
    delay(10);
  }
  old_val5 = val5;

  val6 = digitalRead(BUTTON6);
  if (val6 == HIGH && old_val6 == LOW) {
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < butonBasilmaSayisi1[i]; j++) {
        if (i == 0)
          yuklenenPara += 5;
        else if (i == 1)
          yuklenenPara += 10;
        else if (i == 2)
          yuklenenPara += 20;
        else if (i == 3)
          yuklenenPara += 50;
        else if (i == 4)
          yuklenenPara += 100;
      }

    }
    lcd.clear();
    lcd.print(String(yuklenenPara) + " TL PARA ATTINIZ");
    state = 1;
    delay(1000);
  }
  old_val6 = val6;
}

bool firstTime = true;
bool bankNotYeterliMi = true;
int num, para, tempPara;

void hizmetSecimi() {
  if (firstTime) {
    lcd.clear();
    lcd.print("1.)KOPUKLEME 2.)YIKAMA");
    lcd.setCursor(0, 1); //ikinci satira gecer
    lcd.print("3.)KURULAMA 4.)CILALAMA 6.)BITIS");
    lcd.setCursor(0, 0); //birinci satira doner
    firstTime = false;
    tempPara = yuklenenPara;
  }


  val1 = digitalRead(BUTTON1);

  if (val1 == HIGH && old_val1 == LOW && butonBasilmaSayisi2[0] <= kopuklemeKalan && tempPara >= kopuklemeFiyat) { //Su anki degeri ile eski degerini kiyaslayarak degisim var mi
    butonBasilmaSayisi2[0] += 1;                                                                                     //diye kontrol ediyor.
    kopuklemeKalan--;
    tempPara -= kopuklemeFiyat;
    TerminaleYazdir();
    delay(10);
  } else if (val1 == HIGH && old_val1 == LOW && butonBasilmaSayisi2[0] > kopuklemeKalan && tempPara >= kopuklemeFiyat) {
    lcd.print("KOPUKLEME SAYISI YETERSIZ");
    delay(10);
  } else if (val1 == HIGH && old_val1 == LOW && butonBasilmaSayisi2[0] <= kopuklemeKalan && tempPara < kopuklemeFiyat) {
    lcd.clear();
    lcd.print("PARANIZ BU ISLEM ICIN YETERSIZ");
    delay(1000);
    lcd.clear();
    lcd.print("1.)KOPUKLEME 2.)YIKAMA");
    lcd.setCursor(0, 1); //ikinci satira gecer
    lcd.print("3.)KURULAMA 4.)CILALAMA 6.)BITIS");
    lcd.setCursor(0, 0); //birinci satira doner
  }

  old_val1 = val1;

  val2 = digitalRead(BUTTON2);
  if (val2 == HIGH && old_val2 == LOW && butonBasilmaSayisi2[1] <= yikamaKalan && tempPara >= yikamaFiyat) {
    butonBasilmaSayisi2[1] += 1;
    yikamaKalan--;
    tempPara -= yikamaFiyat;
    TerminaleYazdir();
    delay(10);
  } else if (val2 == HIGH && old_val2 == LOW && butonBasilmaSayisi2[1] > yikamaKalan && tempPara >= yikamaFiyat) {
    lcd.print("YIKAMA SAYISI YETERSIZ");
    delay(10);
  } else if (val2 == HIGH && old_val2 == LOW && butonBasilmaSayisi2[1] <= yikamaKalan && tempPara < yikamaFiyat) {
    lcd.clear();
    lcd.print("PARANIZ BU ISLEM ICIN YETERSIZ");
    delay(1000);
    lcd.clear();
    lcd.print("1.)KOPUKLEME 2.)YIKAMA");
    lcd.setCursor(0, 1); //ikinci satira gecer
    lcd.print("3.)KURULAMA 4.)CILALAMA 6.)BITIS");
    lcd.setCursor(0, 0); //birinci satira doner
  }

  old_val2 = val2;

  val3 = digitalRead(BUTTON3);
  if (val3 == HIGH && old_val3 == LOW && butonBasilmaSayisi2[2] <= kurulamaKalan && tempPara >= kurulamaFiyat) {
    butonBasilmaSayisi2[2] += 1;
    kurulamaKalan--;
    tempPara -= kurulamaFiyat;
    TerminaleYazdir();
    delay(10);
  } else if (val3 == HIGH && old_val3 == LOW && butonBasilmaSayisi2[2] > kurulamaKalan && tempPara >= kurulamaFiyat) {
    lcd.print("KURULAMA SAYISI YETERSIZ");
    delay(10);
  } else if (val3 == HIGH && old_val3 == LOW && butonBasilmaSayisi2[2] <= kurulamaKalan && tempPara < kurulamaFiyat) {
    lcd.clear();
    lcd.print("PARANIZ BU ISLEM ICIN YETERSIZ");
    delay(1000);
    lcd.clear();
    lcd.print("1.)KOPUKLEME 2.)YIKAMA");
    lcd.setCursor(0, 1); //ikinci satira gecer
    lcd.print("3.)KURULAMA 4.)CILALAMA 6.)BITIS");
    lcd.setCursor(0, 0); //birinci satira doner
  }

  old_val3 = val3;

  val4 = digitalRead(BUTTON4);
  if (val4 == HIGH && old_val4 == LOW && butonBasilmaSayisi2[3] <= cilalamaKalan && tempPara >= cilalamaFiyat) {
    butonBasilmaSayisi2[3] += 1;
    cilalamaKalan--;
    tempPara -= cilalamaFiyat;
    TerminaleYazdir();
    delay(10);
  } else if (val4 == HIGH && old_val4 == LOW && butonBasilmaSayisi2[3] > cilalamaKalan && tempPara >= cilalamaFiyat) {
    lcd.print("CILALAMA SAYISI YETERSIZ");
    delay(10);
  } else if (val4 == HIGH && old_val4 == LOW && butonBasilmaSayisi2[3] <= cilalamaKalan && tempPara < cilalamaFiyat) {
    lcd.clear();
    lcd.print("PARANIZ BU ISLEM ICIN YETERSIZ");
    delay(1000);
    lcd.clear();
    lcd.print("1.)KOPUKLEME 2.)YIKAMA");
    lcd.setCursor(0, 1); //ikinci satira gecer
    lcd.print("3.)KURULAMA 4.)CILALAMA 6.)BITIS");
    lcd.setCursor(0, 0); //birinci satira doner
  }
  old_val4 = val4;

  val6 = digitalRead(BUTTON6);
  if (val6 == HIGH && old_val6 == LOW) {

    //Rastgele sayi uretme
    randomSeed(time);
    int num = random(1, 5);
    para = yuklenenPara;

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < butonBasilmaSayisi2[i]; j++) {
        if (i == 0) { //Kopukleme
          para -= kopuklemeFiyat;
        }
        else if (i == 1) { //Yikama
          para -= yikamaFiyat;
        }
        else if (i == 2) { //Kurulama
          para -= kurulamaFiyat;
        }
        else if (i == 3) { //Cilalama
          para -= cilalamaFiyat;
        }
      }
    }

    //Kirmizi Isik
    if (num == 2) {
      digitalWrite(kirmiziLed, HIGH);
      lcd.clear();
      lcd.print("PARA SIKISTI!");
      delay(1000);
      //Verilen Hizmet Sayisinin eski haline donusturulmesi
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < butonBasilmaSayisi2[i]; j++) {
          if (i == 0) { //Kopukleme
            kopuklemeKalan++;
          }
          else if (i == 1) { //Yikama
            yikamaKalan++;
          }
          else if (i == 2) { //Kurulama
            kurulamaKalan++;
          }
          else if (i == 3) { //Cilalama
            cilalamaKalan++;
          }
        }
      }

      TerminaleYazdir();
      //Verilen para ustunun tutuldugu degiskenler
      int verilen5TL = 0;
      int verilen10TL = 0;
      int verilen20TL = 0;
      int verilen50TL = 0;
      int verilen100TL = 0;

      //Kullanicinin yatidigi paraya gore verilecek para ustunun hesaplandigi donguler

      while (butonBasilmaSayisi1[0] > 0) {
        verilen5TL++;
        butonBasilmaSayisi1[0]--;
      }

      while (butonBasilmaSayisi1[1] > 0) {
        verilen10TL++;
        butonBasilmaSayisi1[1]--;
      }

      while (butonBasilmaSayisi1[2] > 0) {
        verilen20TL++;
        butonBasilmaSayisi1[2]--;
      }

      while (butonBasilmaSayisi1[3] > 0) {
        verilen50TL++;
        butonBasilmaSayisi1[3]--;
      }

      while (butonBasilmaSayisi1[4] > 0) {
        verilen100TL++;
        butonBasilmaSayisi1[4]--;
      }

      lcd.clear();
      lcd.print(String(verilen100TL) + " ADET 100TL " + String(verilen50TL) + " ADET 50TL");
      lcd.setCursor(0, 1);
      lcd.print(String(verilen20TL) + " ADET 20TL " + String(verilen10TL) + " ADET 10TL " + String(verilen5TL) + " ADET 5TL");
      lcd.setCursor(0, 0);
    }

    //Yesil Isik
    else {
      digitalWrite(yesilLed, HIGH);
      //Verilen para ustunun tutuldugu degiskenler
      int verilen5TL = 0;
      int verilen10TL = 0;
      int verilen20TL = 0;
      int verilen50TL = 0;
      int verilen100TL = 0;

      kasadakiPara = yuz_Tl_adet * 100 + elli_Tl_adet * 50 + yirmi_Tl_adet * 20 + on_Tl_adet * 10 + bes_Tl_adet * 5;
      
      //Verilecek para ustunun hesaplandigi donguler
      while (para >= 100) {
        if (yuz_Tl_adet > 0) {
          para -= 100;
          yuz_Tl_adet--;
          verilen100TL++;
        }
        else if (yuz_Tl_adet <= 0 && (kasadakiPara < para) ) {
          bankNotYeterliMi = false;
          break;
        }
        else{
          break;
        }
      }

      while (para >= 50) {
        if (elli_Tl_adet > 0) {
          para -= 50;
          elli_Tl_adet--;
          verilen50TL++;
        }
        else if (elli_Tl_adet <= 0 && (elli_Tl_adet * 50 + yirmi_Tl_adet * 20 + on_Tl_adet * 10 + bes_Tl_adet * 5 < para)) {
          bankNotYeterliMi =  false;
          break;
        }
        else{
          break;
        }
      }

      while (para >= 20) {
        if (yirmi_Tl_adet > 0) {
          para -= 20;
          yirmi_Tl_adet--;
          verilen20TL++;
        } else if (yirmi_Tl_adet <= 0 && (yirmi_Tl_adet * 20 + on_Tl_adet * 10 + bes_Tl_adet * 5 < para)) {
          bankNotYeterliMi = false;
          break;
        }
        else{
          break;
        }
      }
      while (para >= 10) {
        if (on_Tl_adet > 0) {
          para -= 10;
          on_Tl_adet--;
          verilen10TL++;
        } else if (on_Tl_adet <= 0 && (on_Tl_adet * 10 + bes_Tl_adet * 5 < para)) {
          bankNotYeterliMi = false;
          break;
        }
        else{
          break;
        }

      }
      while (para >= 5) {
        if (bes_Tl_adet > 0) {
          para -= 5;
          bes_Tl_adet--;
          verilen5TL++;
        } 
        else if (bes_Tl_adet <= 0) {
          bankNotYeterliMi = false;
          break;
        } 
        else{
          break;
        }
      }

      if (bankNotYeterliMi) {
        lcd.clear();
        lcd.print(String(verilen100TL) + " ADET 100TL " + String(verilen50TL) + " ADET 50TL");
        lcd.setCursor(0, 1);
        lcd.print(String(verilen20TL) + " ADET 20TL " + String(verilen10TL) + " ADET 10TL " + String(verilen5TL) + " ADET 5TL");
        TerminaleYazdir();
      } else {
        lcd.clear();
        lcd.print("YETERLI BANKNOT YOK!");
        TerminaleYazdir();
      }
    }

    state = 2;
  }

  old_val6 = val6;

}

void TerminaleYazdir() {
  for (int i = 0; i < 20; i++) {
    Serial.println();
  }
  Serial.println("5TL Adet: " + String(bes_Tl_adet));
  Serial.println("10TL Adet: " + String(on_Tl_adet));
  Serial.println("20TL Adet: " + String(yirmi_Tl_adet));
  Serial.println("50TL Adet: " + String(elli_Tl_adet));
  Serial.println("100TL Adet: " + String(yuz_Tl_adet));
  Serial.println("---------------------------");
  Serial.println("Kopukleme," + String(kopuklemeKalan) + " Adet, " + String(kopuklemeFiyat) + "TL");
  Serial.println("Yikama," + String(yikamaKalan) + " Adet, " + String(yikamaFiyat) + "TL");
  Serial.println("Kurulama," + String(kurulamaKalan) + " Adet, " + String(kurulamaFiyat) + "TL");
  Serial.println("Cilalama," + String(cilalamaKalan) + " Adet, " + String(cilalamaFiyat) + "TL");
}
