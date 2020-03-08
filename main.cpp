/* ------------------------------------
*      Ondrej Cech (xcecho06), 2020
*  ------------------------------------
*/


#include <iostream>
#include "functions.h"

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"
# define resizer        300


using namespace std;

int main(int argc, char *argv[])
{
    //              0  1  2 3 4  5 6 7 8 9 10 11 12 13 14
    //vector<double> v= {4, 2, 2,2,3, 5,6,7,9,5, 3, 1, 1, 1, 3,6,8,9,8,6,3,2,1,3,5,5};
    if(argc!=5){
        cerr<<"Zadejte parametry: nazev vstupniho souboru, delka kroku v sekundach, celkovy pocet kroku a pocet kroku mezi generovanym vystupu."<<endl<<"Napriklad glac_sim.exe ./Test/Test.exr 3600 24 1"<<endl;
        return 1;
    }
    else if(atoi(argv[2])<1){
        cerr<<"Argument 2 (delka kroku) musi byt prirozene cislo (vetsi nez 0)."<<endl;
        return 1;
    }
        else if(atoi(argv[3])<1){
        cerr<<"Argument 3 (pocet kroku) musi byt prirozene cislo (vetsi nez 0)."<<endl;
        return 1;
    }
        else if(atoi(argv[4])<1){
        cerr<<"Argument 4 (pocet kroku mezi generovanym vystupu) musi byt prirozene cislo (vetsi nez 0)."<<endl;
        return 1;
    }

    string filename(argv[1]);
    if(filename.size()>4){
      //ocekava priponu ".exr", smaze ji, aby mohl editovat nazev souboru
      filename.pop_back();
      filename.pop_back();
      filename.pop_back();
      filename.pop_back();

    }

      const char* input = argv[1];//jmeno vstupniho souboru
      float* out; // width * height * RGBA
      int width;
      int height;
      const char* err = nullptr;
      int ret = LoadEXR(&out, &width, &height, input, &err);
      if (ret != TINYEXR_SUCCESS) {
        if (err) {
           fprintf(stderr, "ERR : %s\n", err);
           FreeEXRErrorMessage(err); // release memory of error message.
        }
      }
      else {
      //ted je obrazek v dlouhem poli floatu out[width * height * RGBA]
      //nacita po radcich

      vector<double> cut(width);
      vector<vector<double> >   hmap(height,cut);


     for(int y=0; y<height; y++){
        for(int x=0;x<width*4; x=x+4){
            /*
            cout<<"radek:"<<y<<"sloupec"<<x/4<<endl;
            cout<<"R: "<<out[(y*width*4)+x]<<endl;
            cout<<"G: "<<out[(y*width*4)+x+1]<<endl;
            cout<<"B: "<<out[(y*width*4)+x+2]<<endl;
            cout<<"A: "<<out[(y*width*4)+x+3]<<endl;
            */
            //viditelne kanaly v rozsahu 0-1, ale umoznuje uchovat i dalsi (jen jak je tam dostat?)
            //jeden z nich vezme a ulozi do matice reprezentujici obrazek (pocita s sedym vstupem, tedy vsechny hodnoty RGB stejne)
            hmap[y][x/4]=out[(y*width*4)+x+1]*resizer;

        }
      }
      free(out); // uvolnit data obrazku
    //cout<<"nacteno " <<endl;
    int outputtime=atoi(argv[4]);
    //counter pocita kroky simulace, numerator generovane obrazky
    int numerator=0, counter=0;




    for(int i=0;i<atoi(argv[3]);i++){//kroky zadane parametrem


      //prochazi vodorovne
      for(int t=0;t<height;t++){
            RunVector(hmap[t],atoi(argv[2]));//cas dany parametrem
            //cout<<"runvector "<<t<<endl;
        }
        //prochazi svisle
        for(int iter=0;iter<width;iter++){
            vector<double>helpvector;
            getVertical(hmap,width,height,iter,helpvector);
            RunVector(helpvector,atoi(argv[2]));//cas dany parametrem
            saveVertical(hmap,width,height,iter,helpvector);
        }


        counter++;
        if(counter==outputtime){
            counter=0;
            numerator++;

            //jmeno vygenerovaneho souboru
            stringstream strs;
            strs << filename<<"_"<<numerator<<".exr";
            string temp_name = strs.str();
            char name[temp_name.size()+1];
            temp_name.copy(name,temp_name.size()+1);
            name[temp_name.size()]='\0';
            //musi byt formatu char*
            const char *err = nullptr;

            //vysledek simulace musi byt zase v podobe jednorozmerneho pole floatu
            vector<double> concatenated;
            for(int y=0; y<height; y++){
                for(int x=0;x<width; x=x++){
                        for(int a=0; a<3;a++){
                            concatenated.push_back(hmap[y][x]/resizer);
                        }

                }
            }



            //konverze na floaty
            vector<float> final(concatenated.begin(),concatenated.end());
            cout<<"ukladani "<< name<<endl;
            int ret= SaveEXR(final.data(), int(width), int(height), /* channels */3, /* fp16? */0, name, &err);
            if (ret) {
                if (err) {
                    cerr << err << std::endl;
                    FreeEXRErrorMessage(err);
                    }
                cerr << "failed to write" << std::endl;
                }
            }
      }

    }
    return 0;
}
