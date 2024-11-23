#include <math.h>
#include "image.h"

float nn_interpolate(image im, int c, float h, float w)
{
   
    return get_pixel(im,c,round(h),round(w));
}

image nn_resize(image im, int h, int w)
{   image new_im=make_image(im.c,h,w);
    float ratio=(float)im.h/h;
    float ratio_=(float)im.w/w;
    for(int c=0;c<im.c;c++){
        for(int p=0;p<h;p++){
            for(int q=0;q<w;q++){
                set_pixel(new_im,c,p,q,nn_interpolate(im,c,(ratio/2)+p*ratio-0.5,(ratio_/2)+q*ratio_-0.5));
            }
        }
    }
    return new_im; // <- probably delete this
}

float bilinear_interpolate(image im, int c, float h, float w){
    if(h<0) h=0;
    if(w<0) w=0;
    if(h>=im.h) h=im.h-1;
    if(w>=im.w) w=im.w-1;

    int h1=(int)h;
    int w1=(int)w;
    float y= h-h1;
    float x= w-w1;
    float v1=get_pixel(im,c,h1,w1);
    float v2=get_pixel(im,c,h1,w1+1);
    float v3=get_pixel(im,c,h1+1,w1);
    float v4=get_pixel(im,c,h1+1,w1+1);
    float px= v1*(1-x)*(1-y)+v2*(x)*(1-y)+v3*(1-x)*(y)+v4*(x)*(y);
    return px;
}

image bilinear_resize(image im, int h, int w)
{
     image new_im=make_image(im.c,h,w);
    float ratio=(float)im.h/h;
    float ratio_=(float)im.w/w;
    for(int c=0;c<im.c;c++){
        for(int p=0;p<h;p++){
            for(int q=0;q<w;q++){
                set_pixel(new_im,c,p,q,bilinear_interpolate(im,c,(ratio/2)+p*ratio-0.5,(ratio_/2)+q*ratio_-0.5));
            }
        }
    }
    return new_im; // <- probably delete this
}

