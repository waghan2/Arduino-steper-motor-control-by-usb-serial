#include <Arduino.h>
#include <AccelStepper.h>

// for the Arduino Uno + CNC shield V3

#define MOTOR_A_ENABLE_PIN 8
#define MOTOR_A_STEP_PIN 2
#define MOTOR_A_DIR_PIN 5

#define MOTOR_B_ENABLE_PIN 1
#define MOTOR_B_STEP_PIN 3
#define MOTOR_B_DIR_PIN 6
// MINHAS VARIÁEVIS
int inByte;
bool set = false;
int vm1 = 1800;
int vm2 = 800;
int stpm1 = 500;
int stpm2 = 500;
int acm1 = 5000;
int acm2 = 1000;
String mov1, mov2, mov3, move4, move5;
String allmoves[] = {"mov1{vm1=1800;vm2=800;stpm1=500;stpm2=500;acm1=5000;acm2=1000;}", 
"M2", 
"M3", 
"M4",
"M5"};
AccelStepper stepper1(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);
AccelStepper stepper2(1, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN);

enum moves
{
  mov1,
  mov2,
  mov3,
  mov4,
  mov5,
  mov6,
  mov7,
  mov8,
  mov9,
  mov10
};

void SetMov(String move)
{
  if (move.startsWith("mov1"))
  {
    move.replace("mov1", "");
    move.replace("{", "");
    move.replace("}", "");
    movimento1 = move;
    Serial.println(movimento1);
  }
  if (move.startsWith("mov2"))
  {
    move.replace("mov2", "");
    move.replace("{", "");
    move.replace("}", "");
    movimento2 = move;
    Serial.println(movimento2);
  }
  if (move.startsWith("mov3"))
  {
    move.replace("mov3", "");
    move.replace("{", "");
    move.replace("}", "");
    movimento3 = move;
    Serial.println(movimento3);
  }
  if (move.startsWith("mov4"))
  {
    move.replace("mov4", "");
    move.replace("{", "");
    move.replace("}", "");
    movimento4 = move;
    Serial.println(movimento4);
  }
  if (move.startsWith("mov5"))
  {
    move.replace("mov5", "");
    move.replace("{", "");
    move.replace("}", "");
    movimento5 = move;
    Serial.println(movimento5);
  }
}

void setup()
{
  Serial.begin(115200);
  stepper1.setMaxSpeed(5000.0);    // em passos por segundo
  stepper1.setMinPulseWidth(20);   // em microsegundos
  stepper1.setAcceleration(300.0); // em passos por segundo²
  stepper1.moveTo(1000);           // em passos

  stepper2.setMaxSpeed(300.0);
  stepper2.setMinPulseWidth(20);
  stepper2.setAcceleration(300.0);
  stepper2.moveTo(50);
}
void voltar()
{
  stepper1.moveTo(0);
  stepper2.moveTo(0);
}
void SetVarMoves(String movimento)
{
  set = true;

  // vm1=200;vm2=200;stpm1=5000;stpm2=5500;acm1=50;acm2=10000;

  while (movimento.indexOf(";") != -1)
  {
    String aux = movimento.substring(0, movimento.indexOf(";"));
    movimento = movimento.substring(movimento.indexOf(";") + 1);
    if (aux.indexOf("vm1") != -1)
    {
      vm1 = aux.substring(aux.indexOf("=") + 1).toInt();
      Serial.println("vm1=" + String(vm1));
    }
    else if (aux.indexOf("vm2") != -1)
    {
      vm2 = aux.substring(aux.indexOf("=") + 1).toInt();
      Serial.println("vm2=" + String(vm2));
    }
    else if (aux.indexOf("stpm1") != -1)
    {
      stpm1 = aux.substring(aux.indexOf("=") + 1).toInt();
      Serial.println("stpm1=" + String(stpm1));
    }
    else if (aux.indexOf("stpm2") != -1)
    {
      stpm2 = aux.substring(aux.indexOf("=") + 1).toInt();
      Serial.println("stpm2=" + String(stpm2));
    }
    else if (aux.indexOf("acm1") != -1)
    {
      acm1 = aux.substring(aux.indexOf("=") + 1).toInt();
      Serial.println("acm1=" + String(acm1));
    }
    else if (aux.indexOf("acm2") != -1)
    {
      acm2 = aux.substring(aux.indexOf("=") + 1).toInt();
      Serial.println("acm2=" + String(acm2));
    }
  }
}

void executeMove()
{
  stepper1.setSpeed(vm1);
  stepper1.setAcceleration(acm1);
  stepper1.moveTo(stpm1);
  stepper2.setSpeed(vm2);
  stepper2.setAcceleration(acm2);
  stepper2.moveTo(stpm2);
  stepper1.run();
  stepper2.run();
}

void loop()
{

  while (Serial.available() > 0)
  {
    movimento = Serial.readString();
    Serial.println(movimento);
    SetMov(movimento);
    SetVarMoves(movimento);
  }

  if (movimento1.length() > 0)
  {
    movimento = movimento1;
  }

  if (set == true)
  {
    stepper1.setMaxSpeed(vm1);
    stepper1.moveTo(stpm1);
    stepper1.setAcceleration(acm1);
    stepper2.setSpeed(vm2);
    stepper2.moveTo(stpm2);
    stepper2.setSpeed(acm2);
    movimento = "";
    set = false;
  }

  if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());

  if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());

  stepper1.run();
  stepper2.run();
}