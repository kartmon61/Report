#define F_CPU 16000000UL //16MHz 설정
#define BAUD 9600 //9600bfs 설정
#define MYUBRR F_CPU/16/BAUD-1 //UBRR 계산

#include <util/delay.h>
#include <avr/io.h>
#include <string.h> //문자열 라이브러리 추가

// USART 초기화 함수
void USART_Init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr >> 8); //0으로 초기화
	UBRR0L = (unsigned char)ubrr; //그대로 출력
	UCSR0B =(1<<RXEN0) | (1<<TXEN0); //송수신 가능
    //0000 0000 0000 0011 -> 0000 0000 0000 1100 char size 12로 설정
	UCSR0C = (3<< UCSZ0); 
	
}

// USART 데이터 전송 함수
void USART_Transmit(char data){
	while(!((UCSR0A)&(1<<UDRE0))); //입력이 안들어오면 반복
	UDR0 = data; //UDR0에 데이터 저장
}

// USART 데이터 수신 함수
char USART_Receive(){
	while(!(UCSR0A &(1<<RXC0))); //입력이 안들어오면 반복
	return UDR0; //UDR0 반환 
}

//LCD에 char 문자 출력 하는 함수
void LCD_Transmit(int x, int y, char data) {	

	USART_Transmit_String("$G,"); //커서 지정
	USART_Transmit((char)(x+48)); //x 번째 줄
	USART_Transmit(',');
	USART_Transmit((char)(y+48)); //y 번째 칸
	USART_Transmit('\r'); //종료
	USART_Transmit_String("$T,"); //문자열 출력
	USART_Transmit(data); //data 문자 출력
	
}

//LCD에 String 문자열 출력 하는 함수
void LCD_Transmit_String(int x, int y, char* String){

	USART_Transmit_String("$G,"); //커서 지정
	USART_Transmit((char)(x+48)); //x 번째 줄
	USART_Transmit(',');
	USART_Transmit((char)(y+48)); //y 번째 칸
	USART_Transmit('\r'); //종료
	USART_Transmit_String("$T,"); //문자열 출력
	while(*String) //문자열의 주소값이 null이 아닐때 까지
	{
		USART_Transmit(*String++); //char 문자 출력하는 함수 호출
	}
}

int main(void)
{
	USART_Init(MYUBRR); //초기화
	USART_Transmit_String("$I\r"); //LCD 초기화
	
	char string[] = "Leejinho. Department of Electronic Engineering"; //문자열 저장
	int size = strlen(string); //문자열 길이 저장

	_delay_ms(100); //딜레이 0.1초

	while (1)
	{
		//문자열의 길이 만큼 반복
		for(int i=0;i<size;i++){
			USART_Transmit_String("$C\r"); //LCD 화면 클리어
			LCD_Transmit_String(1,1,string+i); // 1,1의 위치에 문자열+i의 주소로 시작하는 문자열 출력
			USART_Transmit('\r'); //종료
			_delay_ms(500); //딜레이 0.5초
		}

		_delay_ms(100); //딜레이 0.1초
	}
}