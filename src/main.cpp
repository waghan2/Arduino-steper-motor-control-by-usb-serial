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
int vm1 = 200;
int vm2 = 200;
int stpm1 = 500;
int stpm2 = 500;
int acm1 = 50;
int acm2 = 1000;
String movimento = "vm1=200;vm2=200;stpm1=5000;stpm2=5500;acm1=50;acm2=1000;";
AccelStepper stepper1(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);
AccelStepper stepper2(1, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN);

void setup()
{
  Serial.begin(115200);
  stepper1.setMaxSpeed(5000.0);    // em passos por segundo
  stepper1.setMinPulseWidth(200);   // em microsegundos
  stepper1.setAcceleration(300.0); // em passos por segundo²
  stepper1.moveTo(1000);           // em passos

  stepper2.setMaxSpeed(300.0);
  stepper2.setMinPulseWidth(200);
  stepper2.setAcceleration(300.0);
  stepper2.moveTo(50);
}
void voltar()
{
  stepper1.moveTo(0);
  stepper2.moveTo(0);
}
void lerinstru(String movimento)
{
  set = true;vm1=200;vm2=200;stpm1=5000;stpm2=5500;acm1=50;acm2=10000;

  // vm1=200;vm2=200;stpm1=5000;stpm2=5500;acm1=50;acm2=10000;
  int pos = movimento.indexOf(";");
  String aux = movimento.substring(0, pos);
  movimento = movimento.substring(pos + 1);
  pos = aux.indexOf("=");
  String var = aux.substring(0, pos);
  aux = aux.substring(pos + 1);
  int val = aux.toInt();
  if (var == "vm1")
  {
    vm1 = val;
  }
  if (var == "vm2")
  {
    vm2 = val;
  }
  if (var == "stpm1")
  {
    stpm1 = val;
  }
  if (var == "stpm2")
  {
    stpm2 = val;
  }
  if (var == "acm1")
  {
    acm1 = val;
  }
  if (var == "acm2")
  {
    acm2 = val;
  }

  Serial.println(movimento);
  if (movimento.length() > 0)
  { // se ainda houver instruções
    // set = true;
    // lerinstru(movimento);
    // voltar();
  }
}

void loop()
{

  while (Serial.available() > 0)
  {
    movimento = Serial.readString();
    lerinstru(movimento);
  }

  if (set == true)
  {
    stepper1.setMaxSpeed(vm1);
    stepper1.setMinPulseWidth(stpm1);
    stepper1.setAcceleration(acm1);
    stepper2.setMaxSpeed(vm2);
    stepper2.setMinPulseWidth(stpm2);
    stepper2.setAcceleration(acm2);
  }

  if ((stepper1.distanceToGo() == 0) && (stepper2.distanceToGo() == 0))
  {
    set = false;
    stpm1 = 0;
    stpm2 = 0;
  }
  if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());

  if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());
  stepper1.run();
  stepper2.run();
}