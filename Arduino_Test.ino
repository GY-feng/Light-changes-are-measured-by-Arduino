
#include <Wire.h> //IIC
#include <math.h> 
#include<EEPROM.h>
#include <BH1750.h>
void serialEvent();
void Clear();
int Read();
void Write(int a);
int Receive();

//模块：
int BH1750address = 0x23;
byte buff[2];

//EEPROM:
int waddress = 0;//写入的地址
int raddress = 0;//读取的地址
#define ADDRESS_LONG 1024

//OTHER:
#define N 12
BH1750 lightMeter;
#define KEY_LEFT  4
#define KEY_RIGHT  5
#define GLIGHT 6
#define RLIGHT 7
#define STOPTIME 500//测量时间间隔60000
String serialString = "";

void setup()
{
	Wire.begin();//IIC
	lightMeter.begin();
	Serial.begin(9600);
	pinMode(KEY_LEFT, INPUT);//左边按下，执行测量
	pinMode(KEY_RIGHT, INPUT);//右边按下，执行数据传输
	pinMode(GLIGHT, OUTPUT);
	pinMode(RLIGHT, OUTPUT);

}
void loop()
{
	if (digitalRead(KEY_LEFT) == LOW)//当左边按钮被按下，执行测量
	{
		digitalWrite(GLIGHT, HIGH);
		delay(40);//延迟消抖
		Clear();//清楚EEPROM原有的数据
		while (true)
		{
			int a = Receive();
			delay(STOPTIME);
			Write(a);
		}
	}
	if (digitalRead(KEY_RIGHT) == LOW)//当右边按钮被按下，执行数据传输
	{
		digitalWrite(RLIGHT, HIGH);
		delay(40);//延迟消抖
		while (true)
		{
			int date = Read();
			delay(20);
			if (date != 0)
			{
				Serial.println(date);
			}
		}
	}
}
void Clear()
{

	for (int i = 0; i <= ADDRESS_LONG; i++)
	{
		EEPROM.write(i, 0);
	}
}
int Read()
{
	int value = 0;
	if (raddress <= ADDRESS_LONG)
	{
		value = EEPROM.read(raddress);
		raddress = raddress + 1;
	}
	return value;
}
void Write(int a)
{
	if (waddress <= ADDRESS_LONG)//还有剩余的存储空间
	{
		EEPROM.write(waddress, a);
		waddress = waddress + 1;
	}
}
int Receive()
{
	float lux = lightMeter.readLightLevel();
	int a = int(lux);
	return a;
}
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar != '\n')
    {
      serialString += inChar;
    }
  }
}
