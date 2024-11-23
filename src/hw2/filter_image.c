#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{   float a=0;
    for(int i=0;i<im.h;i++){
        for(int j=0;j<im.w;j++){
           a += get_pixel(im,0,i,j);
        }
    }
    for(int i=0;i<im.h;i++){
        for(int j=0;j<im.w;j++){
           set_pixel(im,0,i,j,get_pixel(im,0,i,j)/a);
        }
    }
}

image make_box_filter(int w){
    image im =make_image(1,w,w);
    for(int i=0;i<im.h;i++){
        for(int j=0;j<im.w;j++){
           set_pixel(im,0,i,j,1);
        }
    }
    l1_normalize(im);
    return im;
}

image convolve_image(image im, image filter, int preserve)
{    
    assert(im.c==filter.c||filter.c==1);
    image new_im;
    
    if(preserve==1){
        new_im = make_image(im.c,im.h,im.w);
        }
    else{
        new_im = make_image(1,im.h,im.w);
    }
        for(int k=0;k<im.w;k++){
            for(int j=0;j<im.h;j++){
                float a=0;
                for(int i=0;i<im.c;i++){
                    
                    float value=0;
                    int channel=i;
                    if(filter.c==1){channel=0;}
                    for(int p=-(filter.h-1)/2;p<(filter.h+1)/2;p++){
                        for(int q=-(filter.w-1)/2;q<(filter.w+1)/2;q++)
                            value += get_pixel(im,i,j+p,k+q)*get_pixel(filter,channel,p+(filter.h-1)/2,q+(filter.w-1)/2);
                    }
                    if(preserve==1){
                        set_pixel(new_im,i,j,k,value);
                    }
                    else{
                        a += value;
                    }
                }
                if(preserve!=1)
                {
                    set_pixel(new_im,0,j,k,a);
                }
            }
        }
    
    
    return new_im;
}

image make_highpass_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter,0,0,0,0);
    set_pixel(filter,0,2,0,0);
    set_pixel(filter,0,0,2,0);
    set_pixel(filter,0,2,2,0);
    set_pixel(filter,0,0,1,-1);
    set_pixel(filter,0,2,1,-1);
    set_pixel(filter,0,1,0,-1);
    set_pixel(filter,0,1,2,-1);
    set_pixel(filter,0,1,1,4);
    return filter;
}

image make_sharpen_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter,0,0,0,0);
    set_pixel(filter,0,2,0,0);
    set_pixel(filter,0,0,2,0);
    set_pixel(filter,0,2,2,0);
    set_pixel(filter,0,0,1,-1);
    set_pixel(filter,0,2,1,-1);
    set_pixel(filter,0,1,0,-1);
    set_pixel(filter,0,1,2,-1);
    set_pixel(filter,0,1,1,5);
    return filter;
}

image make_emboss_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter,0,0,0,-2);
    set_pixel(filter,0,2,0,0);
    set_pixel(filter,0,0,2,0);
    set_pixel(filter,0,2,2,2);
    set_pixel(filter,0,0,1,-1);
    set_pixel(filter,0,2,1,1);
    set_pixel(filter,0,1,0,-1);
    set_pixel(filter,0,1,2,1);
    set_pixel(filter,0,1,1,1);
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{   int p;
    if(6*sigma==(int)6*sigma){p=(int)6*sigma;}
    else{p=(int)6*sigma+1;}
    if(p%2==0){p=p+1;}
    image im = make_box_filter(p);
    for(int i=-(p-1)/2;i<=(p-1)/2;i++){
        for(int j=-(p-1)/2;j<=(p-1)/2;j++){
            set_pixel(im,0,(p-1)/2+i,(p-1)/2+j,(1/(TWOPI*pow(sigma,2)))*exp((-pow(i,2)-pow(j,2))/(2*pow(sigma,2))));
        }
    }
    l1_normalize(im);
    
    return im;
}

image add_image(image a, image b){
    // assert(a.w==b.w&&a.h==b.h&&a.c==b.c);
    if(a.h>b.h||a.w>b.w){b=bilinear_resize(b,a.h,a.w);}
    if(a.h<b.h||a.w<b.w){a=bilinear_resize(a,b.h,b.w);}
    image im = make_image(a.c,a.h,a.w);

    for(int i=0;i<a.h;i++){
        for(int j=0;j<a.w;j++){
            for(int k=0;k<a.c;k++){
                set_pixel(im,k,i,j,get_pixel(a,k,i,j)+get_pixel(b,k,i,j));
            }
        }
    }
    return im;
}

image sub_image(image a, image b)
{
    // assert(a.w==b.w&&a.h==b.h&&a.c==b.c);
    if(a.h>b.h||a.w>b.w){b=bilinear_resize(b,a.h,a.w);}
    if(a.h<b.h||a.w<b.w){a=bilinear_resize(a,b.h,b.w);}
    image im = make_image(a.c,a.h,a.w);

    for(int i=0;i<a.h;i++){
        for(int j=0;j<a.w;j++){
            for(int k=0;k<a.c;k++){
                set_pixel(im,k,i,j,get_pixel(a,k,i,j)-get_pixel(b,k,i,j));
            }
        }
    }
    return im;
    
}

image make_gx_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter,0,0,0,-1);
    set_pixel(filter,0,0,1,0);
    set_pixel(filter,0,0,2,1);
    set_pixel(filter,0,1,0,-2);
    set_pixel(filter,0,1,1,0);
    set_pixel(filter,0,1,2,2);
    set_pixel(filter,0,2,0,-1);
    set_pixel(filter,0,2,1,0);
    set_pixel(filter,0,2,2,1);
    return filter;
}

image make_gy_filter()
{
    image filter = make_box_filter(3);
    set_pixel(filter,0,0,0,-1);
    set_pixel(filter,0,0,1,-2);
    set_pixel(filter,0,0,2,-1);
    set_pixel(filter,0,1,0,0);
    set_pixel(filter,0,1,1,0);
    set_pixel(filter,0,1,2,0);
    set_pixel(filter,0,2,0,1);
    set_pixel(filter,0,2,1,2);
    set_pixel(filter,0,2,2,1);
    return filter;
}

void feature_normalize(image im)
{   float max = get_pixel(im,0,0,0);
    float min = get_pixel(im,0,0,0);
    for(int i=0;i<im.h;i++){
        for(int j=0;j<im.w;j++){
            if(get_pixel(im,0,i,j)>max){max=get_pixel(im,0,i,j);}
            if(get_pixel(im,0,i,j)<min){min=get_pixel(im,0,i,j);}
        }
    }
    float range = max-min;
    
        for(int i=0;i<im.h;i++){
            for(int j=0;j<im.w;j++){
               if(range==0){ set_pixel(im,0,i,j,0);}
               else{set_pixel(im,0,i,j,(get_pixel(im,0,i,j)-min)/range);}
            }
        }
    
}

image *sobel_image(image im)
{   image gx = make_gx_filter();
    image gy = make_gy_filter();
    image gx_filtered = convolve_image(im,gx,0);
    image gy_filtered = convolve_image(im,gy,0);
    
    image *d = calloc(2, sizeof(image));
    d[0] = make_image(1,im.h,im.w);
    d[1] = make_image(1,im.h,im.w);
    // for(int c=0;c<im.c;c++){
     for(int i=0;i<im.h;i++){
            for(int j=0;j<im.w;j++){
                float gx_value = get_pixel(gx_filtered,0,i,j);
                float gy_value = get_pixel(gy_filtered,0,i,j);
                set_pixel(d[0],0,i,j,pow(pow(gx_value,2)+pow(gy_value,2),0.5));
                set_pixel(d[1],0,i,j,atan2(gy_value,gx_value));
            }
        }
    // }
    return d;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
