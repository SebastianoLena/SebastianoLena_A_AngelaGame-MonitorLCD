#define butt1 11
#define butt2 12
#define butt3 13
#include <LiquidCrystal.h> // Libreria per gestire il Display LCD
int risDaRaggiungere; //Indica il risultato a cui bisogna arrivare per vincere
int mossaGiocatore; //Indica il numero che ha inserito il giocatore durante la sua mossa
int mossaAppoggio; //Questa variabile verrà utilizzata per impedire di inserire i numeri che equivalgono alla faccia opposta del dado es: 1-6 2-5
int numeroCorrente; //Indica il numero corrente a cui si è arrivati
int turno; // turno = 0 (giocatore 1) | turno = 1 (giocatore 2)
int turnoIniziale; //Indica il turno con cui è cominciato l'ultima partita effettuata
bool finito;
int cont; 
int numTurni; //Variabile che terrà conto del numero dei turni della partita es: Gioc.2 fa la sua mossa e il turno aumenta di 1
String istruzioni; //Stringa che andrà a visualizzare nel monitor seriale le informazioni necessarie
bool vero; // variabile che verrà utilizzata per i controlli
int val1; // valore bott1
int val2; // valore bott2
int val3; // valoew bott3

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

byte smiley[8] = //crea un immagine sul monitor ldc
{
  B00000,
  B00000,
  B01010,
  B10101,
  B00000,
  B10001,
  B01110,
  B00000,
};

byte apple0[8] = //crea un immagine sul monitor ldc
{
  B00000,
  B00000,
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte apple1[8] = //crea un immagine sul monitor ldc
{
  B00100,
  B01000,
  B00111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11110,
};

byte apple2[8] = //crea un immagine sul monitor ldc
{
  B00000,
  B00000,
  B00000,
  B10000,
  B11000,
  B00000,
  B00000,
  B00000,
};

byte apple3[8] = //crea un immagine sul monitor ldc
{
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B01111,
  B00111,
  B00111,
};

byte apple4[8] = //crea un immagine sul monitor ldc
{
  B11100,
  B11110,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B00111,
};

byte apple5[8] = //crea un immagine sul monitor ldc
{
  B00000,
  B00000,
  B00000,
  B11000,
  B11000,
  B10000,
  B00000,
  B00000,
};
void setup()
{
  // put your setup code here, to run once:
  pinMode(butt1, INPUT);
  pinMode(butt2, INPUT);
  pinMode(butt3, INPUT);   
  lcd.begin(16, 2); 
  
  SchermataIniziale();
  lcd.clear(); // Pulisce l'LCD e pone il cursore all'inizio
  risDaRaggiungere = 30;
  numeroCorrente = 0;
  turno = 0;
  turnoIniziale = 0;
  finito = false;
  cont = 0;
  mossaGiocatore = 0;
  mossaAppoggio = -1;
  numTurni = 0;
  istruzioni = "";
  vero = 0;
  val1 = 0;
  val2 = 0;
  val3 = 0;
  lcd.createChar(0, apple0);
  lcd.createChar(1, apple1);
  lcd.createChar(2, apple2);
  lcd.createChar(3, apple3);
  lcd.createChar(4, apple4);
  lcd.createChar(5, apple5);
  lcd.createChar(6, smiley);
}

void loop()
{
  // put your main code here, to run repeatedly:
  ScegliRisDaRaggiungere();
  SchermataMossa();
  LetturaMossa();
}
 
void ScegliRisDaRaggiungere()
{
  if (numTurni == 0)
  {
    lcd.clear();
    while(val2 == 0)
    {
        ScriviRisultato();
        lcd.setCursor(14,1);
        AggiungiOSottraiRisulatatoDaRaggiungere();
        delay(100);
        lcd.print(risDaRaggiungere);
        LeggiValBott();
    }
    val2 = 0;
    CambiaTurnoIniziale();    
    delay(1000);
  }
}

void AggiungiOSottraiRisulatatoDaRaggiungere()
{
  if(val1 == HIGH)
  {
    if (risDaRaggiungere > 30)
    {
      risDaRaggiungere--;
    }    
  }
  if(val3 == HIGH)
  {
    if (risDaRaggiungere < 99)
    {
      risDaRaggiungere++;
    }
  }
}

void LeggiValBott()
{
  val1 = digitalRead(butt1);
  val2 = digitalRead(butt2);
  val3 = digitalRead(butt3);
}

void SchermataMossa()
{
  if (turno == 0)
  {
    istruzioni = "1: ";
  }
  else
  {
   istruzioni = "2: ";
  }
  LeggiValBott();
  ScriviMossaGiocatore();
  val2 = 0;
}

void LetturaMossa() //Metodo che consente la lettura dell'input effettuato dal giocatore
{
  while (val2 == 0)
  {

    InserisciMossa();
    delay(150);
    LeggiValBott();   
    lcd.setCursor(3,1);
    VisualizzaMossa();
  }
  if (mossaGiocatore == mossaAppoggio || mossaAppoggio == (7 - mossaGiocatore))
  {
    MossaNonValida();
    delay(1000);
    val2 = 0;
    delay(150);
    mossaGiocatore = 1;
    SchermataMossa();
    LetturaMossa();      
  }
  else
  {
    mossaAppoggio = mossaGiocatore;      
    AggiungiMossaAlRisultato();
    val2 = 0;
    delay(300);
  }
}

void AggiungiMossaAlRisultato() //Aggiunge la mossa appena fatta al risultato corrente
{
  numeroCorrente = numeroCorrente + mossaGiocatore;
  DeterminaChiVince();
  PassaTurno();
}

void InserisciMossa()
{
  if (val1 == 1 && mossaGiocatore > 0 && numTurni == 0)
  {
    mossaGiocatore--;
  }
  else if (val1 == 1 && mossaGiocatore > 1)
  {
    mossaGiocatore--;
  }
  if (val3 == 1 && mossaGiocatore < 6)
  {
    mossaGiocatore++;
  }
}

void PassaTurno()
{
  if (turno == 0)
  {
    turno = 1;
  }
  else
  {
    turno = 0;
  }
  numTurni++;
}

void CambiaTurnoIniziale() //Questo metodo decide chi dovrà fare la prima mossa nella prossima partita in base a chi ha eseguito per primo la sua mossa in quella precedente
{
  if (turnoIniziale == 0)
  {
    turnoIniziale = 1;
    CominciaGioc1();
    turno = 0;
  }
  else
  {
    turnoIniziale = 0;
    CominciaGioc2();
    turno = 1;
  }
}

void VisualizzaMossa()
{
  lcd.print(mossaGiocatore);
}

void VisualizzaNumCorrente()
{
  lcd.setCursor(14,1);
  lcd.print(numeroCorrente);
}

void CominciaGioc1()
{
  lcd.clear();
  lcd.print("Comincia il");
  lcd.setCursor(0,1);
  lcd.print("giocatore 1");
}
void CominciaGioc2()
{
  lcd.clear();
  lcd.print("Comincia il");
  lcd.setCursor(0,1);
  lcd.print("giocatore 2"); 
}

void DeterminaChiVince() //Metodo che determina chi vince in base a chi ha eseguito l'ultima mossa 
{
  if (turno == 0 && numeroCorrente == risDaRaggiungere)
  {
    VinceGioc1();
    TerminaPartita();
    CambiaTurnoIniziale();
  }
  else if (turno == 1 && numeroCorrente == risDaRaggiungere)
  {
    VinceGioc2();
    TerminaPartita();
    CambiaTurnoIniziale();
  }
  else if (turno == 0 && numeroCorrente > risDaRaggiungere)
  {
    VinceGioc2();
    TerminaPartita();
    CambiaTurnoIniziale();
  }
  else if (turno == 1 && numeroCorrente > risDaRaggiungere)
  {
    VinceGioc1();
    TerminaPartita();
    CambiaTurnoIniziale();
  }
}

void VinceGioc1()
{
  lcd.clear();
  lcd.print("VINCE IL");
  lcd.setCursor(0,1);
  lcd.print("GIOCATORE 1  ");
  lcd.write(byte(6));
  delay(1000);
}
void VinceGioc2()
{
  lcd.clear();
  lcd.print("VINCE IL");
  lcd.setCursor(0,1);
  lcd.print("GIOCATORE 2  ");
  lcd.write(byte(6));
  delay(1000);
}

void ScriviRisultato()
{
  lcd.setCursor(0,0);
  lcd.write("SELEZIONA IL");
  lcd.setCursor(0,1);
  lcd.write("RISULTATO");
}

bool MossaNonValida()
{
  lcd.clear();
  lcd.print("MOSSA NON");
  lcd.setCursor(0,1);
  lcd.print("VALIDA");
}
void ScriviMossaGiocatore()
{
  lcd.clear();
  lcd.print("Mossa Giocatore");
  lcd.setCursor(0,1);
  lcd.print(istruzioni);
  VisualizzaNumCorrente();
}

void SchermataIniziale()
{
  lcd.print("BENVENUTO IN");
  lcd.setCursor(0,1);
  lcd.print("ANGELA GAME");
  CreaMela();
  delay(2000);
  for(cont = 0; cont < 16; cont++)
  {
    delay(200);
    lcd.scrollDisplayRight();
    
  }
}

void CreaMela()
{
  lcd.setCursor(13,0);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.setCursor(13,1);
  lcd.write(byte(3));
  lcd.write(byte(4));
  lcd.write(byte(5));
}
void TerminaPartita() //Metodo che termina la partita
{
  numTurni = 0;
  mossaAppoggio = -1;
  mossaGiocatore = 0;
  numeroCorrente = 0;
  risDaRaggiungere = 30;
  val2 = 0;
  ScegliRisDaRaggiungere();
}
