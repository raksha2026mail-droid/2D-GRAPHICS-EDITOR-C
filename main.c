#include<stdio.h>
#define R 20
#define C 40
#define M 100
char canvas[R][C];
typedef struct{int id,kind,y,x,v1,v2,live;}Obj;
Obj list[M];
int total=0,nextID=1;
void reset(){
for(int i=0;i<R;i++)
  for(int j=0;j<C;j++)canvas[i][j]='_';
}

  scanf("%d",&id);
for(int i=0;i<total;i++)
  if(list[i].id==id&&list[i].live)
  {list[i].live=0;hit=1;
   break;}
if(hit)
{refresh();
 printf("Removed!\n");
}
else 
  printf("No ID found\n");
}
void edit(){
int id,hit=0;
  printf("ID: ");
  scanf("%d",&id);
for(int i=0;i<total;i++){
if(list[i].id==id&&list[i].live){
hit=1;
if(list[i].kind==1)
  printf("New Row Start End: "),
    scanf("%d %d %d",&list[i].y,&list[i].x,&list[i].v1);
