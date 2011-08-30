#include "k.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
//https://code.kx.com/trac/wiki/Cookbook/InterfacingWithC

int c=0;

int main()

{
K y,d;
K r;
int e=0;
char example[10];


printf("Which example do you want to run?\n");

c=khp("localhost",9001);


scanf( "%s", &example );
printf( "Example Chosen was %s\n", example );
//example='eg1';
if( strcmp(example,"eg1")==0)(eg1());
if( strcmp(example,"eg2")==0)(eg2());
if( strcmp(example,"eg3")==0)(eg3());
if( strcmp(example,"eg4")==0)(eg4());
if( strcmp(example,"eg5")==0)(eg5());
if( strcmp(example,"eg6")==0)(eg6());
if( strcmp(example,"eg7")==0)(eg7());
if( strcmp(example,"eg8")==0)(eg8());
printf( "Finished example run. Exiting. Hope you had a nice time." );
kclose(c);

return 0;

}

//Simple Hello World
int eg1()
{
printf( "Executing Example 1 - Sending a simple message [Synch].");

k(c,"0N!\"Hello World\"",(K)0);

return 1;

}

//Simple asynch message setting a variable
int eg2()
{
time_t t;
char buf[50],msg[50];
time(&t);
itoa(t, buf,10);
strcpy(msg,"a:");
printf( "Executing Example 1 - Sending a simple message [Asynh].");
printf(strcat(msg,buf));
k(-1*c,msg,(K)0);

return 1;

}

//Calling functions with different valencies and with simple data types
int eg3()
{
K result=NULL;


//Monadic [Single argument function]
result=k(c,"f1s",ks("Hello World"),(K)0);
printf( "Executing f1s - This will return a symbol [type=%i] with value %s\n\n\n",result->t,result->s);

//Dyadic function
result=k(c,"f2i",ki(10),ki(1),(K)0);
printf( "Executing f2s - This will return a integer [type=%i] with value %i\n\n\n",result->t,result->i);

//Triadic function
result=k(c,"f3f",kf(10),kf(-1.),kf(2.2),(K)0);
printf( "Executing f2s - This will return a float [type=%i] with value %f\n\n\n",result->t,result->f);


//The examples above can be extended to up to functions with eight arguments.
//[Errors are discussed later]

printf("Finished Example 3");

return 1;

}

//Executing a simple text based query
int eg4()
{
K result=NULL;


//Monadic [Single argument function]
result=k(c,"select from t1",(K)0);
printf( "Executing f1s - This will return a table [type=%i]",result->t);


return 1;

}

//Createing simple vectors
int eg5()
{

//Create a set of vectors of differing types each of length 5.
int i=0,l=5;
K vsymbol=ktn(KS,l);
K vint=ktn(KI,l);
K vfloat=ktn(KF,l);
K vdate=ktn(KD,l);
K vtime=ktn(KT,l);
K vdatetime=ktn(KZ,l);
K vtimestamp=ktn(KP,l);
for(i=0;i<l;i++)
{

kS(vsymbol)[i]=ss("w");
kI(vint)[i]=i;
kF(vfloat)[i]=i+0.0;
kI(vdate)[i]=i;
kI(vtime)[i]=i;
kF(vdatetime)[i]=i+0.1*i;
kJ(vtimestamp)[i]=i;
}

k(c,"display",vint,(K)0);
k(c,"display",vsymbol,(K)0);
k(c,"display",vfloat,(K)0);
k(c,"display",vdate,(K)0);
k(c,"display",vtime,(K)0);
k(c,"display",vdatetime,(K)0);
k(c,"display",vtimestamp,(K)0);

return 1;

}

//create ,pass and recieve a simple dictionary. [Note Dictionary will be
// homogenous. This demo gives an idea on how to deal withthe heterogenous
// case.]
int eg6()
{
int i=0,l=2;
K keys=ktn(KS,l);
K vals=ktn(KI,l);
K dict;
K result,resultk,resultv;
//A dictionary is a mapping of two conforming lists.
kI(vals)[0]=1; kI(vals)[1]=2;
kS(keys)[0]=ss("key1"); kS(keys)[1]=ss("key2");
// Turn these into a dictionary
dict=xD(keys,vals);
result=k(c,"{[x]x[`key3]:`int$.z.t;show x;x}",dict,(K)0);

printf( " This return has [type=%i] and contains %i enclosed elements\n\n\n",result->t,result->i);
//Extract the keys and value vectors from the k bject
resultk=kK(result)[0];
resultv=kK(result)[1];
//extract the lenth of the vectors
l=resultk->n;
printf( " The returned dictionary has %i elements.\n",l);
for(i=0;i<l;i++)
{
printf( " The %i key is %s \n",i,kS(resultk)[i]);
printf( " The associated value is %i \n",kI(resultv)[i]);
}

return 1;
}

//create ,pass and recieve a simple table.
int eg7()
{
K cc,d,e,v,tab;
K flip,result,columnNames,columnData;
int row,col,nCols,nRows;

cc=ktn(KS,2);kS(cc)[0]=ss("pid");kS(cc)[1]=ss("uid");
d=ktn(KS,3);kS(d)[0]=ss("ibm");kS(d)[1]=ss("gte");kS(d)[2]=ss("kvm");
e=ktn(KI,3);kI(e)[0]=1;kI(e)[1]=2;kI(e)[2]=3;
v=knk(2,d,e);
tab=xT(xD(cc,v));

flip=k(c,"{[x]a:update t:.z.t,y:.z.d from x;.tst.t:a;a}",tab,(K)0);

//Turn into a dictionary. flip->k [transpose?]
//Display table. [Borrowed from code.kx.com: 
// https://code.kx.com/trac/attachment/wiki/Cookbook/InterfacingWithC/csv.c ]
columnNames=kK(flip->k)[0];
columnData=kK(flip->k)[1];
nCols=columnNames->n;
nRows=kK(columnData)[0]->n;

for(row=0;row<nRows;row++)
{
if(0==row)
{
for(col=0;col<nCols;col++)
{
if(col>0)printf(",");
printf("%s",kS(columnNames)[col]);
}
printf("\n");
}
for(col=0;col<nCols;col++)
{
K obj=kK(columnData)[col];
if(col>0)printf(",");
switch(obj->t)
{
case(1):{printf("%d",kG(obj)[row]);}break;
case(4):{printf("%d",kG(obj)[row]);}break;
case(5):{printf("%d",kH(obj)[row]);}break;
case(6):{printf("%d",kI(obj)[row]);}break;
case(7):{printf("%lld",kJ(obj)[row]);}break;
case(8):{printf("%f",kE(obj)[row]);}break;
case(9):{printf("%f",kF(obj)[row]);}break;
case(11):{printf("%s",kS(obj)[row]);}break;
case(19):{printf("%i",kI(obj)[row]);}break;
case(14):{printf("%i",kI(obj)[row]);}break;
default:{printf("unknown type");}break;
}
}
printf("\n");
}

return 1;
}

// join an integer to a list and join a K object to a list
int eg8()
{
        K z=ki(20);
        K intlist=ktn(KI,0);
		K x=ktn(0,0);
        DO(5,ja(&intlist,&i));        
        jk(&x,intlist);
        jk(&x,z);
        k(c,"display",x,(K)0);
        return 1;
}
