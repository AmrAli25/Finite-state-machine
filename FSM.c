typedef const struct State STyp;

#define goW 0
#define waitW 1
#define goS 2
#define waitS 3
#define goP 4
#define waitAll 5
#define flashP1 6
#define flashP2 7
#define flashP3 8

STyp FSM[9]={
{0x0C, 0x02, 100,{goW,goW,waitW,waitW,waitW,waitW,waitW,waitW}}, // S0) West green, South red, Walk red
{0x14, 0x02, 100,{waitAll,waitAll,waitAll,waitAll,waitAll,waitAll,waitAll,goS}}, // S1) West yellow, South red, Walk red
{0x21, 0x02, 100,{goS,waitS,goS,waitS,waitS,waitS,waitS,waitS}}, // S2) West red, South green, Walk red
{0x22, 0x02, 100,{waitAll,waitAll,waitAll,waitAll,waitAll,waitAll,waitAll,goP}}, // S3) West red, South yellow, Walk red
{0x24, 0x08, 100,{goP,flashP1,flashP1,flashP1,goP,flashP1,flashP1,waitAll}}, // S4) West red, South red, Walk green
{0x24, 0x02, 100,{waitAll,goW,goS,goW,goP,goP,goP,flashP1}}, // S5) West red, South red, Walk red
{0x24, 0x00, 100,{flashP2,flashP2,flashP2,flashP2,flashP2,flashP2,flashP2,flashP2}}, // S6) West red, South red, Walk off
{0x24, 0x02, 100,{flashP3,flashP3,flashP3,flashP3,flashP3,flashP3,flashP3,flashP3}}, // S7) West red, South red, Walk red
{0x24, 0x00, 100,{waitAll,waitAll,waitAll,waitAll,waitAll,waitAll,waitAll,goW}} // S8) West red, South red, Walk off
};

unsigned long cState; // index to the current state
unsigned long Input;
void PLL_Init2(void); // 80 MHz, Program 10.1
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);

// ***** 3. Subroutines Section *****

int main(void){
TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz

PLL_Init2(); // 80 MHz, Program 10.1
SysTick_Init(); // Program 10.2
//Gpio_Init();
PortF_Init();
PortB_Init();
PortE_Init();

cState = waitAll; // Initial state

EnableInterrupts();
while(1){
// output based on current state
GPIO_PORTB_DATA_R = FSM[cState].OutLight;
// output PF3 and PF1
GPIO_PORTF_DATA_R = FSM[cState].OutWalk;
// wait for time according to state
SysTick_Wait10ms(FSM[cState].Time);
// get input
Input = GPIO_PORTE_DATA_R&0x07; // Input 0,1,2
// change the state based on input and current state
cState = FSM[cState].Next[Input];
}
}
