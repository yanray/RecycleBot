// SoftwareSerial - Version: Latest 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define GET_LOW_BYTE(A) (uint8_t)((A))
//Macro function  get lower 8 bits of A
#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)
//Macro function  get higher 8 bits of A
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))
//put A as higher 8 bits   B as lower 8 bits   which amalgamated into 16 bits integer

#define LOBOT_SERVO_FRAME_HEADER         0x55
#define LOBOT_SERVO_MOVE_TIME_WRITE      1
#define LOBOT_SERVO_MOVE_TIME_READ       2
#define LOBOT_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LOBOT_SERVO_MOVE_TIME_WAIT_READ  8
#define LOBOT_SERVO_MOVE_START           11
#define LOBOT_SERVO_MOVE_STOP            12
#define LOBOT_SERVO_ID_WRITE             13
#define LOBOT_SERVO_ID_READ              14

byte LobotCheckSum(byte buf[])
{
  byte i;
  uint16_t temp = 0;
  for (i = 2; i < buf[3] + 2; i++) {
    temp += buf[i];
  }
  temp = ~temp;
  i = (byte)temp;
  return i;
}

void LobotSerialServoMove(SoftwareSerial &SerialX, uint8_t id, int16_t position, uint16_t time)
{
  byte buf[10];
  if(position < 0)
    position = 0;
  if(position > 1000)
    position = 1000;
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
  buf[5] = GET_LOW_BYTE(position);
  buf[6] = GET_HIGH_BYTE(position);
  buf[7] = GET_LOW_BYTE(time);
  buf[8] = GET_HIGH_BYTE(time);
  buf[9] = LobotCheckSum(buf);
  SerialX.write(buf, 10);
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  delay(1000);
}

#define ID1   1
#define ID2   2
#define ID3   3
#define ID4   4
#define ID5   5
#define ID6   6

//int i[6] = {-188, -180, -209, -166, 180, 0};
int i[6] = {-125, -188, -313, -126, 0, 0};
int j[6] = {-125, -188, -313, -126, 0, 400};
int bin[6] = {375, 0, -375, 0, 0, 400};
int put_bin[6] = {375, 0, -375, 0, 0, 0};
int num = 0;

void move_to_initial(){
  LobotSerialServoMove(mySerial, ID6, 412, 1000);
  LobotSerialServoMove(mySerial, ID5, 600, 1000);
  LobotSerialServoMove(mySerial, ID4, 500, 1000);
  LobotSerialServoMove(mySerial, ID3, 500, 1000);
  LobotSerialServoMove(mySerial, ID2, 535, 1000);
  LobotSerialServoMove(mySerial, ID1, 200, 1000);
  delay(2000);
}

void move_to(int pos[]) {
  LobotSerialServoMove(mySerial, ID6, 412 + pos[0], 1000);
  LobotSerialServoMove(mySerial, ID5, 600 + pos[1], 1000);
  LobotSerialServoMove(mySerial, ID4, 500 + pos[2], 1000);
  LobotSerialServoMove(mySerial, ID3, 500 + pos[3], 1000);
  LobotSerialServoMove(mySerial, ID2, 535 + pos[4], 1000);
  LobotSerialServoMove(mySerial, ID1, 200 + pos[5], 1000);
  delay(2000);
}

void move_to_bin() {
  LobotSerialServoMove(mySerial, ID6, 412 + bin[0], 1000);
  LobotSerialServoMove(mySerial, ID5, 600 + bin[1], 1000);
  LobotSerialServoMove(mySerial, ID4, 500 + bin[2], 1000);
  LobotSerialServoMove(mySerial, ID3, 500 + bin[3], 1000);
  LobotSerialServoMove(mySerial, ID2, 535 + bin[4], 1000);
  LobotSerialServoMove(mySerial, ID1, 200 + bin[5], 1000);
  delay(2000);
}

void sweep_workspace() {
  LobotSerialServoMove(mySerial, ID6, 412 + 188, 1000);
  LobotSerialServoMove(mySerial, ID5, 600 - 350, 1000);
  LobotSerialServoMove(mySerial, ID4, 500, 1000);
  LobotSerialServoMove(mySerial, ID3, 500, 1000);
  LobotSerialServoMove(mySerial, ID2, 535, 1000);
  LobotSerialServoMove(mySerial, ID1, 200, 1000);
  delay(2000);

  LobotSerialServoMove(mySerial, ID6, 412 - 188, 1000);
  LobotSerialServoMove(mySerial, ID5, 600 - 350, 1000);
  LobotSerialServoMove(mySerial, ID4, 500, 1000);
  LobotSerialServoMove(mySerial, ID3, 500, 1000);
  LobotSerialServoMove(mySerial, ID2, 535, 1000);
  LobotSerialServoMove(mySerial, ID1, 200, 1000);
  delay(5000);

  move_to_initial();
}


// data to be received from raspberry pi
int destination[6] = {0, 0, 0, 0, 0, 0};
  
void loop() {
  // put your main code here, to run repeatedly:
  move_to_initial();
  communicate();
  // signal, ready to move
  LobotSerialServoMove(mySerial, ID1, 600, 1000);
  delay(1200);
  LobotSerialServoMove(mySerial, ID1, 200, 1000);
  delay(1200);

  move_to(destination);
  LobotSerialServoMove(mySerial, ID1, 600, 1000);
  delay(1200);
  LobotSerialServoMove(mySerial, ID5, 900 , 1000);
  delay(1200);
  LobotSerialServoMove(mySerial, ID6, 712, 1000);
  delay(1200);
  move_to_bin();
  delay(1200);
  LobotSerialServoMove(mySerial, ID1, 100, 1000);
  delay(2000);

  Serial.println("done");

  
}

void communicate(){
  Serial.println("Hello Pi, This is Arduino UNO...:");
  
  for (int pos = 0; pos < 6; pos++) {
  while (Serial.available() == 0){
    ; // wait for input signal
  }
  
  // read the incoming byte:
  if (Serial.available()){
      receive(Serial.parseInt(), pos);
  }
   }
  // Print the array
  for(int i = 0; i < 6; i++)
  {
    Serial.println(destination[i]);
  }
  Serial.println("All completed");
  // Serial.flush();
}

void receive(int n, int pos) { 
  Serial.print("Received:");
  Serial.println(n);
  destination[pos] = n;
  Serial.println("Completed");
}
