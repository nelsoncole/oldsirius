#include <io.h>
#include <gui/gui.h>
#include <gui/font.h>

extern display_t *display;
struct boxchar_st {

    // coordenda do campo
    int coord_x;
    int coord_y;    
    
    // cursor mov
    int x_cursor;
    int y_cursor;

    //Font size X*Y
    int fx_s;
    int fy_s;

};


struct boxchar_st boxchar_st = { .x_cursor =0, .y_cursor =0, .fx_s =8, .fy_s=8};

void BoxChar(int c, int color){


    if(boxchar_st.x_cursor >= display->x_resolution){
        boxchar_st.y_cursor +=boxchar_st.fy_s;
        boxchar_st.x_cursor  =0;      
    } 

    if(c == '\n'){  
        boxchar_st.y_cursor +=boxchar_st.fy_s;
        boxchar_st.x_cursor =0;
    }else if(c == '\t'){
        boxchar_st.x_cursor = (boxchar_st.x_cursor + (8*boxchar_st.fx_s)) &~((8*boxchar_st.fx_s)-1); 
    }else if(c >= ' '){ 
        drawchar_transparent(c,boxchar_st.x_cursor,boxchar_st.y_cursor,color);
        boxchar_st.x_cursor += boxchar_st.fx_s; 
           
       
    }

     


}

int __putchar_box(int c)
{
	BoxChar(c,0xffffffff);
	return 0;
}



void drawchar(uint8_t c, int x, int y, int fgcolor, int bgcolor){
   static  int font;
   int cx, cy;

   char mask[]={128,64,32,16,8,4,2,1};
    
   for(cy=0;cy < 8;cy++){
        font = font_data_8x8[(c *8) + cy];
                for(cx = 0;cx < 8;cx++){ 
                        if(font &mask[cx])put_pixel(x + cx,y + cy,fgcolor);
                             else put_pixel(x + cx,y + cy,bgcolor);
          }
        
    }
        
    

}

void drawchar_transparent(uint8_t c, int x, int y, int fgcolor){
   static  int font;
   int cx, cy;

   char mask[]={128,64,32,16,8,4,2,1};
    
   for(cy=0;cy < 8;cy++){
        font = font_data_8x8[(c *8) + cy];
                for(cx = 0;cx < 8;cx++){ 
                        if(font &mask[cx])put_pixel(x + cx,y + cy,fgcolor);
                  
          }
        
    }
           

}
