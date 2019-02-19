#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

Fuzzy* fuzzy = new Fuzzy(); 

int const referenciapin = A0;
float referenciaValor = 0;
float referenciaValorGrados = 0;

int const sensorpin = A1;
float sensorValor = 0;
float sensorValorGrados = 0;

int const salidaControl = A2;
float senalControl = 0;

int IN1 = 8;
int IN2 = 9;

//filtro para los potenciometros para que sea estable

int sensorLectura[10];
int referenciaLectura[10];
int sensorTotal = 0;
int referenciaTotal = 0;
int sensorPromedio = 0;
int referenciaPromedio = 0;

int sensor = 0;
int referencia = 0;

void setup() {

pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);

for (sensor = 0; sensor < 10; sensor++){
  sensorLectura[sensor] = 0;
}
sensor =0;

for (referencia = 0; referencia < 10; referencia++){
  referenciaLectura[referencia] = 0;
}
referencia=0;

FuzzyInput* error = new FuzzyInput(1); // entrada del conjunto difuso

FuzzySet* ENG = new FuzzySet(-300, -300, -150, -75); //Error Negativo Grande, trapecio.
error -> addFuzzySet(ENG); //Se añade el conunto al universo de entrada.
FuzzySet* ENP = new FuzzySet(-150, -75, -75, 0); //Eror Negativo Peuqeño, TRIANGULAR
error -> addFuzzySet(ENP); //Se añade el conunto al universo de entrada.
FuzzySet* EC = new FuzzySet(-75, 0, 0, 75); //Error Cero, TRIANGULAR
error -> addFuzzySet(EC); //Se añade el conunto al universo de entrada.
FuzzySet* EPP = new FuzzySet(0, 75, 75, 150); //Error Positivo pequeño,TRIANGULAR
error -> addFuzzySet(EPP); //Se añade el conunto al universo de entrada.
FuzzySet* EPG = new FuzzySet(75, 150, 300, 300); //Error Positivo Grande, Trapeio
error -> addFuzzySet(EPG); //Se añade el conunto al universo de entrada.
fuzzy -> addFuzzyInput(error);

FuzzyOutput* voltaje = new FuzzyOutput(1);
FuzzySet* VNG = new FuzzySet(-12, -12, -6, -3); // Voltaje Negativo Grande, trapecio.
voltaje -> addFuzzySet(VNG); //Se añade el conunto al universo de salida.
FuzzySet* VNP = new FuzzySet(-6, -3, -3, 0); // Voltaje Negativo Pequeño, Triangular.
voltaje -> addFuzzySet(VNP); //Se añade el conunto al universo de salida.
FuzzySet* VC = new FuzzySet(-3, 0, 0, 3); // Voltaje Cero, Triangular.
voltaje -> addFuzzySet(VC); //Se añade el conunto al universo de salida.
FuzzySet* VPP = new FuzzySet(0, 3, 3, 6); // Voltaje Positivo Pequeño, Triangular.
voltaje -> addFuzzySet(VPP); //Se añade el conunto al universo de salida.
FuzzySet* VPG = new FuzzySet(3, 6, 10, 12); // Voltaje Positivo Grande, trapecio.
voltaje -> addFuzzySet(VPG); //Se añade el conunto al universo de salida.
fuzzy -> addFuzzyOutput(voltaje); 

////////////////////SE establecen las Reglas//////////////////////////////////

//////////  REGLA 1////////////////
FuzzyRuleAntecedent* siErrorNegativoGrande = new FuzzyRuleAntecedent();
siErrorNegativoGrande ->joinSingle(ENG);                    //Antecedente
FuzzyRuleConsequent*entoncesVoltajeNegativoGrande = new FuzzyRuleConsequent();
entoncesVoltajeNegativoGrande->addOutput(VNG);          //Consecuencia
FuzzyRule* reglaDifusa1 = new FuzzyRule(1, siErrorNegativoGrande, entoncesVoltajeNegativoGrande);
fuzzy -> addFuzzyRule(reglaDifusa1);                        //Asignación

//////////  REGLA 2////////////////
FuzzyRuleAntecedent* siErrorNegativoPequeno = new FuzzyRuleAntecedent();
siErrorNegativoPequeno ->joinSingle(ENP);                    //Antecedente
FuzzyRuleConsequent*entoncesVoltajeNegativoPequeno = new FuzzyRuleConsequent();
entoncesVoltajeNegativoPequeno->addOutput(VNP);          //Consecuencia
FuzzyRule* reglaDifusa2 = new FuzzyRule(1, siErrorNegativoPequeno, entoncesVoltajeNegativoPequeno);
fuzzy -> addFuzzyRule(reglaDifusa2);                        //Asignación

//////////  REGLA 3////////////////
FuzzyRuleAntecedent* siErrorCero = new FuzzyRuleAntecedent();
siErrorCero  ->joinSingle(EC);                    //Antecedente
FuzzyRuleConsequent*entoncesVoltajeCero = new FuzzyRuleConsequent();
entoncesVoltajeCero->addOutput(VC);          //Consecuencia
FuzzyRule* reglaDifusa3 = new FuzzyRule(1, siErrorCero, entoncesVoltajeCero);
fuzzy -> addFuzzyRule(reglaDifusa3);                        //Asignación

//////////  REGLA 4////////////////
FuzzyRuleAntecedent* siErrorPositivoPequeno = new FuzzyRuleAntecedent();
siErrorPositivoPequeno  ->joinSingle(EPP);                    //Antecedente
FuzzyRuleConsequent*entoncesVoltajePositivoPequeno = new FuzzyRuleConsequent();
entoncesVoltajePositivoPequeno->addOutput(VPP);          //Consecuencia
FuzzyRule* reglaDifusa4 = new FuzzyRule(1, siErrorPositivoPequeno, entoncesVoltajePositivoPequeno);
fuzzy -> addFuzzyRule(reglaDifusa4);                        //Asignación


//////////  REGLA 5////////////////
FuzzyRuleAntecedent* siErrorPositivoGrande = new FuzzyRuleAntecedent();
siErrorPositivoGrande  ->joinSingle(EPG);                    //Antecedente
FuzzyRuleConsequent*entoncesVoltajePositivoGrande = new FuzzyRuleConsequent();
entoncesVoltajePositivoGrande->addOutput(VPG);          //Consecuencia
FuzzyRule* reglaDifusa5 = new FuzzyRule(1, siErrorPositivoGrande, entoncesVoltajePositivoGrande);
fuzzy -> addFuzzyRule(reglaDifusa5);                        //Asignación

}

void loop() {

  referenciaTotal = referenciaTotal - referenciaLectura[referencia]; 
  referenciaLectura[referencia] = analogRead(referenciapin); // se lee el dato del pin  y se almacena en el vector
  referenciaTotal = referenciaTotal + referenciaLectura[referencia]; //se acumula la suma de cada posicion en la referencia
  referencia = referencia + 1; // se incrementa el valor de la referencia
  if (referencia >= 10){

    referencia =0;
    referenciaPromedio = referenciaTotal / 10;
    referenciaValor = referenciaPromedio;
    referenciaValor = referenciaValor * 100 / 1023;
    referenciaValorGrados = map(referenciaValor, 0, 100, 0, 300);
    
  }

  sensorTotal = sensorTotal - sensorLectura[sensor]; 
  sensorLectura[sensor] = analogRead(sensorpin); // se lee el dato del pin  y se almacena en el vector
  sensorTotal = sensorTotal + sensorLectura[sensor];  //se acumula la suma de cada posicion del sensor
  sensor = sensor + 1; // se incrementa el valor de la referencia
  if (sensor >= 10){

    sensor =0;
    sensorPromedio = sensorTotal / 10;
    sensorValor = sensorPromedio;
    sensorValor = sensorValor * 100 / 1023; // indica el porcentaje de rotacion el maximo valor que se obtiene es 1023
    sensorValorGrados = map(sensorValor, 0, 100, 0, 300); //0% equivale a 0 grado y 100 % equivale a 300 grados 
    
  }

  float e = referenciaValorGrados - sensorValorGrados; // se toma los valor del error

  fuzzy -> setInput(1, e);   //  se toma los valor para que se ffusifique
  fuzzy ->fuzzify();   //Se fuzifica el sistema

  float v = fuzzy -> defuzzify(1); // se fuzifica el valor, si el valor es cero entonces el valor fisificado va hacer cero como esta en la regla

if  (v < 0){

   digitalWrite(IN1, LOW); // 
   digitalWrite(IN2, HIGH);
   senalControl = map(v, -8, 0, 0, 255 );//si v cercano a 0 va asignar un valor de 5v 
   analogWrite(salidaControl, senalControl); 
}
if  (v > 0){

   digitalWrite(IN1, HIGH); 
   digitalWrite(IN2, LOW);
   senalControl = map(v, 0, 8, 0, 255 ); //si ve es cercano a 0 se asigna un valor de 0V 
   analogWrite(salidaControl, senalControl); //manda a mover el servo
}
 if (v = 0){

   digitalWrite(IN1, LOW); // recibe cero volteos el motor y no se mueve
   digitalWrite(IN2, LOW);
   v=0;
   senalControl = v;
   analogWrite(salidaControl, senalControl); 
   
   
  }
}
