// Test d'instructions via Serial pour Neo Pixel
/*
X|01&ABCDEFG|02&HIJKLMNO|03&DADADE|04&LKJU|05&JKJ|06&KNMJG|07&KJMN|08&JMNH|09&SDCG|10&KMNJ|11&JMHJK|12&JHSD|13&ASDF|14&MNBV|15&UJHN|X
*/
char m = 'Z';
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);	// opens serial port, sets data rate to 9600 bps
        Serial.flush();
Serial.println("Debut du code");
delay(2000);
}

void loop() {


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

m = Serial.read();

if (m == 'P')
  {
  // We are receiving a pixel message. 
  incomingPixel();
  }
else if (m == 'T')
  {
  incomingTimer();
  }
else if (m == 'G')
  {
  incomingGo();
  }

}

void incomingPixel()
{
  //Serial.println("It is a Pixel!");
while (!Serial.available())
        {;}
//Serial.print("Starting at pixel: ");
        m = Serial.read();
while (m != '&')
  {
  while (!Serial.available())
      {;}
//  Serial.print(m);
  m = Serial.read();
  }
Serial.println();  

if (m == '&')
  {
 // Serial.println("I have received the pixel, now here are the colours");
  }
while (!Serial.available())
        {;}
    m = Serial.read();
    
while ( m != 'X')
  {

  if (m != '|')
    {  Serial.print(m);}
  else 
    {Serial.println();} 
    
     while (!Serial.available())
        {;}
    m = Serial.read();
  }
  
  

  
}

void incomingTimer()
{
while (!Serial.available())
        {;}
}

void incomingGo()
{
while (!Serial.available())
        {;}
}


/*
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
                    
          while ( m != '|' && mCount <  && xCount < 2 && m != '&' )
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
 
                  //Serial.print(m);
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
*/
