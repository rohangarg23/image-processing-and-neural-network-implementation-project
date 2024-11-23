#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int c, int h, int w){
    if(h>=im.h) h=im.h-1;
    if(w>=im.w) w=im.w-1;
    if(h<0) h=0;
    if(w<0) w=0;
    return im.data[h*im.w+w+(c)*im.h*im.w];
    
}

void set_pixel(image im, int c, int h, int w, float v)
{   if(w<im.w&&w>=0&&h<im.h&&h>=0&&c<im.c&&c>=0)
    { im.data[h*im.w+w+(c)*im.h*im.w]=v;}
    else
    {return;}
}

image copy_image(image im)
{
    image copy = make_image(im.c, im.h, im.w);
    for(int c =0;c<im.c;c++){
        for(int h=0;h<im.h;h++){
            for(int w =0;w<im.w;w++){
                set_pixel(copy,c,h,w,get_pixel(im,c,h,w));
                }
                }
                }

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(1, im.h, im.w);
    for(int w =0;w<im.w;w++){
        for(int h=0;h<im.h;h++){
            set_pixel(gray,0,h,w,0.299*get_pixel(im,0,h,w)+0.587*get_pixel(im,1,h,w)+0.114*get_pixel(im,2,h,w));
          }
          }
    return gray;
}

void shift_image(image im, int c, float v)
{
        for(int w =0;w<im.w;w++){
        for(int h=0;h<im.h;h++){
            float p = get_pixel(im,c,h,w);
            set_pixel(im,c,h,w,p+v);
          }
          }
}

void clamp_image(image im)
{
    for(int c =0;c<im.c;c++){
        for(int h=0;h<im.h;h++){
            for(int w =0;w<im.w;w++){
                if(get_pixel(im,c,h,w)>1){
                    set_pixel(im,c,h,w,1);
                    }
                if(get_pixel(im,c,h,w)<0){
                    set_pixel(im,c,h,w,0);
                    }
                }
                }
                }   
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
  for(int w =0;w<im.w;w++){
        for(int h=0;h<im.h;h++){
            float r=get_pixel(im,0,h,w);
            float g=get_pixel(im,1,h,w);
            float b=get_pixel(im,2,h,w);

            float V=three_way_max(r,g,b);
            float m=three_way_min(r,g,b);
                        
            float C = V-m;
            float S=0;
            if(V!=0)  {S=C/V;}
            
            float H1=0,H;
            if(C==0){
                H1=0;
            }
            else if (V==r)
            {
                H1=(g-b)/C;
            }
            else if (V==g)
            {
                H1=((b-r)/C)+2;
            }
            else if (V==b)
            {
                H1=((r-g)/C)+4;
            }
            if(H1<0) H=1+(H1/6);
            if(H1>=0) H=H1/6;
            


            set_pixel(im,0,h,w,H);
            set_pixel(im,1,h,w,S);
            set_pixel(im,2,h,w,V);
          }
          }

}

 void hsv_to_rgb(image im)
{   if (im.c < 3) {
        return;
        } 
    
    
    for(int h=0;h<im.h;h++){
        for(int w =0;w<im.w;w++){
            float H=get_pixel(im,0,h,w);
            float S=get_pixel(im,1,h,w);
            float V=get_pixel(im,2,h,w);
            float C = S*V;
            float m=V-C;
            float H1,R,G,B;
            if(6*H>=5){
                H1=6*H-6;
            }
            else if(6*H<5){
                H1=6*H;
            }
            if(H1<0) {R=V;G=m;B=G-H1*C;}
            else if(H1>=0&&H1<1) {R=V;B=m;G=B+H1*C;}
            else if(H1>=1&&H1<2) {G=V;B=m;R=B-(H1-2)*C;}
            else if(H1>=2&&H1<3) {G=V;R=m;B=R+(H1-2)*C;}
            else if(H1>=3&&H1<4) {B=V;R=m;G=R-(H1-4)*C;}
            else if(H1>=4&&H1<5) {B=V;G=m;R=G+(H1-4)*C;}
            set_pixel(im,0,h,w,R);
            set_pixel(im,1,h,w,G);
            set_pixel(im,2,h,w,B);



        }
        }
}
