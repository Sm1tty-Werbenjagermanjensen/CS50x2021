#include "helpers.h"
#include "math.h"
#include "stdio.h"

void blur_pixel(int y, int x, int height, int width, RGBTRIPLE image[height][width], BYTE *buffer);

// convert image to grayscale
void grayscale(int height, int width, rgbtriple image[height][width])
{
    byte buffer;
    float sum;
    
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            // calc greyscale value for pixel
            sum = image[i][j].rgbtblue + image[i][j].rgbtgreen + image[i][j].rgbtred;
            buffer = round(sum / 3); 
            //apply greyscale value to all color values in pixel
            for (int k = 0; k < 3; k++)
                *(&image[i][j].rgbtblue + k) = buffer;
        }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

        
    float originalPixel[3]; // 0,1,2 == Blue,Green,Red
    int sepiaPixel[3];     // 0,1,2 == Blue,Green,Red
    

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            // store original pixel color values
            for (int k = 0; k < 3; k++)
                originalPixel[k] = *(&(image[i][j].rgbtBlue) + k);
            // calc Sepia filter to color values
            // Blue:
            sepiaPixel[0] = round((.272 * originalPixel[2]) + (.534 * originalPixel[1]) + (.131 * originalPixel[0]));
            // Green:
            sepiaPixel[1] = round((.349 * originalPixel[2]) + (.686 * originalPixel[1]) + (.168 * originalPixel[0]));
            // Red:
            sepiaPixel[2] = round((.393 * originalPixel[2]) + (.769 * originalPixel[1]) + (.189 * originalPixel[0]));
            // cap color values @ 0xff (i.e black) and apply them to image pixel 
            for (int k = 0; k < 3; k++)
            {
                if (sepiaPixel[k] > 0xff)
                    sepiaPixel[k] = 0xff;
                *((&image[i][j].rgbtBlue) + k) = sepiaPixel[k];
            } 
        }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Left and Right indexes of scanline
    int l = 0;
    int r = width - 1;
    //temp pixel value
    RGBTRIPLE tmp;
    // reflect image
    for (int i = 0; i < height; i++)
    { 
        int j = 0;
        while((r - j) > (l + j)) 
        {
            tmp = image[i][l+j];
            image[i][l+j] = image[i][r-j];
            image[i][r-j] = tmp;
            j++;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[2][width]; /* buffer stores 2 scanlines worth of blurred pixels.
                                   The 1st dimension denotes a figurative waiting list for being 
                                   written to the image:
                                   buffer[0] are the pixels who are first in line.
                                   buffer[1] are pixels next in line.
                                   When a blurred pixel is calculated, it is added to buffer[1].
                                   When a pixel in buffer[0] is written to image, the corresponding 
                                   pixel below in buffer[1] moves up the line to take its place. */
 
    for (int i = 0; i < height + 2; i++) // + 2 is needed to finish writing the last two lines to image.
    {
        for (int j = 0; j < width; j++)
        {  
            if (i > 0) // Move blurred pixel from previous cycle up the waiting list. (i.e pixel[i-1][j])
                buffer[0][j] = buffer[1][j];
            if (i > 1) // Write blurred pixel from 2 cycles ago to image (i.e value first in line: pixel[i-2][j])
                image[i-2][j] = buffer[0][j];
            if (i < height) //calc and write pixel[i][j]'s blurred value to buffer[1][j]
                blur_pixel(i, j, height, width, image, &buffer[1][j].rgbtBlue);
        }
    }

    return;
}


void blur_pixel(int y, int x, int height, int width, RGBTRIPLE image[height][width], BYTE *buffer)
{
    float sum = 0; // sum of pixel color values
    float n = 0; // number of pixel color values (varies if pixel is on an edge or corner) 
    // loop for each color value (BGR)
    for (BYTE c = 0; c < 3; c++)
    {
        for (int i = -1; i < 2; i++) // loop 3x ( i = -1, 0, 1) for pixels 1 above, equal, and 1 below height
        {   //proceed if pixel exists (i.e if y+1 is a pixel height value that exists in the image)
            if ((y+i) > -1 && (y+i) < (height - 1))
            {
                for (int j = -1; j < 2; j++) // same as "i" but now for horizontal values (in relation to width) 
                { // proceed if pixel exists (i.e if x+j is a pixel width value that exists in image)
                    if ((x+j) > 0 && (x+j) < (width - 1))
                        {
                            sum += *(&(image[y+i][x+j].rgbtBlue) + c); //add this pixels color value to sum
                            n++; //increment number of pixel color values
                        }
                }
            }   
        }
        // set pixel[y][x] color value to blurred value
       *(buffer + c) = round(sum / n);
    }
    return;
}
