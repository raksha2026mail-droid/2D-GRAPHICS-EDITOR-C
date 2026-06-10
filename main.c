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
void makeLine(int y,int x1,int x2){
if(y<0||y>=R)
return;
if(x1>x2)
{int t=x1;x1=x2;x2=t;}
for(int j=x1;j<=x2;j++)
if(j>=0&&j<C)
canvas[y][j]='';
}
void makeRect(int y,int x,int h,int w){
for(int j=x;j<x+w;j++){
if(y>=0&&y<R&&j>=0&&j<C)
canvas[y][j]='';
if(y+h-1>=0&&y+h-1<R&&j>=0&&j<C)
canvas[y+h-1][j]='';
}
for(int i=y;i<y+h;i++){
if(i>=0&&i<R&&x>=0&&x<C)
canvas[i][x]='';
if(i>=0&&i<R&&x+w-1>=0&&x+w-1<C)
canvas[i][x+w-1]='';
}
}
void makeTri(int y,int x,int h){
for(int i=0;i<h;i++)
for(int j=0;j<=i;j++)
if(y+i>=0&&y+i<R&&x+j>=0&&x+j<C)
canvas[y+i][x+j]='';
}
void makeCirc(int cy,int cx,int rad){
for(int y=0;y<R;y++)
for(int x=0;x<C;x++){
int dy=y-cy,dx=x-cx;
if(dydy+dxdx<=radrad)canvas[y][x]='';
}
}
void refresh(){
reset();
for(int i=0;i<total;i++){
if(!list[i].live)
continue;
if(list[i].kind==1)
makeLine(list[i].y,list[i].x,list[i].v1);
else if(list[i].kind==2)
makeRect(list[i].y,list[i].x,list[i].v1,list[i].v2);
else if(list[i].kind==3)
makeTri(list[i].y,list[i].x,list[i].v1);
else if(list[i].kind==4)
makeCirc(list[i].y,list[i].x,list[i].v1);
}
}
void show(){
printf("\n");
for(int i=0;i<R;i++)
{
for(int j=0;j<C;j++)
printf("%c",canvas[i][j]);
printf("\n");}
}
void create(){
Obj o;
printf("\n1.Line 2.Rect 3.Tri 4.Circ\nType: ");
scanf("%d",&o.kind);
o.id=nextID++;
o.live=1;
if(o.kind==1)
printf("Row StartCol EndCol: "),
scanf("%d %d %d",&o.y,&o.x,&o.v1);
else if(o.kind==2)
printf("Row Col Height Width: "),
scanf("%d %d %d %d",&o.y,&o.x,&o.v1,&o.v2);
else if(o.kind==3)
printf("Row Col Height: "),
scanf("%d %d %d",&o.y,&o.x,&o.v1);
else if(o.kind==4)
printf("Cy Cx Rad: "),
scanf("%d %d %d",&o.y,&o.x,&o.v1);
else {
printf("Bad type!\n");
return;}
list[total++]=o;
refresh();
printf("Done! ID=%d\n",o.id);
}
void removeObj(){
int id,hit=0;
printf("ID: ");
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
else if(list[i].kind==2)
printf("New Row Col H W: "),
scanf("%d %d %d %d",&list[i].y,&list[i].x,&list[i].v1,&list[i].v2);
else if(list[i].kind==3)
printf("New Row Col H: "),
scanf("%d %d %d",&list[i].y,&list[i].x,&list[i].v1);
else if(list[i].kind==4)
printf("New Cy Cx Rad: "),
scanf("%d %d %d",&list[i].y,&list[i].x,&list[i].v1);
refresh();
printf("Updated!\n");break;
}
}
if(!hit)
printf("No ID found\n");
}
int main(){
int op;
reset();
do{
printf("\n1.Add 2.Show 3.Del 4.Edit 5.List 6.Wipe 7.Quit\nChoice: ");
scanf("%d",&op);
if(op==1)create();
else if(op==2)show();
else if(op==3)removeObj();
else if(op==4)edit();
else if(op==5)
{for(int i=0;i<total;i++)
if(list[i].live)
printf("ID:%d Type:%d\n",list[i].id,list[i].kind);}
else if(op==6){total=0;
reset();
printf("Cleared!\n");}
}while(op!=7);
return 0;
}
