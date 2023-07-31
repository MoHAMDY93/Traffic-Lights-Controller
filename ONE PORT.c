//Giving names to all leds :)
#define RedWest porta.B0
#define YellowWest porta.B1
#define GreenWest porta.B2

#define RedSouth porta.B3
#define YellowSouth porta.B4 //The special one
#define GreenSouth porta.B5

#define SW_1 portc.B5 //AUTO--0 / MANUAL--1
#define SW_2 portc.B6 //SOUTH--1 / WEST--0

//The array of numbers being displayed on the 7-Segments converted from binary eg. 0b00110010=50 -- (23)
char Display[] = {0 , 16 , 32 , 48 , 64 , 80 , 96 , 112 , 128 , 144
, 1  , 17 , 33 , 49 , 65 , 81 , 97 , 113 , 129 , 145 , 2 , 18 , 34 , 50};

int check_first=0;//To check first befre waiting 3 Useless_SECs [ Auto-South -- Manual-South -- No delay ]

//The start of the loop in each street
int CntWest;
int CntSouth;
int cnt;//The iterator

int const delay = 200;

void main() {
     adcon1=0x07; //To use the analog pins of portA as digital ones
     //Declaring ports( A , B , C , D) as output
     trisd = 0;
     trisc = 0;
     trisa = 0;

     //only Pins 5 & 6 of portC as input
     trisc.B5 = 1;
     trisc.B6 = 1;

     //These two pins are the controllers of the 7_Segments(On / OFF)
     portc.B0=1;
     portc.B1=1;

     //The label of the Manual mode
     Manual :
     if(SW_1==1){
         //Making the all leds off at first coz there values may be randomised
         porta = 0;
         porta.B4 = 1;

         //The south street will be green and the opposite for the west one
         if(SW_2==1){
              if(check_first==0){//if the south one was turned off
                  for(cnt=3 ; cnt>0 ; cnt--){
                    YellowWest=1;//to warn people that the road gonna close(3 sec)
                    RedSouth=1;
                    portd = Display[cnt];
                    delay_ms(delay);
                }
              }
              while(SW_2==1 && SW_1==1){//the lights will not change while the switch is on
                  YellowWest=0;
                  RedSouth=0;
                  GreenSouth=1;
                  RedWest=1;
                  portc.B0=0;
                  portc.B1=0;
              }
              check_first=1;
         }
         else{
              if(check_first==1){//if the West one was turned off
                  for(cnt=3 ; cnt>0 ; cnt--){//the sams as the above one
                    YellowSouth=0;
                    RedWest=1;
                    portd = Display[cnt];
                    delay_ms(delay);
                  }
              }
              while(SW_2==0 && SW_1==1){
                YellowSouth=1;
                RedSouth=1;
                GreenWest=1;
                RedWest=0;
                portc.B0=0;
                portc.B1=0;
              }
              check_first=0;
         }
     }
     while(1){
         //Make sure that 7-Segments are ON
         portc.B0=1;
         portc.B1=1;
         
         //make the default of all leds is off
         porta=0;
         porta.B4=1;

         //Redeclaring the Street counters
         CntWest=15;
         CntSouth=23;
        
         for(cnt=0 ; cnt<15 ; cnt++){
              if(SW_1==1)goto Manual;
              check_first=1;

              //the west street is closed for the whole 15 sec
              RedWest=1;
              portd = Display[CntWest--];

              //the south one will be green for 12 sec and yellow for the rest 3 sec (handled by if , else)
              if(cnt>11){
                  GreenSouth=0;
                  YellowSouth=0;
              }
              else{
                  GreenSouth=1;
              }
              delay_ms(delay);
         }
          //turn the above leds off
          YellowSouth=1;
          RedWest=0;

          //the same as the west street but with the south one :)
          for(cnt=0 ; cnt<23 ; cnt++){
              if(SW_1==1)goto Manual;
              check_first=0;

              RedSouth=1;
              portd = Display[CntSouth--];
              if(cnt>19){
                  GreenWest=0;
                  YellowWest=1;
              }
              else {
                  GreenWest=1;
              }
              delay_ms(delay);
          }
     }
}
                               // By / Mohamed Hamdy