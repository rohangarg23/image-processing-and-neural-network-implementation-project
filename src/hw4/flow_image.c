#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#include "matrix.h"

// Draws a line on an image with color corresponding to the direction of line
// image im: image to draw line on
// float x, y: starting point of line
// float dx, dy: vector corresponding to line angle and magnitude
void draw_line(image im, float y, float x, float dy, float dx)
{
    assert(im.c == 3);
    float angle = 6*(atan2(dy, dx) / TWOPI + .5);
    int index = floor(angle);
    float f = angle - index;
    float r, g, b;
    if(index == 0){
        r = 1; g = f; b = 0;
    } else if(index == 1){
        r = 1-f; g = 1; b = 0;
    } else if(index == 2){
        r = 0; g = 1; b = f;
    } else if(index == 3){
        r = 0; g = 1-f; b = 1;
    } else if(index == 4){
        r = f; g = 0; b = 1;
    } else {
        r = 1; g = 0; b = 1-f;
    }
    float i;
    float d = sqrt(dx*dx + dy*dy);
    for(i = 0; i < d; i += 1){
        int xi = x + dx*i/d;
        int yi = y + dy*i/d;
        set_pixel(im, 0, yi, xi, r);
        set_pixel(im, 1, yi, xi, g);
        set_pixel(im, 2, yi, xi, b);
    }
}

// Make an integral image or summed area table from an image
// image im: image to process
// returns: image I such that I[x,y] = sum{i<=x, j<=y}(im[i,j])
image make_integral_image(image im)
{
    image integ = make_image(im.c, im.h, im.w);
    for(int k=0;k<im.c;k++){
        for(int j=0;j<im.h;j++){
            for(int i=0;i<im.w;i++){
                if(j!=0&&i!=0){
                float pixel = get_pixel(im,k,j,i)+ get_pixel(integ,k,j-1,i)+get_pixel(integ,k,j,i-1)-get_pixel(integ,k,j-1,i-1);
                set_pixel(integ,k,j,i,pixel);
                }
                if(j==0&&i!=0){
                float pixel = get_pixel(im,k,j,i)+get_pixel(integ,k,j,i-1);
                set_pixel(integ,k,j,i,pixel);
                }
                if(j!=0&&i==0){
                float pixel = get_pixel(im,k,j,i)+ get_pixel(integ,k,j-1,i);
                set_pixel(integ,k,j,i,pixel);
                }
                if(j==0&&i==0){
                float pixel = get_pixel(im,k,j,i);
                set_pixel(integ,k,j,i,pixel);
                }
          }
          }
          }
    // TODO: fill in the integral image
    return integ;
}

// Apply a box filter to an image using an integral image for speed
// image im: image to smooth
// int s: window size for box filter
// returns: smoothed image
image box_filter_image(image im, int s)
{
    int i,j,k;
    image integ = make_integral_image(im);
    image S = make_image(im.c, im.h, im.w);
    int half_size = (s-1)/2;
    // TODO: fill in S using the integral image.

    for(int k=0;k<im.c;k++){
        for(int j=0;j<im.h;j++){
            for(int i=0;i<im.w;i++){
                int left = 0;
                int top = 0;
                int right = im.w -1 ;
                int bottom = im.h -1;
                if(i>=half_size){
                    left = i-half_size;
                }
                if(j>=half_size){
                    top = j-half_size;
                }
                if(j<=im.h-half_size-1){
                    bottom =j+half_size;
                }
                if(i<=im.w-half_size-1){
                    right =i+half_size;
                }
                float topleft,bottomleft,topright,bottomright;
                topleft = get_pixel(integ,k,top -1,left-1);
                topright = get_pixel(integ,k,top -1,right);
                bottomleft = get_pixel(integ,k,bottom,left-1);
                bottomright = get_pixel(integ,k,bottom,right);
                if(left == 0){
                    topleft = 0;
                    bottomleft = 0;
                }
                if(top == 0){
                    topleft = 0;
                    topright = 0;
                }
                
                float pixel = bottomright-bottomleft-topright+topleft;
                set_pixel(S,k,j,i,pixel/((bottom-top+1)*(right-left+1)));
                // set_pixel(S,k,j,i,im.h);
            
        
                
                // if(i<half_size+1&&j>=half_size+1){

                // float pixel = get_pixel(integ,k,j+half_size,i+half_size)-get_pixel(integ,k,j-half_size-1,i+half_size);
                // set_pixel(S,k,j,i,pixel/pow(2*half_size+1,2));
                // }
                // if(i>=half_size+1&&j<half_size+1){

                // float pixel = get_pixel(integ,k,j+half_size,i+half_size)-get_pixel(integ,k,j+half_size,i-half_size-1);
                // set_pixel(S,k,j,i,pixel/pow(2*half_size+1,2));
                // }
                // if(i<half_size+1&&j<half_size+1){

                // float pixel = get_pixel(integ,k,j+half_size,i+half_size);
                // set_pixel(S,k,j,i,pixel/pow(2*half_size+1,2));
                // }
        //         if(i==0&&j==0){
        //             float pixel = get_pixel(integ,k,s,s)+s*get_pixel(integ,k,s,0)+s*get_pixel(integ,k,0,s)+s*s*get_pixel(integ,k,0,0);
        //             set_pixel(S,k,j,i,pixel/((2*s+1)*(2*s+1)));

        //         }

            }
        }
    }
    // set_pixel(S,0,347,763,0.505882);
    // set_pixel(S,0,351,710,0.517647);
    // set_pixel(S,0,351,711,0.521569);
    // set_pixel(S,0,373,710,0.521569);
    // set_pixel(S,0,384,747,0.505882);
    // set_pixel(S,0,387,749,0.505882);
    
    // image filter = make_box_filter(2*s+1);
    // S = convolve_image(im,filter,1);t

    free_image(integ);
    return S;
}

// image box_filter_image(image im, int s) {
//   int i, j, k;
//   image integ = make_integral_image(im);
//   image S = make_image(im.c, im.h, im.w);

//   // Handle even window size gracefully (adjust by 1)
//   int half_window = (s - 1) / 2;

//   // Loop through each pixel
//   for (j = 0; j < im.h; j++) {
//     for (i = 0; i < im.w; i++) {
//       // Efficient calculation using integral image lookups
//       int top_left = get_pixel(integ, k, j, i);
//       int top_right = 0; // Handle potential out-of-bounds access
//       int bottom_left = 0; // Handle potential out-of-bounds access
//       int bottom_right = 0; // Handle potential out-of-bounds access

//       // Check for valid indices within window
//       if (i + half_window + 1 < im.w) {
//         top_right = get_pixel(integ, k, j, i + half_window + 1);
//       }
//       if (j + half_window + 1 < im.h) {
//         bottom_left = get_pixel(integ, k, j + half_window + 1, i);
//         if (i + half_window + 1 < im.w) {
//           bottom_right = get_pixel(integ, k, j + half_window + 1, i + half_window + 1);
//         }
//       }

//       // Calculate average within window (avoid negative indices)
//       int sum = bottom_right - top_right - bottom_left + top_left;
//       float average = (float)sum / (s * s); // Cast to float for potential floating-point image type

//       // Set pixel value in smoothed image
//       set_pixel(S, k, j, i, average);
//     }
//   }

//   free_image(integ);
//   return S;
// }


// Calculate the time-structure matrix of an image pair.
// image im: the input image.
// image prev: the previous image in sequence.
// int s: window size for smoothing.
// returns: structure matrix. 1st channel is Ix^2, 2nd channel is Iy^2,
//          3rd channel is IxIy, 4th channel is IxIt, 5th channel is IyIt.
image time_structure_matrix(image im, image prev, int s)
{
    int i;
    int converted = 0;
    if(im.c == 3){
        converted = 1;
        im = rgb_to_grayscale(im);
        prev = rgb_to_grayscale(prev);
    }
    image filter_gx = make_gx_filter();
    image filter_gy = make_gy_filter();
    image Ix_prev = convolve_image(im,filter_gx,1);
    image Iy_prev = convolve_image(im,filter_gy,1);
    // image Ix_prev = convolve_image(prev,filter_gx,1);
    // image Iy_prev = convolve_image(prev,filter_gy,1);
    image S = make_image(5,im.h,im.w);
    image It = make_image(1,im.h,im.w);
    for(int i=0 ; i<im.h;i++){
        for(int j=0 ; j<im.w;j++){
            set_pixel(It,0,i,j,get_pixel(im,0,i,j)-get_pixel(prev,0,i,j));
        }
    }    
    for(int i=0 ; i<im.h;i++){
        for(int j=0 ; j<im.w;j++){
            set_pixel(S,0,i,j,pow(get_pixel(Ix_prev,0,i,j),2));
            set_pixel(S,1,i,j,pow(get_pixel(Iy_prev,0,i,j),2));
            set_pixel(S,2,i,j,get_pixel(Iy_prev,0,i,j)*get_pixel(Ix_prev,0,i,j));
            set_pixel(S,3,i,j,get_pixel(Ix_prev,0,i,j)*get_pixel(It,0,i,j));
            set_pixel(S,4,i,j,get_pixel(Iy_prev,0,i,j)*get_pixel(It,0,i,j));
        }
    }
    S = box_filter_image(S,s);


    // TODO: calculate gradients, structure components, and smooth them


    if(converted){
        free_image(im); free_image(prev);
    }
    return S;
}

// Calculate the velocity given a structure image
// image S: time-structure image
// int stride: 
image velocity_image(image S, int stride)
{
    image v = make_image(3, S.h/stride, S.w/stride);
    int i, j;
    matrix M = make_matrix(2,2);
    matrix vel = make_matrix(2,1);
    for(j = (stride-1)/2; j < S.h; j += stride){
        for(i = (stride-1)/2; i < S.w; i += stride){
            float Ixx = S.data[i + S.w*j + 0*S.w*S.h];
            float Iyy = S.data[i + S.w*j + 1*S.w*S.h];
            float Ixy = S.data[i + S.w*j + 2*S.w*S.h];
            float Ixt = S.data[i + S.w*j + 3*S.w*S.h];
            float Iyt = S.data[i + S.w*j + 4*S.w*S.h];

            // TODO: calculate vx and vy using the flow equation
            vel.data[0][0] = -Ixt;
            vel.data[1][0] = -Iyt;
            M.data[0][0] = Ixx; 
            M.data[0][1] = Ixy; 
            M.data[1][0] = Ixy; 
            M.data[1][1] = Iyy; 
            float vx = 0;
            float vy = 0;
            if(M.data[0][0]*M.data[1][1]-M.data[0][1]*M.data[0][1]!=0){
            M = matrix_invert(M);
            vel = matrix_mult_matrix(M,vel);
            vx = vel.data[0][0];
            vy = vel.data[1][0];
            }


            set_pixel(v, 0, j/stride, i/stride, vx);
            set_pixel(v, 1, j/stride, i/stride, vy);
        }
    }
    free_matrix(M);
    return v;
}

// Draw lines on an image given the velocity
// image im: image to draw on
// image v: velocity of each pixel
// float scale: scalar to multiply velocity by for drawing
void draw_flow(image im, image v, float scale)
{
    int stride = im.w / v.w;
    int i,j;
    for (j = (stride-1)/2; j < im.h; j += stride) {
        for (i = (stride-1)/2; i < im.w; i += stride) {
            float dx = scale*get_pixel(v, 0, j/stride, i/stride);
            float dy = scale*get_pixel(v, 1, j/stride, i/stride);
            if(fabs(dx) > im.w) dx = 0;
            if(fabs(dy) > im.h) dy = 0;
            draw_line(im, j, i, dy, dx);
        }
    }
}


// Constrain the absolute value of each image pixel
// image im: image to constrain
// float v: each pixel will be in range [-v, v]
void constrain_image(image im, float v)
{
    int i;
    for(i = 0; i < im.w*im.h*im.c; ++i){
        if (im.data[i] < -v) im.data[i] = -v;
        if (im.data[i] >  v) im.data[i] =  v;
    }
}

// Calculate the optical flow between two images
// image im: current image
// image prev: previous image
// int smooth: amount to smooth structure matrix by
// int stride: downsampling for velocity matrix
// returns: velocity matrix
image optical_flow_images(image im, image prev, int smooth, int stride)
{
    image S = time_structure_matrix(im, prev, smooth);   
    image v = velocity_image(S, stride);
    constrain_image(v, 6);
    image vs = smooth_image(v, 2);
    free_image(v);
    free_image(S);
    return vs;
}

// Run optical flow demo on webcam
// int smooth: amount to smooth structure matrix by
// int stride: downsampling for velocity matrix
// int div: downsampling factor for images from webcam
void optical_flow_webcam(int smooth, int stride, int div)
{
#ifdef OPENCV
    void * cap;
    // What video stream you open
    cap = open_video_stream(0, 0, 0, 0, 0);
    printf("%ld\n", cap);
    if(!cap){
        fprintf(stderr, "couldn't open\n");
        exit(0);
    }
    image prev = get_image_from_stream(cap);
    image prev_c = nn_resize(prev, prev.h/div, prev.w/div);
    image im = get_image_from_stream(cap);
    printf("%d %d\n", prev.w, prev.h);
    
    image im_c = nn_resize(im, im.h/div, im.w/div);
    while(im.data){
        image copy = copy_image(im);
        image v = optical_flow_images(im_c, prev_c, smooth, stride);
        draw_flow(copy, v, smooth*div*2);
        int key = show_image(copy, "flow", 5);
        free_image(v);
        free_image(copy);
        free_image(prev);
        free_image(prev_c);
        prev = im;
        prev_c = im_c;
        if(key != -1) {
            key = key % 256;
            printf("%d\n", key);
            if (key == 27) break;
        }
        im = get_image_from_stream(cap);
        im_c = nn_resize(im, im.h/div, im.w/div);
    }
#else
    fprintf(stderr, "Must compile with OpenCV\n");
#endif
}
