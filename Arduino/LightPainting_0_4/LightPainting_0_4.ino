// Test d'instructions via Serial pour Neo Pixel
/*
X|01&ABCDEFG|02&HIJKLMNO|03&DADADE|04&LKJU|05&JKJ|06&KNMJG|07&KJMN|08&JMNH|09&SDCG|10&KMNJ|11&JMHJK|12&JHSD|13&ASDF|14&MNBV|15&UJHN|X
*/
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);	// opens serial port, sets data rate to 9600 bps
        Serial.flush();
Serial.println("Debut du code");
delay(2000);
}

void loop() {
char m = 'Z';

Serial.println("Shoot!");

//On attend qu'un message arrive
while (!Serial.available())
{;}
//On a recu un message
//Debut de boucle de lecture
// Un message a le format X|NB1|VALUE1|NB2|VALUE2|X
int xCount = 0;
int mCount = 0;
//Serial.println("Un message en attente");
while ( xCount < 2)
{
//Serial.println("While 1");
      while (!Serial.available())
        {;}
      if (m == 'X')
        {xCount = xCount +1;
        //  Serial.print("Un X recu, c'est le #"); Serial.println(xCount);
        }
      while (m =='|' && xCount < 2)
         {
           int mCount = 0;
           //Serial.println("Un | recu, on decode un message");
           while (!Serial.available())
              {;}
             m = Serial.read();
             if (m == 'X')
                    {xCount = xCount +1;}
                    
          while ( m != '|' && mCount < 6 && xCount < 2 && m != '&' )
              {
 
                  Serial.print(m);
                  mCount++;
                while (!Serial.available())
                    {;}
                    m = Serial.read();
             
             }
             mCount = 0;
          while ( m != '|' && mCount < 6 && xCount < 2)
              {
 
                  Serial.print(m);
                  mCount++;
                while (!Serial.available())
                    {;}
                    m = Serial.read();
             }
           if (m == '|')
               {Serial.println();
               //m = 'X';
             }
           //Serial.println("Sortie de boucle message");
         }
    
m = Serial.read();
   
      
}

//Serial.println("Sortie de boucle totale");



}

/*void decode() //Fonction de décodage du message
{
PARAM_HAUT = 0;
PARAM_BAS = 0;
m = 0; //On analyse le message
n = 0;
p = 1;
while (Serial.available() > 0 && m <20) // On commence par récupérer le message et le stocker dans message[], attention ce sont des nombres
{
message[m] = Serial.read();
Serial.println(char(message[m]));
m++;
}
taille_message = m;
m=0;
//Serial.println("Message lu");  
sens_haut = char(message[m]);
//Serial.println(sens_haut);  

m++;
m++;
while(message[m] != '|')
{

  message_haut[n] = message[m];
  m++;
  n++;
 // Serial.println("Fais moi mal, john john johnny");  
  //Serial.println(message_haut[n]);  
  }  

n--;
while(n >= 0)
  {
    if(message_haut[n] == '-'){PARAM_HAUT = PARAM_HAUT*-1; n--;}
    else{
  PARAM_HAUT = PARAM_HAUT + (message_haut[n]-'0')*p;
  p = p*10 ; 
  n--;
  }}
//Serial.println(PARAM_HAUT);  
m++;
n = 0;
p = 1;
sens_bas = char(message[m]);
//Serial.println(sens_bas);
m++;
m++;
while(message[m] != 124 && m <= taille_message) //On trouve le paramère bas
  {
  message_bas[n] = message[m];
  m++;
  n++;
  } 
n--;
while(n >= 0)
  {
        if(message_bas[n] == '-'){PARAM_BAS = PARAM_BAS*-1; n--;}
    else{
  PARAM_BAS = PARAM_BAS + (message_bas[n]-'0')*p;
  p = p*10;
  n--;
  }
}




//Serial.println(PARAM_BAS);  
}*/
