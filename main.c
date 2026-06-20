#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 20
#define MAX_OBJECTS 50

typedef enum{LINE,RECTANGLE,CIRCLE,TRIANGLE}ShapeType;

typedef struct{
    int id;
    ShapeType type;
    int params[6]; 
    int active;    
}CanvasObject;

char canvas[HEIGHT][WIDTH];
CanvasObject object_list[MAX_OBJECTS];
int next_id=1;

// 1. Clear canvas matrix back to empty
void clear_canvas(){
    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;x++){
            canvas[y][x]='_';
        }
    }
}

// 2. Plot pixel with bounds safety check
void set_pixel(int x,int y){
    if(x>=0&&x<WIDTH&&y>=0&&y<HEIGHT){
        canvas[y][x]='*';
    }
}

// 3. Bresenham line drawer
void draw_line_logic(int x0,int y0,int x1,int y1){
    int dx=abs(x1-x0),sx=x0<x1?1:-1;
    int dy=-abs(y1-y0),sy=y0<y1?1:-1;
    int err=dx+dy,e2;
    while(1){
        set_pixel(x0,y0);
        if(x0==x1&&y0==y1)break;
        e2=2*err;
        if(e2>=dy){err+=dy;x0+=sx;}
        if(e2<=dx){err+=dx;y0+=sy;}
    }
}

// 4. Rectangle outline using lines
void draw_rectangle_logic(int x,int y,int w,int h){
    if(w<=0||h<=0){
        printf("Warning: Invalid rectangle dimensions (W:%d, H:%d)\n",w,h);
        return;
    }
    draw_line_logic(x,y,x+w-1,y);             
    draw_line_logic(x,y+h-1,x+w-1,y+h-1); 
    draw_line_logic(x,y,x,y+h-1);             
    draw_line_logic(x+w-1,y,x+w-1,y+h-1); 
}

// 5. Midpoint circle algorithm with aspect fix
void draw_circle_logic(int xc,int yc,int r){
    if(r<=0){
        printf("Warning: Invalid circle radius (%d)\n",r);
        return;
    }
    int x=0,y=r;
    int d=3-2*r;
    while(y>=x){
        int ax=(int)(x*1.8);
        int ay=(int)(y*1.8);
        set_pixel(xc+ax,yc+y);
        set_pixel(xc-ax,yc+y);
        set_pixel(xc+ax,yc-y);
        set_pixel(xc-ax,yc-y);
        set_pixel(xc+ay,yc+x);
        set_pixel(xc-ay,yc+x);
        set_pixel(xc+ay,yc-x);
        set_pixel(xc-ay,yc-x);
        if(d<0){
            d=d+4*x+6;
        }else{
            d=d+4*(x-y)+10;
            y--;
        }
        x++;
    }
}

// 6. Connect three vertices
void draw_triangle_logic(int x0,int y0,int x1,int y1,int x2,int y2){
    draw_line_logic(x0,y0,x1,y1);
    draw_line_logic(x1,y1,x2,y2);
    draw_line_logic(x2,y2,x0,y0);
}

// 7. Track and add new vector primitive
int add_object(ShapeType type,int params[]){
    for(int i=0;i<MAX_OBJECTS;i++){
        if(!object_list[i].active){
            object_list[i].id=next_id++;
            object_list[i].type=type;
            object_list[i].active=1;
            int p_count=(type==TRIANGLE)?6:(type==CIRCLE?3:4);
            for(int j=0;j<p_count;j++){
                object_list[i].params[j]=params[j];
            }
            printf("Success: Added object with allocated ID %d\n",object_list[i].id);
            return object_list[i].id;
        }
    }
    printf("Error: Object database registry capacity reached!\n");
    return -1; 
}

// 8. Soft-delete object by resetting active flag
int delete_object(int id){
    for(int i=0;i<MAX_OBJECTS;i++){
        if(object_list[i].active&&object_list[i].id==id){
            object_list[i].active=0;
            printf("Success: Object ID %d removed from database.\n",id);
            return 1; 
        }
    }
    return 0; 
}

// 9. Overwrite geometry parameters
int modify_object(int id,int new_params[]){
    for(int i=0;i<MAX_OBJECTS;i++){
        if(object_list[i].active&&object_list[i].id==id){
            int p_count=(object_list[i].type==TRIANGLE)?6:(object_list[i].type==CIRCLE?3:4);
            for(int j=0;j<p_count;j++){
                object_list[i].params[j]=new_params[j];
            }
            printf("Success: Object ID %d tracking configurations modified.\n",id);
            return 1;
        }
    }
    return 0;
}

// 10. Vector translation engine
int move_object(int id,int dx,int dy){
    for(int i=0;i<MAX_OBJECTS;i++){
        if(object_list[i].active&&object_list[i].id==id){
            if(object_list[i].type==CIRCLE){
                object_list[i].params[0]+=dx;
                object_list[i].params[1]+=dy;
            }else if(object_list[i].type==TRIANGLE){
                object_list[i].params[0]+=dx;object_list[i].params[1]+=dy;
                object_list[i].params[2]+=dx;object_list[i].params[3]+=dy;
                object_list[i].params[4]+=dx;object_list[i].params[5]+=dy;
            }else{
                object_list[i].params[0]+=dx;
                object_list[i].params[1]+=dy;
                if(object_list[i].type==LINE){
                    object_list[i].params[2]+=dx;
                    object_list[i].params[3]+=dy;
                }
            }
            printf("Success: Shifted Object ID %d by delta translation (%d,%d).\n",id,dx,dy);
            return 1;
        }
    }
    return 0;
}

// 11. Redraw and refresh screen output
void display_picture(){
    clear_canvas();
    for(int i=0;i<MAX_OBJECTS;i++){
        if(object_list[i].active){
            CanvasObject obj=object_list[i];
            if(obj.type==LINE) 
                draw_line_logic(obj.params[0],obj.params[1],obj.params[2],obj.params[3]);
            else if(obj.type==RECTANGLE) 
                draw_rectangle_logic(obj.params[0],obj.params[1],obj.params[2],obj.params[3]);
            else if(obj.type==CIRCLE) 
                draw_circle_logic(obj.params[0],obj.params[1],obj.params[2]);
            else if(obj.type==TRIANGLE) 
                draw_triangle_logic(obj.params[0],obj.params[1],obj.params[2],obj.params[3],obj.params[4],obj.params[5]);
        }
    }
    printf("\033[H\033[J"); 
    printf("   ");
    for(int x=0;x<WIDTH;x++)printf("%d",x%10);
    printf("\n");
    for(int y=0;y<HEIGHT;y++){
        printf("%2d ",y); 
        for(int x=0;x<WIDTH;x++){
            printf("%c",canvas[y][x]);
        }
        printf("\n");
    }
}

// Dump active shape metadata below canvas
void list_meta_data(){
    printf("\n--- Active Objects ---\n");
    int blank=1;
    for(int i=0;i<MAX_OBJECTS;i++){
        if(object_list[i].active){
            blank=0;
            printf("ID %d -> ",object_list[i].id);
            if(object_list[i].type==LINE) 
                printf("Line: (%d,%d) to (%d,%d)\n",object_list[i].params[0],object_list[i].params[1],object_list[i].params[2],object_list[i].params[3]);
            else if(object_list[i].type==RECTANGLE) 
                printf("Rect: (%d,%d) W:%d H:%d\n",object_list[i].params[0],object_list[i].params[1],object_list[i].params[2],object_list[i].params[3]);
            else if(object_list[i].type==CIRCLE) 
                printf("Circle: Center(%d,%d) Rad:%d\n",object_list[i].params[0],object_list[i].params[1],object_list[i].params[2]);
            else if(object_list[i].type==TRIANGLE) 
                printf("Triangle: A(%d,%d), B(%d,%d), C(%d,%d)\n",object_list[i].params[0],object_list[i].params[1],object_list[i].params[2],object_list[i].params[3],object_list[i].params[4],object_list[i].params[5]);
        }
    }
    if(blank)printf("[Empty Canvas Matrix Grid Layer]\n");
}

// 12. Command input dispatcher and controller
int main(){
    for(int i=0;i<MAX_OBJECTS;i++){
        object_list[i].active=0;
    }
    char operation;
    int tracking_id,offsets[2],inputs[6];
    while(1){
        display_picture();
        list_meta_data();
        printf("\nActions: [a] Add | [d] Delete | [m] Modify | [v] Move Vector | [q] Quit\nSelection: ");
        if(scanf(" %c",&operation)!=1)continue;
        if(operation=='q'){
            printf("Exiting vector graphic application environment context.\n");
            break;
        }
        switch(operation){
            case 'a':{
                char mode;
                printf("Shape Select: [l] Line | [r] Rect | [c] Circle | [t] Triangle: ");
                scanf(" %c",&mode);
                if(mode=='l'){
                    printf("Enter Coordinates (x0 y0 x1 y1) within bounds [0..59, 0..19]: ");
                    scanf("%d %d %d %d",&inputs[0],&inputs[1],&inputs[2],&inputs[3]);
                    add_object(LINE,inputs);
                }else if(mode=='r'){
                    printf("Enter Attributes (x y width height): ");
                    scanf("%d %d %d %d",&inputs[0],&inputs[1],&inputs[2],&inputs[3]);
                    add_object(RECTANGLE,inputs);
                }else if(mode=='c'){
                    printf("Enter Attributes (cx cy radius): ");
                    scanf("%d %d %d",&inputs[0],&inputs[1],&inputs[2]);
                    add_object(CIRCLE,inputs);
                }else if(mode=='t'){
                    printf("Enter 3 Points (x0 y0 x1 y1 x2 y2): ");
                    scanf("%d %d %d %d %d %d",&inputs[0],&inputs[1],&inputs[2],&inputs[3],&inputs[4],&inputs[5]);
                    add_object(TRIANGLE,inputs);
                }else{
                    printf("Error: Selection variant unrecognized.\n");
                }
                break;
            }
            case 'd':
                printf("Enter target tracking ID to delete: ");
                scanf("%d",&tracking_id);
                if(!delete_object(tracking_id)){
                    printf("Error: Object ID reference missing.\n");
                }
                break;
            case 'm':
                printf("Enter target tracking ID to modify: ");
                scanf("%d",&tracking_id);
                int found=0;
                for(int i=0;i<MAX_OBJECTS;i++){
                    if(object_list[i].active&&object_list[i].id==tracking_id){
                        found=1;
                        if(object_list[i].type==LINE){
                            printf("Enter replacement coordinates (x0 y0 x1 y1): ");
                            scanf("%d %d %d %d",&inputs[0],&inputs[1],&inputs[2],&inputs[3]);
                        }else if(object_list[i].type==RECTANGLE){
                            printf("Enter replacement attributes (x y width height): ");
                            scanf("%d %d %d %d",&inputs[0],&inputs[1],&inputs[2],&inputs[3]);
                        }else if(object_list[i].type==CIRCLE){
                            printf("Enter replacement attributes (cx cy radius): ");
                            scanf("%d %d %d",&inputs[0],&inputs[1],&inputs[2]);
                        }else if(object_list[i].type==TRIANGLE){
                            printf("Enter replacement vertices (x0 y0 x1 y1 x2 y2): ");
                            scanf("%d %d %d %d %d %d",&inputs[0],&inputs[1],&inputs[2],&inputs[3],&inputs[4],&inputs[5]);
                        }
                        modify_object(tracking_id,inputs);
                        break;
                    }
                }
                if(!found)printf("Error: ID matrix value mismatch.\n");
                break;
            case 'v':
                printf("Enter target tracking ID to shift: ");
                scanf("%d",&tracking_id);
                printf("Enter offset coordinates translation factors (dx dy): ");
                scanf("%d %d",&offsets[0],&offsets[1]);
                if(!move_object(tracking_id,offsets[0],offsets[1])){
                    printf("Move failed: Object parameters unshifted.\n");
                }
                break;
            default:
                printf("Error: Unknown action character token entry.\n");
        }
    }
    return 0;
}

