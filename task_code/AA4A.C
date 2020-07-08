#include <stdio.h>       
#include <stdlib.h>
#include <string.h> /*for memcpy command*/
#include "movieobj.h"
#include "video.h"
#include <mem.h>
#include "exptlib.h"
#include <math.h>

#define status STAND_ALONE
#define fastms 200
#define numletters 8
#define numaddends 5
#define numsums 3
#define WRONG 0
#define RIGHT 1
#define RESPTRUE 'z'
#define RESPFALSE '/'
#define numrepeated 10
#define numreps 20
#define numnoveltrials ((numletters*numaddends*numsums)-numrepeated)
#define numreptrials (numrepeated*numreps)
#define numtrials numnoveltrials+numreptrials
#define numstimuli (numletters*numaddends*numsums)
#define sumone (int)(1-(floor(numsums/2)+1))

#define fore_time 30

int checkrepeated(int repeated[]);
char * convertid(int id);
int testtrue(int id);
int testrepeat(int id, int repeats[]);
int getaddend(int id);
int getletter(int id);
int getsum(int id);
void rtexit(FILE *outfile);

int main(){
 int sex;
 double r;
 //long seed=0;
 int allstim[numstimuli];
 int i=0,j=0,k=0,problem=0;
 int repeatedstimuli[numrepeated];
 int novelstimuli[numnoveltrials];
 int reptrials[numreptrials];
 int alltrials[numtrials];
 char *stimulus;
 char stimstring[5];
 int stimcounter[numstimuli];  
 int cor;
 int rand;
 int truerepeats[numaddends];
 int falserepeats[numaddends];
 int rtwarnings=0;
  
  // Jeff's stuff
  logtype *log;
  FILE *outfile,*backfile;
  char *fontlist,junk[3];
  char backstr[40],outline[80];
  long seed;

  image *ready_i,*wrong_i,*blank, *targ_i, *stim_i;
  image *fast_i;
  movie *fast_m;
  movie *wrong_m,*ready_m,*stim_m,*feed_m;
  image *ready13_i;
  response *data,*data2; int resp,second; long RT;
  
  fontlist=getenv("FONTLIST");
  log=(logtype *)malloc(1*sizeof(logtype));
  gen_init(status,log);
  SetupMoviePackage(fontlist);
  seed=log->seed;
  outfile=fopen(log->outfn,"wt");
  
  if ((log->subjnum)>499){
        sex=0;
        }
  else{
        sex=1;
  }
  /* set up pallete here*/
   makePalette(GRAYSCALE);
   Palette[0].r=0;Palette[0].g=0;Palette[0].b=107;
  /*blue*/
   Palette[253].r=255;Palette[253].g=255;Palette[253].b=0;
  /*Yellow*/
  

    blank=newImage();
  /*READY movie*/
    ready_i=newImage();
  downloadImage(ready_i);
  drawText("ready?",0,0,0,253);
  uploadImage(ready_i);
  ready_m=initMovie(1);
  setMovie(ready_m,0,ready_i,180);
    
   /*too fast movie*/
    fast_i=newImage();
  downloadImage(fast_i);
  drawText("Too fast",0,0,0,253);
  uploadImage(fast_i);
  fast_m=initMovie(1);
  setMovie(fast_m,0,fast_i,1);

  /*WRONG movie*/
  wrong_i=newImage();
  downloadImage(wrong_i);
  drawText("invalid key",0,0,0,253);
  uploadImage(wrong_i);
  wrong_m=initMovie(1);
  setMovie(wrong_m,0,wrong_i,1);
  //end Jeff's stuff


 
 for (i=0;i<numaddends;i++) {truerepeats[i]=0;falserepeats[i]=0;} //initializing to 0s
 for (i=0;i<numstimuli;i++) stimcounter[i]=0; //initializing to 0s
 
 for (i=0;i<numstimuli;i++){
        allstim[i]=i;        //creating all stimuli
 }
 
 //the next loop picks the repeated and novel, and checks to make sure
 //each addend is picked evenly for the repeated

while(!(checkrepeated(truerepeats))){
for (i=0;i<numaddends;i++){
        rand=randint(0,numletters-1,&seed);
        truerepeats[i]=((0-sumone)*numaddends*numletters)+(numaddends*rand)+i;
}
}
while(!(checkrepeated(falserepeats))){
for (i=0;i<numaddends;i++){
        rand=randint(0,((numsums-1)*numletters)-1,&seed);
        if (rand>((-sumone*numletters)-1)) rand+=numletters;
        falserepeats[i]=(numaddends*rand)+i;
}
}
for (i=0;i<numrepeated;i++){
        if (i<5){
        repeatedstimuli[i]=truerepeats[i];}
        else{
        repeatedstimuli[i]=falserepeats[i-5];}
}
i=0;
        for (j=0;j<numstimuli;j++){
                problem=0;
                for (k=0;k<numrepeated;k++){
                        if (j==repeatedstimuli[k]) problem=1;
                }
                if (!(problem)){
                        novelstimuli[i++]=j;
                }
        }



 //repeat the repeated trials
 for (i=0;i<numreptrials;i++) reptrials[i]=repeatedstimuli[i%numrepeated];
 
 //add all trials to the mix and distribute
 for (i=0;i<numtrials;i++){
        if (i<numreptrials) {
                alltrials[i]=reptrials[i];
        }else{
                alltrials[i]=novelstimuli[i-numreptrials];
        }
 }
 distribute(alltrials,numtrials, &seed);

 stim_m=initMovie(2);
 blank=newImage();
 stim_i=newImage();
 setMovie(stim_m,0,blank,fore_time);

 //start running trials
 runMovie(ready_m,UNTIL_RESPONSE,1);
 for (i=0;i<numtrials;i++){
        stimulus=convertid(alltrials[i]);
        sprintf(stimstring,"%c%c%c%c%c",
              stimulus[0],stimulus[1],stimulus[2],stimulus[3],stimulus[4]);
 stimcounter[alltrials[i]]++; 

  downloadImage(stim_i);
  clearPicBuf();
  drawText(stimstring,xcenter-30,ycenter-30,0,253);
  uploadImage(stim_i);
  setMovie(stim_m,1,stim_i,1);
          data=runMovie(stim_m, UNTIL_RESPONSE,1);

          switch (data->x[0].resp){
          case RESPTRUE: resp=1;break;
          case RESPFALSE: resp=0;break;
         case '@': resp=10;break;
          default : resp=11;audio(INVALID, wrong_m);break;}
          RT=data->x[0].rt-(msperframe*fore_time);
          if (resp==testtrue(alltrials[i]))
            {
            audio(CORRECT);
            cor=1;
           }
         else
            {
              audio(ERROR);
              cor=0;
            }  
          if (resp==10) {fclose(outfile);CleanupMoviePackage();
          printf ("stopped while running by participant\n");exit(1);}

sprintf(outline, "sub%03d sex%i trl%03d let%i add%i sum%03i id%04d tru%i isr%i %04i rsp%i %i %05d\n",
       log->subjnum, sex ,i, getletter(alltrials[i]), getaddend(alltrials[i]),
       getsum(alltrials[i]), alltrials[i], testtrue(alltrials[i]), testrepeat(alltrials[i],repeatedstimuli),stimcounter[alltrials[i]],resp,cor,RT);
fprintf(outfile,"%s",outline);  

if (RT<fastms){
  rtwarnings++;
  audio(INVALID, fast_m);
  if (rtwarnings>=5) rtexit(outfile);
}

}
 //cleanup
 fclose(outfile);
 thankyou();
 CleanupMoviePackage();
 return 0;
}

int checkrepeated(int repeated[]){
 int i=0,j=0;
 int problem=0; 
 for (i=0;i<numaddends;i++){
        for (j=0;j<numaddends;j++){
                if ((j!=i)&&(repeated[i]==repeated[j])) problem=1;
        }
 }

 if (problem) {return 0;} else{return 1;}
}


char * convertid(int id){
 char stimulus[]="L+N=S";
 int addend;
 int temp;
 int letter;
 int sum;
 char charletter;
 char charsum; 
 addend=(id%numaddends)+1;
 temp=floor(id/numaddends);
 letter=(temp%numletters)+1;
 temp=floor(floor(id/numaddends)/numletters);
 sum=(temp%numsums)+sumone;
 charletter=(letter+64);
 charsum=(letter+addend+sum+64);
 sprintf(stimulus,"%c+%i=%c",charletter,addend,charsum);
 return stimulus;
}

int testtrue(int id){
 int temp=floor(floor(id/numaddends)/numletters);
 int sum=(temp%numsums)+sumone;
 return (!(sum));
}

int testrepeat(int id, int repeats[]){
 int problem=0;
 int i;
 for (i=0;i<numrepeated;i++) if (id==repeats[i]) problem=1;
 return problem;
}

int getaddend(int id){
 return (id%numaddends)+1;
}

int getsum(int id){
 int temp=floor(floor(id/numaddends)/numletters);
 return (temp%numsums)+sumone+getletter(id)+getaddend(id);
}

int getletter(int id){
 int temp=floor(id/numaddends);
 return (temp%numletters)+1;
}

void rtexit(FILE *outfile){
  image *rtexit_i;
  movie *rtexit_m;
  response *data;
  int code=0;
  rtexit_i=newImage();
  downloadImage(rtexit_i);
  clearPicBuf();
  drawText("You have had 5 too-fast responses.",0,0,0,253);
  drawText("Please see the experimenter.",0,50,0,253);
  uploadImage(rtexit_i);
  rtexit_m=initMovie(1);
  setMovie(rtexit_m,0,rtexit_i,1);
  do
   {
     data=runMovie(rtexit_m, UNTIL_RESPONSE,2);
     if ((data->x[0].resp=='x') && (data->x[1].resp=='@')) code=1;
   } while (code==0);
  fclose(outfile);
  CleanupMoviePackage();
  exit(1);
}





