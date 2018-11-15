#include <stdlib.h>
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <SoftwareSerial.h>
#include <Ultrasonic.h>

//------------------------------------------------------------------------------------------------------------------------------
//
//                           Set left side motors pins
//
//------------------------------------------------------------------------------------------------------------------------------
int ENFE = 10;
int motorFrenteEsq1 = 13;
int motorFrenteEsq2 = 12;

int ENTE = 9;
int motorTrasEsq1 = 11;
int motorTrasEsq2 = 2;

//------------------------------------------------------------------------------------------------------------------------------
//
//                           Set Right side motors pins
//
//------------------------------------------------------------------------------------------------------------------------------

int ENFD = 5;
int motorFrenteDir1 = 4;
int motorFrenteDir2 = 3;

int ENTD = 6;
int motorTrasDir1 = 14;
int motorTrasDir2 = 15;

//------------------------------------------------------------------------------------------------------------------------------
//
//                           Set QRE Sensor
//
//------------------------------------------------------------------------------------------------------------------------------

int QRE_cantoDireito = A4;
int QRE_centroDireito = A5;
int QRE_centroEsquerdo = A3;
int QRE_cantoEsquerdo = A2;

int sensorQRE_cantoDireito = 0;  
int sensorQRE_centroDireito = 0; 
int sensorQRE_centroEsquerdo = 0;
int sensorQRE_cantoEsquerdo = 0; 

Ultrasonic ultrasonic(7, 8);
float infoDistancia;

Fuzzy *fuzzy = new Fuzzy();

void setup()
{
    Serial.begin(9600);
    pinMode(motorFrenteEsq1, OUTPUT);
    pinMode(motorFrenteEsq2, OUTPUT);
    pinMode(motorTrasEsq1, OUTPUT);
    pinMode(motorTrasEsq2, OUTPUT);

    pinMode(motorFrenteDir1, OUTPUT);
    pinMode(motorFrenteDir2, OUTPUT);
    pinMode(motorTrasDir1, OUTPUT);
    pinMode(motorTrasDir2, OUTPUT);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                           FuzzyInput HC-SR04 Sensor
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyInput *Distancia = new FuzzyInput(1);

    FuzzySet *MuitoPerto = new FuzzySet(5, 8, 8, 19); //ate 10 cm
    Distancia->addFuzzySet(MuitoPerto);

    FuzzySet *Perto = new FuzzySet(10, 20, 20, 30); //ate 20 cm
    Distancia->addFuzzySet(Perto);

    FuzzySet *Medio = new FuzzySet(30, 45, 45, 60); //ate 45 cm
    Distancia->addFuzzySet(Medio);

    FuzzySet *Longe = new FuzzySet(50, 80, 80, 300); //ate 130 cm
    Distancia->addFuzzySet(Longe);

    fuzzy->addFuzzyInput(Distancia);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                           FuzzyInput Right Side QRE
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyInput *SensorLinhaCantoDireito = new FuzzyInput(2);

    FuzzySet *ForaLinhaCantoDireito = new FuzzySet(0, 200, 400, 600);
    SensorLinhaCantoDireito->addFuzzySet(ForaLinhaCantoDireito);

    FuzzySet *LinhaCantoDireito = new FuzzySet(600, 800, 800, 1200);
    SensorLinhaCantoDireito->addFuzzySet(LinhaCantoDireito);

    fuzzy->addFuzzyInput(SensorLinhaCantoDireito);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                           FuzzyInput Left Side QRE
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyInput *SensorLinhaCentroDireito = new FuzzyInput(3);

    FuzzySet *ForaLinhaCentroDireito = new FuzzySet(0, 200, 400, 600);
    SensorLinhaCentroDireito->addFuzzySet(ForaLinhaCentroDireito);

    FuzzySet *LinhaCentroDireito = new FuzzySet(600, 800, 800, 1200);
    SensorLinhaCentroDireito->addFuzzySet(LinhaCentroDireito);

    fuzzy->addFuzzyInput(SensorLinhaCentroDireito);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                           FuzzyInput Center-Left QRE
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyInput *SensorLinhaCentroEsquerdo = new FuzzyInput(4);

    FuzzySet *ForaLinhaCentroEsquerdo = new FuzzySet(0, 200, 400, 600);
    SensorLinhaCentroEsquerdo->addFuzzySet(ForaLinhaCentroEsquerdo);

    FuzzySet *LinhaCentroEsquerdo = new FuzzySet(600, 800, 800, 1200);
    SensorLinhaCentroEsquerdo->addFuzzySet(LinhaCentroEsquerdo);

    fuzzy->addFuzzyInput(SensorLinhaCentroEsquerdo);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                           FuzzyInput Left-Corner QRE
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyInput *SensorLinhaCantoEsquerdo = new FuzzyInput(5); // FuzzyficaÃ§Ã£o QRE

    FuzzySet *ForaLinhaCantoEsquerdo = new FuzzySet(0, 200, 400, 600);
    SensorLinhaCantoEsquerdo->addFuzzySet(ForaLinhaCantoEsquerdo);

    FuzzySet *LinhaCantoEsquerdo = new FuzzySet(600, 800, 800, 1200);
    SensorLinhaCantoEsquerdo->addFuzzySet(LinhaCantoEsquerdo);

    fuzzy->addFuzzyInput(SensorLinhaCantoEsquerdo);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                                  Motors Speed
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyOutput *Velocidade = new FuzzyOutput(1);
    FuzzySet *MuitoLento = new FuzzySet(0, 0, 0, 0); // 0
    Velocidade->addFuzzySet(MuitoLento);

    FuzzySet *Lento = new FuzzySet(80, 105, 105, 125); // 104
    Velocidade->addFuzzySet(Lento);

    FuzzySet *Normal = new FuzzySet(110, 135, 135, 165); // 136
    Velocidade->addFuzzySet(Normal);

    FuzzySet *Rapido = new FuzzySet(190, 225, 225, 255); // 223
    Velocidade->addFuzzySet(Rapido);

    fuzzy->addFuzzyOutput(Velocidade);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                                  Speed Settings - Right Side
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyOutput *VelocidadeAjusteDireita = new FuzzyOutput(2);

    FuzzySet *MuitoLentoAjusteDireita = new FuzzySet(0, 0, 0, 0); //0
    VelocidadeAjusteDireita->addFuzzySet(MuitoLentoAjusteDireita);

    FuzzySet *LentoAjusteDireita = new FuzzySet(120, 145, 145, 175); //140
    VelocidadeAjusteDireita->addFuzzySet(LentoAjusteDireita);

    FuzzySet *NormalAjusteDireita = new FuzzySet(140, 175, 175, 200); //170
    VelocidadeAjusteDireita->addFuzzySet(NormalAjusteDireita);

    FuzzySet *RapidoAjusteDireita = new FuzzySet(230, 245, 245, 255); //240
    VelocidadeAjusteDireita->addFuzzySet(RapidoAjusteDireita);

    fuzzy->addFuzzyOutput(VelocidadeAjusteDireita);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                                  Speed Settings - Left Side
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    FuzzyOutput *VelocidadeAjusteEsquerda = new FuzzyOutput(3);

    FuzzySet *MuitoLentoAjusteEsquerda = new FuzzySet(0, 0, 0, 0);
    VelocidadeAjusteEsquerda->addFuzzySet(MuitoLentoAjusteEsquerda);

    FuzzySet *LentoAjusteEsquerda = new FuzzySet(120, 145, 145, 175); //140
    VelocidadeAjusteEsquerda->addFuzzySet(LentoAjusteEsquerda);

    FuzzySet *NormalAjusteEsquerda = new FuzzySet(140, 175, 175, 200); //170
    VelocidadeAjusteEsquerda->addFuzzySet(NormalAjusteEsquerda);

    FuzzySet *RapidoAjusteEsquerda = new FuzzySet(230, 245, 245, 255); //240
    VelocidadeAjusteEsquerda->addFuzzySet(RapidoAjusteEsquerda);

    fuzzy->addFuzzyOutput(VelocidadeAjusteEsquerda);

    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------
    //
    //                                  Assembly FUZZY Rules
    //
    //------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------------------

    //Rule 1
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito1 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito1->joinWithAND(ForaLinhaCentroEsquerdo, LinhaCentroDireito);

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito1AndDistanciaMuitoPerto1 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito1AndDistanciaMuitoPerto1->joinWithAND(ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito1, MuitoPerto);

    FuzzyRuleConsequent *thenVelocidadeLento1 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento1->addOutput(MuitoLento);                           
    thenVelocidadeLento1->addOutput(MuitoLentoAjusteEsquerda);             

    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito1AndDistanciaMuitoPerto1, thenVelocidadeLento1); 
    fuzzy->addFuzzyRule(fuzzyRule1);                                                                                                                                                         

    //Rule 2
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito2 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito2->joinWithAND(ForaLinhaCentroEsquerdo, LinhaCentroDireito);

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito2AndDistanciaPerto2 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito2AndDistanciaPerto2->joinWithAND(ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito2, Perto);

    FuzzyRuleConsequent *thenVelocidadeLento2 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento2->addOutput(Lento);                                
    thenVelocidadeLento2->addOutput(LentoAjusteEsquerda);                  

    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito2AndDistanciaPerto2, thenVelocidadeLento2); 
    fuzzy->addFuzzyRule(fuzzyRule2);                                                                                                                                                    

    //Rule 3
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito3 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito3->joinWithAND(ForaLinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito3AndDistanciaMedio3 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito3AndDistanciaMedio3->joinWithAND(ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito3, Medio);

    FuzzyRuleConsequent *thenVelocidadeLento3 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento3->addOutput(Normal);                               
    thenVelocidadeLento3->addOutput(NormalAjusteEsquerda);                 

    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito3AndDistanciaMedio3, thenVelocidadeLento3); 
    fuzzy->addFuzzyRule(fuzzyRule3);                                                                                                                                                    

    //Rule 4
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito4 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito4->joinWithAND(ForaLinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito4AndDistanciaLonge4 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito4AndDistanciaLonge4->joinWithAND(ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito4, Longe);

    FuzzyRuleConsequent *thenVelocidadeLento4 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento4->addOutput(Rapido);                               
    thenVelocidadeLento4->addOutput(RapidoAjusteEsquerda);                 

    FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito4AndDistanciaLonge4, thenVelocidadeLento4); 
    fuzzy->addFuzzyRule(fuzzyRule4);                                                                                                                                                    

    //Rule 5
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito5 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito5->joinWithAND(LinhaCentroEsquerdo, ForaLinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito5AndDistanciaMuitoPerto5 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito5AndDistanciaMuitoPerto5->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito5, MuitoPerto);

    FuzzyRuleConsequent *thenVelocidadeLento5 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento5->addOutput(MuitoLento);                           
    thenVelocidadeLento5->addOutput(MuitoLentoAjusteDireita);              

    FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito5AndDistanciaMuitoPerto5, thenVelocidadeLento5); 
    fuzzy->addFuzzyRule(fuzzyRule5);                                                                                                                                                         

    //Rule 6
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito6 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito6->joinWithAND(LinhaCentroEsquerdo, ForaLinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito6AndDistanciaPerto6 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito6AndDistanciaPerto6->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito6, Perto);

    FuzzyRuleConsequent *thenVelocidadeLento6 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento6->addOutput(Lento);                                
    thenVelocidadeLento6->addOutput(LentoAjusteDireita);                   

    FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito6AndDistanciaPerto6, thenVelocidadeLento6); 
    fuzzy->addFuzzyRule(fuzzyRule6);                                                                                                                                                    

    //Rule 7
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito7 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito7->joinWithAND(LinhaCentroEsquerdo, ForaLinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito7AndDistanciaMedio7 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito7AndDistanciaMedio7->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito7, Medio);

    FuzzyRuleConsequent *thenVelocidadeLento7 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento7->addOutput(Normal);                               
    thenVelocidadeLento7->addOutput(NormalAjusteDireita);                  

    FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito7AndDistanciaMedio7, thenVelocidadeLento7); 
    fuzzy->addFuzzyRule(fuzzyRule7);                                                                                                                                                    

    //Rule 8
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito8 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito8->joinWithAND(LinhaCentroEsquerdo, ForaLinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito8AndDistanciaMedio8 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito8AndDistanciaMedio8->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito8, Longe);

    FuzzyRuleConsequent *thenVelocidadeLento8 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento8->addOutput(Rapido);                               
    thenVelocidadeLento8->addOutput(RapidoAjusteDireita);                  

    FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito8AndDistanciaMedio8, thenVelocidadeLento8); 
    fuzzy->addFuzzyRule(fuzzyRule8);                                                                                                                                                    

    //Rule 9
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito9 = new FuzzyRuleAntecedent();      
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito9->joinWithAND(LinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito9AndDistanciaMuitoPerto9 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito9AndDistanciaMuitoPerto9->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito9, MuitoPerto);

    FuzzyRuleConsequent *thenVelocidadeLento9 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento9->addOutput(MuitoLento);                           

    FuzzyRule *fuzzyRule9 = new FuzzyRule(9, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito9AndDistanciaMuitoPerto9, thenVelocidadeLento9); 
    fuzzy->addFuzzyRule(fuzzyRule9);                                                                                                                                                     

    //Rule 10
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito10 = new FuzzyRuleAntecedent();      
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito10->joinWithAND(LinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito10AndDistanciaPerto10 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito10AndDistanciaPerto10->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito10, Perto);

    FuzzyRuleConsequent *thenVelocidadeLento10 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento10->addOutput(Lento);                                

    FuzzyRule *fuzzyRule10 = new FuzzyRule(10, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito10AndDistanciaPerto10, thenVelocidadeLento10); 
    fuzzy->addFuzzyRule(fuzzyRule10);                                                                                                                                                    

    //Rule 11
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito11 = new FuzzyRuleAntecedent();      
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito11->joinWithAND(LinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito11AndDistanciaMedio11 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito11AndDistanciaMedio11->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito11, Medio);

    FuzzyRuleConsequent *thenVelocidadeLento11 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento11->addOutput(Normal);                               

    FuzzyRule *fuzzyRule11 = new FuzzyRule(11, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito11AndDistanciaMedio11, thenVelocidadeLento11); 
    fuzzy->addFuzzyRule(fuzzyRule11);                                                                                                                                                    

    //Rule 12
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito12 = new FuzzyRuleAntecedent();      
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito12->joinWithAND(LinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito12AndDistanciaLonge12 = new FuzzyRuleAntecedent();
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito12AndDistanciaLonge12->joinWithAND(ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito12, Longe);

    FuzzyRuleConsequent *thenVelocidadeLento12 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento12->addOutput(Rapido);                               

    FuzzyRule *fuzzyRule12 = new FuzzyRule(12, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito12AndDistanciaLonge12, thenVelocidadeLento12); 
    fuzzy->addFuzzyRule(fuzzyRule12);                                                                                                                                                    

    //  Rule 13
    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito13 = new FuzzyRuleAntecedent();    
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito13->joinWithAND(LinhaCantoEsquerdo, LinhaCantoDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito13AndDistanciaMuitoPerto13 = new FuzzyRuleAntecedent();
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito13AndDistanciaMuitoPerto13->joinWithAND(ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito13, MuitoPerto);

    FuzzyRuleConsequent *thenVelocidadeLento13 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento13->addOutput(MuitoLento);                           

    FuzzyRule *fuzzyRule13 = new FuzzyRule(13, ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito13AndDistanciaMuitoPerto13, thenVelocidadeLento13); 
    fuzzy->addFuzzyRule(fuzzyRule13);                                                                                                                                                     

    //Rule 14
    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito14 = new FuzzyRuleAntecedent();    
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito14->joinWithAND(LinhaCantoEsquerdo, LinhaCantoDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito14AndDistanciaPerto14 = new FuzzyRuleAntecedent();
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito14AndDistanciaPerto14->joinWithAND(ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito14, Perto);

    FuzzyRuleConsequent *thenVelocidadeLento14 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento14->addOutput(Lento);                                

    FuzzyRule *fuzzyRule14 = new FuzzyRule(14, ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito14AndDistanciaPerto14, thenVelocidadeLento14); 
    fuzzy->addFuzzyRule(fuzzyRule14);                                                                                                                                                

    //Rule 15
    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito15 = new FuzzyRuleAntecedent();    
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito15->joinWithAND(LinhaCantoEsquerdo, LinhaCantoDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito15AndDistanciaMedio15 = new FuzzyRuleAntecedent();
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito15AndDistanciaMedio15->joinWithAND(ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito15, Medio);

    FuzzyRuleConsequent *thenVelocidadeLento15 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento15->addOutput(Normal);                               

    FuzzyRule *fuzzyRule15 = new FuzzyRule(15, ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito15AndDistanciaMedio15, thenVelocidadeLento15); 
    fuzzy->addFuzzyRule(fuzzyRule15);                                                                                                                                                

    //Rule 16
    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito16 = new FuzzyRuleAntecedent();    
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito16->joinWithAND(LinhaCantoEsquerdo, LinhaCantoDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito16AndDistanciaLonge16 = new FuzzyRuleAntecedent();
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito16AndDistanciaLonge16->joinWithAND(ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito16, Longe);

    FuzzyRuleConsequent *thenVelocidadeLento16 = new FuzzyRuleConsequent(); 
    thenVelocidadeLento16->addOutput(Rapido);                               

    FuzzyRule *fuzzyRule16 = new FuzzyRule(16, ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito16AndDistanciaLonge16, thenVelocidadeLento16); 
    fuzzy->addFuzzyRule(fuzzyRule16);                                                                                                                                                

    //Rule 17
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito17 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito17->joinWithAND(ForaLinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleConsequent *thenVelocidadeNormal17 = new FuzzyRuleConsequent(); 
    thenVelocidadeNormal17->addOutput(Lento);                                
    thenVelocidadeNormal17->addOutput(LentoAjusteEsquerda);                  

    FuzzyRule *fuzzyRule17 = new FuzzyRule(17, ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito17, thenVelocidadeNormal17); 
    fuzzy->addFuzzyRule(fuzzyRule17);                                                                                                                                      

    //Rule 18
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito18 = new FuzzyRuleAntecedent();          
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito18->joinWithAND(LinhaCentroEsquerdo, ForaLinhaCentroDireito); 

    FuzzyRuleConsequent *thenVelocidadeNormal18 = new FuzzyRuleConsequent(); 
    thenVelocidadeNormal18->addOutput(Lento);                                
    thenVelocidadeNormal18->addOutput(LentoAjusteDireita);                   

    FuzzyRule *fuzzyRule18 = new FuzzyRule(1, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito18, thenVelocidadeNormal18); 
    fuzzy->addFuzzyRule(fuzzyRule1);                                                                                                                                      

    //Rule 19
    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito19 = new FuzzyRuleAntecedent();      
    ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito19->joinWithAND(LinhaCentroEsquerdo, LinhaCentroDireito); 

    FuzzyRuleConsequent *thenVelocidadeNormal19 = new FuzzyRuleConsequent(); 
    thenVelocidadeNormal19->addOutput(Lento);                                

    FuzzyRule *fuzzyRule19 = new FuzzyRule(19, ifSensorLinhaCentroEsquerdoLinhaCentroEsquerdoAndSensorLinhaCentroDireitoLinhaCentroDireito19, thenVelocidadeNormal19); 
    fuzzy->addFuzzyRule(fuzzyRule19);                                                                                                                                  

    //Rule 20
    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito20 = new FuzzyRuleAntecedent();    
    ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito20->joinWithAND(LinhaCantoEsquerdo, LinhaCantoDireito); 

    FuzzyRuleConsequent *thenVelocidadeNormal20 = new FuzzyRuleConsequent(); 
    thenVelocidadeNormal20->addOutput(Lento);                                

    FuzzyRule *fuzzyRule20 = new FuzzyRule(20, ifSensorLinhaCantoEsquerdoLinhaCantoEsquerdoAndSensorLinhaCantoDireitoLinhaCantoDireito20, thenVelocidadeNormal20); 
    fuzzy->addFuzzyRule(fuzzyRule20);                                                                                                                              

    //Rule 21
    FuzzyRuleAntecedent *ifSensorLinhaCantoEsquerdoForaLinhaCantoEsquerdoAndSensorLinhaCantoDireitoForaLinhaCantoDireito21 = new FuzzyRuleAntecedent();            
    ifSensorLinhaCantoEsquerdoForaLinhaCantoEsquerdoAndSensorLinhaCantoDireitoForaLinhaCantoDireito21->joinWithAND(ForaLinhaCantoEsquerdo, ForaLinhaCantoDireito); 

    FuzzyRuleAntecedent *ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito21 = new FuzzyRuleAntecedent();              
    ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito21->joinWithAND(ForaLinhaCentroEsquerdo, ForaLinhaCentroDireito); 

    FuzzyRuleAntecedent *ifTodosForaLinha = new FuzzyRuleAntecedent();                                                                                                                                                                       
    ifTodosForaLinha->joinWithAND(ifSensorLinhaCantoEsquerdoForaLinhaCantoEsquerdoAndSensorLinhaCantoDireitoForaLinhaCantoDireito21, ifSensorLinhaCentroEsquerdoForaLinhaCentroEsquerdoAndSensorLinhaCentroDireitoForaLinhaCentroDireito21); 

    FuzzyRuleConsequent *thenVelocidadeMuitoLento21 = new FuzzyRuleConsequent(); 
    thenVelocidadeMuitoLento21->addOutput(MuitoLento);                           

    FuzzyRule *fuzzyRule21 = new FuzzyRule(21, ifTodosForaLinha, thenVelocidadeMuitoLento21); 
    fuzzy->addFuzzyRule(fuzzyRule21);                                                         

    Serial.println("Reading sensors input ...");
}

void loop()
{
    long microsec = ultrasonic.timing();
    infoDistancia = ultrasonic.convert(microsec, Ultrasonic::CM);

    sensorQRE_cantoDireito = analogRead(QRE_cantoDireito);
    sensorQRE_centroDireito = analogRead(QRE_centroDireito);
    sensorQRE_centroEsquerdo = analogRead(QRE_centroEsquerdo);
    sensorQRE_cantoEsquerdo = analogRead(QRE_cantoEsquerdo);

    fuzzy->setInput(1, infoDistancia);
    fuzzy->setInput(2, sensorQRE_cantoDireito);
    fuzzy->setInput(3, sensorQRE_centroDireito);
    fuzzy->setInput(4, sensorQRE_centroEsquerdo);
    fuzzy->setInput(5, sensorQRE_cantoEsquerdo);

    fuzzy->fuzzify();

    int Velocidade = fuzzy->defuzzify(1);
    int VelocidadeAjusteDireita = fuzzy->defuzzify(2);
    int VelocidadeAjusteEsquerda = fuzzy->defuzzify(3);

    bool wasTheRuleFired1 = fuzzy->isFiredRule(1);
    bool wasTheRuleFired2 = fuzzy->isFiredRule(2);
    bool wasTheRuleFired3 = fuzzy->isFiredRule(3);
    bool wasTheRuleFired4 = fuzzy->isFiredRule(4);
    bool wasTheRuleFired5 = fuzzy->isFiredRule(5);
    bool wasTheRuleFired6 = fuzzy->isFiredRule(6);
    bool wasTheRuleFired7 = fuzzy->isFiredRule(7);
    bool wasTheRuleFired8 = fuzzy->isFiredRule(8);
    bool wasTheRuleFired9 = fuzzy->isFiredRule(9);
    bool wasTheRuleFired10 = fuzzy->isFiredRule(10);
    bool wasTheRuleFired11 = fuzzy->isFiredRule(11);
    bool wasTheRuleFired12 = fuzzy->isFiredRule(12);
    bool wasTheRuleFired13 = fuzzy->isFiredRule(13);
    bool wasTheRuleFired14 = fuzzy->isFiredRule(14);
    bool wasTheRuleFired15 = fuzzy->isFiredRule(15);
    bool wasTheRuleFired16 = fuzzy->isFiredRule(16);
    bool wasTheRuleFired17 = fuzzy->isFiredRule(17);
    bool wasTheRuleFired18 = fuzzy->isFiredRule(18);
    bool wasTheRuleFired19 = fuzzy->isFiredRule(19);
    bool wasTheRuleFired20 = fuzzy->isFiredRule(20);
    bool wasTheRuleFired21 = fuzzy->isFiredRule(21);

    if (wasTheRuleFired1 == true)
    {
        //Left
        analogWrite(ENFE, VelocidadeAjusteEsquerda);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, VelocidadeAjusteEsquerda);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired2 == true)
    {
        //Left
        analogWrite(ENFE, VelocidadeAjusteEsquerda);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, VelocidadeAjusteEsquerda);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired3 == true)
    {
        //Left
        analogWrite(ENFE, VelocidadeAjusteEsquerda);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, VelocidadeAjusteEsquerda);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired4 == true)
    {
        //Left
        analogWrite(ENFE, VelocidadeAjusteEsquerda);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, VelocidadeAjusteEsquerda);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired5 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, VelocidadeAjusteDireita);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, VelocidadeAjusteDireita);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired6 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, VelocidadeAjusteDireita);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, VelocidadeAjusteDireita);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired7 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, VelocidadeAjusteDireita);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, VelocidadeAjusteDireita);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired8 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, VelocidadeAjusteDireita);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, VelocidadeAjusteDireita);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired9 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired10 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired11 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired12 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired13 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired14 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired15 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired16 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired17 == true)
    {
        //Left
        analogWrite(ENFE, VelocidadeAjusteEsquerda);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, VelocidadeAjusteEsquerda);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired18 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, VelocidadeAjusteDireita);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, VelocidadeAjusteDireita);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired19 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired20 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
    else if (wasTheRuleFired21 == true)
    {
        //Left
        analogWrite(ENFE, Velocidade);
        digitalWrite(motorFrenteEsq1, HIGH);
        digitalWrite(motorFrenteEsq2, LOW);

        analogWrite(ENTE, Velocidade);
        digitalWrite(motorTrasEsq1, LOW);
        digitalWrite(motorTrasEsq2, HIGH);

        //Right
        analogWrite(ENFD, Velocidade);
        digitalWrite(motorFrenteDir1, HIGH);
        digitalWrite(motorFrenteDir2, LOW);

        analogWrite(ENTD, Velocidade);
        digitalWrite(motorTrasDir1, LOW);
        digitalWrite(motorTrasDir2, HIGH);
    }
}
