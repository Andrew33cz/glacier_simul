/* ------------------------------------
*      Ondrej Cech (xcecho06), 2020
*  ------------------------------------
*/

#include <iostream>
#include <vector>
#include <math.h>
#include <stdexcept>
#include <iomanip>
#include <string>

#include <cstdlib>
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif


using namespace std;

# define M_PI           3.14159265358979323846  /* pi */


struct points {
  double x_axis;
  double y_axis;
};

  void ClearScreen()
    {
    cout << string( 100, '\n' );
    }


/*
 * @brief   genruje popis povrchu
 * @param   vect vstupni vektor
 * @param   Pts - vektor s vyslednými body
*/
void active_points (vector<double> &vect, vector<points> &Pts)
{
    Pts.clear();
    int counter=0;
    for(int i=0; i<vect.size(); i++){
        if(i!=0){
                if(vect[i-1]>vect[i]){//klesa
                    for(int j=round(vect[i-1])-1;j>round(vect[i]);j--){
                        //cout<<"["<< i-1 <<","<< j<<"], ";
                        Pts.push_back(points());
                        Pts[counter].x_axis=i-1;
                        Pts[counter].y_axis=j;
                        counter++;
                    }
                }

                if(vect[i-1]<vect[i]){//roste
                    for(int j=round(vect[i-1])+1;j<vect[i];j++){
                        //cout<<"["<< i <<","<< j<<"], ";
                        Pts.push_back(points());
                        Pts[counter].x_axis=i;
                        Pts[counter].y_axis=j;
                        counter++;
                    }
                }
        }
        //cout<<"["<< i<< ","<< vect[i]<<"], ";
        Pts.push_back(points());
        Pts[counter].x_axis=i;
        Pts[counter].y_axis=vect[i];
        counter++;
    }
    //cout<<endl;
}

/*
 * @brief   jednoduchy vypis hodnot bodu
 * @param   Pts vektor s body
*/
void Read_points(vector<points> &Pts){
    for(int i=0; i<Pts.size(); i++){
        cout<<"["<< Pts[i].x_axis<< ","<< Pts[i].y_axis<<"], ";

    }
}
/*
 * @brief   hleda bod ve vektoru podle zadanych souradnic
 * @param   Pts - vektor s body
 * @param   x, y - souradnice bodu
 * @param   reverse - jestli se hleda od pocatku vektoru nebo od konce - body se mohou vyskytovat vicekrat
 * reverse==false - prohledava od predu, true - prohledava od zadu
 * @return  int poradi v vektoru, -1 pokud bod v vektoru neni
*/
int Find_point(vector<points> Pts, int x, int y, bool revers){
    if(revers==false){
        for(int i=0; i<Pts.size(); i++){
            if((Pts[i].x_axis==x)&&(Pts[i].y_axis==y)){
                return i;
            }
        }
        return -1;
    }
    else{
        for(int i=Pts.size(); i>0; i--){
            if((Pts[i].x_axis==x)&&(Pts[i].y_axis==y)){
                return i;
            }
        }
        return -1;

    }
}

/*
 * @brief   pocita vektor smernice tecny v bode
 * @param   Pts vektor s body
 * @param   x, y - souradnice bodu
 * @param   jestli se hleda od pocatku vektoru nebo od konce - body se mohou vyskytovat vicekrat
 * @param   pointer struktura pro ulozeni vysledku
 * @note    x nabyva hodnot 0, 1, 2
*/
void Slope(vector<points> Pts, int x, int y, bool revers, points &pointer){
    int place=Find_point(Pts, x, y,revers);
    if(place==-1){
            //handle error
    }
    else{
        if(place==0){
                pointer.x_axis=Pts[place+1].x_axis-Pts[place].x_axis+1;
                pointer.y_axis=Pts[place+1].y_axis-Pts[place].y_axis;
            }
        else if(place==Pts.size()-1){
                pointer.x_axis=Pts[place].x_axis-Pts[place-1].x_axis+1;
                pointer.y_axis=Pts[place].y_axis-Pts[place-1].y_axis;
            }
        else{
                 pointer.x_axis=Pts[place+1].x_axis-Pts[place-1].x_axis;
                pointer.y_axis=Pts[place+1].y_axis-Pts[place-1].y_axis;
            }

        if((pointer.x_axis==0)&&(pointer.y_axis==0)){
                pointer.x_axis=1; //muze se stat "vrcholek veze", kdy predchozi a nasledujici body jsou stejne. smernice vrcholu veze je vodorovna, protoze se jedna o lokalni maximum (minimum nenastane)
            }
    }

}

/*
 * @brief   pocita vektor smernice tecny v bode
 * @param   Pts vektor s body
 * @param   PointNum poradi bodu ve vektoru
 * @param   pointer struktura pro ulozeni vysledku
 * @note    x nabyva hodnot 0, 1, 2
*/
void Slope(vector<points> Pts, int PointNum, points &pointer){
    if((PointNum<0)||(PointNum>Pts.size()-1)){
           throw std::invalid_argument( "error in argument PointNum, must be in range of 0 to vector size-1" );
    }
    else{
        if(PointNum==0){
                pointer.x_axis=Pts[PointNum+1].x_axis-Pts[PointNum].x_axis+1;
                pointer.y_axis=Pts[PointNum+1].y_axis-Pts[PointNum].y_axis;
            }
        else if(PointNum==Pts.size()-1){
                pointer.x_axis=Pts[PointNum].x_axis-Pts[PointNum-1].x_axis+1;
                pointer.y_axis=Pts[PointNum].y_axis-Pts[PointNum-1].y_axis;
            }
        else{
                 pointer.x_axis=Pts[PointNum+1].x_axis-Pts[PointNum-1].x_axis;
                pointer.y_axis=Pts[PointNum+1].y_axis-Pts[PointNum-1].y_axis;
            }

        if((pointer.x_axis==0)&&(pointer.y_axis==0)){
                pointer.x_axis=1; //muze se stat "vrcholek veze", viz vyse
            }
    }

}


/*
 * @brief   generuje vektor s komponentami smernic bodu
 * @param   Pts  vstupni vektor s body
 * @return  vektor s vysledky
*/
vector<points> GenerateSlopeVector(vector<points> Pts){
    vector<points> helper;
    points h;
for(int i=0;i<Pts.size();i++){
    Slope(Pts,i,h);
    helper.push_back(h);
    }
    return helper;
}

/*
 * @brief   pocita vektor paprsku ze Slunce
 * @param   height astronomicka vyska Slunce
 * @param   azimuth astronomicky azimut Slunce
 * @param   vect struktura pro ulozeni vysledku
*/
void heightToFc(double height, double azimuth, points &vect){
if((0<=height)&&(height<=90)&&(0<=azimuth)&&(azimuth<=360)){
    double temp;
    vect.y_axis=-1;//paprsky Slunce jdou vzdy shora dolu
    if(height==90){//Slunce v nadhlavniku
            vect.x_axis=0;
    }
    else if(azimuth<=180){//Slunce na vychode
            temp=1/tan(height*M_PI/180);//prevod na radiany
            vect.x_axis=-temp;
    }
    else{//slunce na zapade
            temp=1/tan(height*M_PI/180);
            vect.x_axis=temp;
   }
}
else throw std::invalid_argument( "error in arguments of heightToFc" );
}

/*
 * @brief   uhel mezi povrchem a paprsky Slunce
 * @param   u1, u2 x a y komponenty smernice povrchu
 * @param   v1, v2 x a y komponenty smernice paprsku slunce
 * @return  uhel v radianech
*/
double angle(double u1, double u2, double v1, double v2){
    return M_PI-acos((u1*v1+u2*v2)/(sqrt(u1*u1+u2*u2)*sqrt(v1*v1+v2*v2)));//acos vraci ten vetsi uhel
}
/*
 * @brief   otoci vektor o uhel proti smeru hod.rucicek
 * @param   x,y komponenty vstupniho vektoru
 * @param   tgt  vystupni vektor (muze byt stejny jako vstupni)
 * @param   angle uhel v radianech
*/
void rotate_Counterclockwise(double x, double y, points &tgt, double angle){
    tgt.x_axis=x*cos(2*angle)-y*sin(2*angle); //x'=x*cos(2*theta)-y*sin(2*theta)
    tgt.x_axis=x*sin(2*angle)+y*cos(2*angle); //y'=x*sin(2*theta)+y*cos(2*theta)
}


/*
 * @brief   normala smernice
 * @param   x,y komponenty vstupniho vektoru
 * @param   tgt  vystupni vektor (muze byt stejny jako vstupni)
 * @note    y musi byt kladna, aby ukazovala nad povrch
*/
void normal(double x, double y, points &tgt){
    tgt.x_axis=-y;
    tgt.y_axis=x;
    if(tgt.y_axis<0){
        tgt.y_axis=-tgt.y_axis;
        tgt.x_axis=-tgt.x_axis;
    }
}

/*
 * @brief   otoci vektor o uhel po smeru hod.rucicek
 * @param   x,y komponenty vstupniho vektoru
 * @param   tgt  vystupni vektor (muze byt stejny jako vstupni)
 * @param   angle uhel v radianech
*/
void rotate_clockwise(double x, double y, points &tgt, double angle){
    //v podstate stejne jako counterclockwise, ale o negativni uhel
    tgt.x_axis=x*cos(-2*angle)-y*sin(-2*angle);
    tgt.x_axis=x*sin(-2*angle)+y*cos(-2*angle);
}

/*
 * @brief   x a y komponenty vektoru paprsku odrazeneho v bode pointNum
 * @param   pts vektor bodu
 * @param   pointnum poradi bodu ve vektoru
 * @param   ray_x, ray_y x a y komponenty paprsku
 * @param   reflection struktura pro ulozeni vysledku
*/
void reflectVector(vector<points> Pts, int pointnum, double ray_x, double ray_y, points &reflection){
    points hitPoint;
    Slope(Pts, pointnum,hitPoint);
    //cout<<endl<<"Slope: ["<<hitPoint.x_axis<<","<<hitPoint.y_axis<<"]"<<endl;
    if(hitPoint.x_axis==0){
        /*vodorovna plocha, preklopit vodorovne
        *  \ |/   vysledek
        *-------   ^
        *    |\ posunuty vektor
        */
        reflection.x_axis=ray_x;
        reflection.y_axis=-ray_y;
    }
        else if(hitPoint.y_axis==0){
        /*svisla plocha, preklopit svisle
        *    |/
        *    |---
        *   /|\ < vysledek
        *  ^
        *posunuty vektor
        */
        reflection.x_axis=-ray_x;
        reflection.y_axis=ray_y;
    }
    else{
    double alpha =angle(hitPoint.x_axis, hitPoint.y_axis, ray_x, ray_y);
    cout<<"angle: "<<alpha/M_PI*180<<endl;
    double compare=-hitPoint.y_axis/hitPoint.x_axis;//-y/x -pomer normaly
    if(compare<0){//v bode roste, pokud -y/x<0 pak x/y>0
            if(ray_x/ray_y<compare){//jde seshora nad normalou
                rotate_clockwise(ray_x, ray_y,reflection,alpha);
            }
            else{
               rotate_Counterclockwise(ray_x, ray_y,reflection,alpha);
            }
        }
    else{//v bode klesa, nuly jsou odchyceny predem
            if(ray_x/ray_y>compare){//jde seshora nad normalou
                rotate_Counterclockwise(ray_x, ray_y,reflection,alpha);
            }
            else{
                rotate_clockwise(ray_x, ray_y,reflection,alpha);
            }
        }
    }
}

/*
 * @brief   hleda kam dopadne odraz z bodu PointNum v poli pts
 * @param   Pts vektor s body
 * @param   PointNum poradi bodu ve vektoru
 * @param   vect vektor, kterym se odrazi svetlo z daneho bodu
 * @return  poradi bodu dopadu v vektoru bodu
*/
int refractTo(vector<points> Pts, int PointNum, points &vect){
    double c;
    //    l   (-1)*(-1)=1       x     -      k           y
    c=vect.y_axis*Pts[PointNum].x_axis-vect.x_axis*Pts[PointNum].y_axis; //c=-ax-bx ze vzorce ax+by+c=0, vektor (k,l) ma rovnici -l*x+k*y+c
    //zname linearni funkci
    //cout<<vect.y_axis<<"x+"<<vect.x_axis<<"y+"<<c<<"=0";
    //cout<<"["<<Pts[PointNum].x_axis<<", "<<Pts[PointNum].y_axis<<"]"<<endl;
    double ypsilon;
    if(vect.x_axis==0){
        if(vect.y_axis>0){
            return -1;//paprsek nic nemuze trefit a leti do nebe
        }
        else return PointNum+1; //kdyby nahodou, rozhodne to schyta dalsi blocek,
    }
    else if(vect.x_axis>0){//vektor smeruje "po smeru"
        for(int i=PointNum+1; i<Pts.size(); i++){
                    //     (   l   (-1)*(-1)=1     x     )      k           y
            ypsilon=(vect.y_axis*Pts[PointNum].x_axis-c)/vect.x_axis; //y=(-ax-c)/b  dosadime x, pokud je to hledany bod, bude vysledek sedet na y
            ypsilon=ypsilon-Pts[i].y_axis;//hledame bod z vektoru, ktery by odpovidal rovnici (po zaokrouhleni, jsme v plovouci radove carce)
            if((-0.5<ypsilon)&&(ypsilon<=0.5)){
                return i;
            }
        }
        return -1;//paprsek nic netrefil a leti do nebe
    }
    else if(vect.x_axis<0){//vektor smeruje "proti smeru"
        for(int i=PointNum-1; i<Pts.size(); i--){
                    //     (   l   (-1)*(-1)=1     x     )      k           y
            ypsilon=(vect.y_axis*Pts[PointNum].x_axis-c)/vect.x_axis; //y=(-ax-c)/b  dosadime x, pokud je to hledany bod, bude vysledek sedet na y
            ypsilon=ypsilon-Pts[i].y_axis;//hledame bod z vektoru, ktery by odpovidal rovnici (po zaokrouhleni, jsme v plovouci radove carce)
            if((-0.5<ypsilon)&&(ypsilon<=0.5)){
                return i;
            }
        }
        return -1;//paprsek nic netrefil a leti do nebe
    }

}


/*
 * @brief   urci viditelne body v Pts z pohledu Pts[PointNum]
 * @param   Pts vektor s body
 * @param   PointNum poradi bodu ve vektoru
 * @param   Reslt vektor pro ulozeni vysledku
*/
void getVisible(vector<points> Pts, int PointNum,vector<points> Slopes, vector<int> &Reslt){

    //cout<<"visible points:";
    points normala;
    normala.x_axis=Slopes[PointNum].x_axis;
    normala.y_axis=Slopes[PointNum].y_axis;
    //cout<<"normala=["<<normala.x_axis<< ","<< normala.y_axis<<"], ";
    //doprava
    for(int i=PointNum+1; i<Pts.size(); i++){
        double deltaX=Pts[PointNum].x_axis-Pts[i].x_axis;
        double deltaH=Pts[PointNum].y_axis-Pts[i].y_axis;

        double test=deltaH-deltaX*Slopes[i].y_axis/Slopes[i].x_axis;

        double cosangle=deltaH-deltaX*normala.y_axis/normala.x_axis;//skalarni soucin normály v bode x s vektorem p
        //cout<< "p=["<< deltaX<< ","<< deltaH<<"], ";
        //cout<<"cos: "<<cosangle<<" test: "<<test<<endl;
        if((Pts[i].y_axis>=Pts[PointNum].y_axis)&&(Slopes[i].y_axis<=0)){//najdeme lokalni maximum vyssi nez bod, za nej uz neuvidi
                break;}
        if((test>0)&&(cosangle<0)){//uhel mezi normalou v bode x a vektorem p musi byt v rozmezi 0-90 nebo 270-360, tedy v kladnych castech kosinu
                //cout<<PointNum<<":["<<Pts[i].x_axis<<","<<Pts[i].y_axis<<"],";
                Reslt.push_back(i);
        }
    }
    //doleva
    for(int j=PointNum-1; j>=0; j--){
        double deltaX=Pts[PointNum].x_axis-Pts[j].x_axis;
        double deltaH=Pts[PointNum].y_axis-Pts[j].y_axis;
        double test=deltaH-deltaX*Slopes[j].y_axis/Slopes[j].x_axis;

        double cosangle=deltaH-deltaX*normala.y_axis/normala.x_axis;//skalarni soucin normály v bode x s vekotrem p
        //cout<< "p=["<< deltaX<< ","<< deltaH<<"], ";
        //cout<<j<<" cos: "<<cosangle<<" test: "<<test<<endl;
        if((Pts[j].y_axis>=Pts[PointNum].y_axis)&&(Slopes[j].y_axis>=0)){//najdeme lokalni maximum vyssi nez bod, za nej uz neuvidi
                break;}
        if((test>0)&&(cosangle<0)){
                //cout<<PointNum<<":["<<Pts[j].x_axis<<","<<Pts[j].y_axis<<"],";
                Reslt.push_back(j);
        }
    }
}

/*
 * @brief   urci rychlost ablace Pts[PointNum]
 * @param   Pts vektor s body
 * @param   PointNum poradi bodu ve vektoru
 * @return  double výsledek vzorce modelu (delta h/ delta t), tedy zmenu vysky za cas bodu Pts[PointNum]
*/
double getAblation(vector<points> Pts, int PointNum, vector<points> Slopes){
    vector<int> popis;
    getVisible(Pts,PointNum,Slopes,popis);
    double Iota=0;
    double deltaH, deltaX;
    points derivate;
    //v popisu jsou nyni body viditelne z Pts[PointNum]
    for(int i=0;i<popis.size();i++){
        deltaX=Pts[PointNum].x_axis-Pts[popis[i]].x_axis;
        deltaH=Pts[PointNum].y_axis-Pts[popis[i]].y_axis;

        //spocitat delty
        derivate.x_axis=Slopes[PointNum].x_axis;
        derivate.y_axis=Slopes[PointNum].y_axis;
        if(derivate.x_axis==0){
            derivate.x_axis=0.1;//pro korektni vypocet nesmi byt derivace deleni nulou,
        }
    //cout<<PointNum<<":"<<popis[i]<<":dX="<<deltaX<<", dY="<<deltaH<<", h'(x)="<<derivate.y_axis<<"/"<<derivate.x_axis<<endl;
        Iota=Iota+(deltaH-(derivate.y_axis/derivate.x_axis)*deltaX)/(deltaX*deltaX+deltaH*deltaH);
    }
    Iota=Iota/M_PI;
    //cout<<PointNum<<":Iota="<<Iota<<endl;

    //                ergcm-2sec-1    gcm-3 cm2sec-1
    double height=-0.5*  1e6*    Iota/(7e9); //+2.5e-5
    //cout<<PointNum<<":vyska="<<height<<endl;
    return height;
}

/*
 * @brief   najde vsechny body s odpovidajici x-ovou slozkou
 * @param   Pts vektor s body
 * @param   c hledana x-ova slozka
 * @return  vektor s cisly bodu, co maji odpovidajici x-ovou souradnici
*/
vector<int> getIndex(vector<points> Pts, int c)
{
    vector<int> vecky;
    for (int i = 0; i != Pts.size(); i++){
        if (Pts[i].x_axis == c){
            vecky.push_back(i);
        }
    }
    return vecky;
}


/*
 * @brief  pocita vysku pro body na povrchu
 * @param  Pts vektor s body
 * @param  Slopes vektor se smernicemi jednotlivych bodu
 * @return vektor, kde kazde x ma prave jednu vysku, o kterou se bude snizovat
*/
vector<double> getHeight(vector<points> Pts,vector<points> Slopes){
    vector<double> heights;
    vector<double> h_heights;
    vector<int> helper;
    int x_num=Pts[Pts.size()-1].x_axis;
    //pro kazdy bod vypocita getAblation
    //ulozi do stejne dlouheho pole
    double h;
    for (int i = 0; i != Pts.size(); i++){

        h=getAblation(Pts,i,Slopes);
        heights.push_back(h);
    }
    //pro kazde x najde vektory
    //pokud najde 0x, chyba
    //pokud najde 1x, vlozi na pozici x ten
    //pokud najde 2+x, spocita jejich prumer a vlozi na pozici x ten
    for(int x=0;x<=x_num;x++){
        helper=getIndex(Pts,x);
        if(helper.size()==0){
            throw std::invalid_argument( "error in getHeight, no point has this value on x-axis" );
            }
        else if(helper.size()==1){
            h_heights.push_back(heights[helper[0]]);//pro bod s danou souradnici x nese vysku ten jediny nalezeny, ktery ma vysku ulozenou v heights na pozici nalezene a ulozene do helper
            }
        else{
            double avg=0;
            for(int j=0; j<helper.size();j++){
                avg=avg+heights[helper[j]];//secist vsechny vysky u bodu na ktere odkazuje helper ()
            }
            avg=avg/helper.size();
            h_heights.push_back(avg);
            }
    helper.clear();//smazat cely obsah helperu, pro dalsi x musi byt prazdny
    }

    return h_heights;
}


/*
 * @brief  generuje novy povrch
 * @param  input vektor s vyskovou mapou
 * @param  heights vektor s vyskami urcenymi pro jednotlive body
 * @param  time doba kroku simulace
 * @return novy vyskovy vektor
*/
vector<double> setHeight(vector<double> input, vector<double> heights, double time){
    vector<double> helper;
    for(int i=0; i<input.size();i++){
        helper.push_back(input[i]-(heights[i]*time));
        //cout<<input[i]<<":"<<heights[i]<<endl;
    }
    return helper;
}

/*
 * @brief  za pomoci ASCII znaku vizualizuje teren
 * @param  Pts vektor s body
*/
void ShowSimple(vector<points> Pts){
    double testmax=Pts[0].y_axis;
    bool found=false;
    for(int i=0;i<Pts.size();i++){
        //cout<<Pts[i]<<",";
        if(testmax<Pts[i].y_axis){
            testmax=Pts[i].y_axis;
        }
    }
    //cout<<endl;
    cout<<"    ";
    for(int i=0; i<=Pts[Pts.size()-1].x_axis; i++){
        if((i>=0)&&(i<=9)){
            cout<<i;
        }
        else if((i>=10)&&(i<=35)){
            cout<<(char)(i+55);
        }
        else if((i>=36)&&(i<=61)){
            cout<<(char)(i+61);
        }
        else cout<<"%";
    }
    cout<<"\n";
    for(int y=round(testmax);y>=0;y--){
        cout<<setw(4)<<y;
        for(int x=0;x<=Pts[Pts.size()-1].x_axis;x++){
            found=false;
            for(int a=0;a<=Pts.size(); a++){
                if((round(Pts[a].y_axis)==y)&&(Pts[a].x_axis==x)){
                    found=true;
                }
            }
            if(found==true)
                cout<<"X";
            else cout<<" ";

        }
        cout<<"\n";
    }
}

/*
 * @brief  simulace jednoho rezu
 * @param  v rez vyskovou mapou
 * @param  time delka kroku v sekundach
 * @note   prubeh je vizualizovan pomoci "ASCII artu", je brzden.
*/
void RunVectorWithVisual(vector<double> &v, int time){
    vector<points> Body, smernice, res;
    vector<double> zmeny;
        active_points(v, Body);
        //Read_points(Body);
        //points test;
        //Slope(Body, 8,8,true, test);
        //cout<<endl<<"["<<test.x_axis<<","<<test.y_axis<<"]"<<endl;
        //int target;
        //points sunray;
        //reflectVector(Body,5,0,-1,sunray);
        //target=refractTo(Body,5,sunray);
        //cout<<target<<endl;
        system("CLS");
        ShowSimple(Body);
        smernice=GenerateSlopeVector(Body);
        zmeny=getHeight(Body,smernice);
        v=setHeight(v,zmeny,time);
        sleep(2);
}


/*
 * @brief  simulace jednoho rezu
 * @param  v rez vyskovou mapou
 * @param  time delka kroku v sekundach
*/
void RunVector(vector<double> &v, int time){
    vector<points> Body, smernice, res;
    vector<double> zmeny;

        active_points(v, Body);
        smernice=GenerateSlopeVector(Body);
        zmeny=getHeight(Body,smernice);
        v=setHeight(v,zmeny,time);
}



