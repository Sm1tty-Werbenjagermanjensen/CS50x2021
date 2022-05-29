#include "helpers.h"

// Sub-function prototypes
// Edges:
void edge_pixel(int y, int x, int height, int width, RGBTRIPLE image[height][width], BYTE *buffer);
// Blur:
void blur_pixel(int y, int x, int height, int width, RGBTRIPLE image[height][width], BYTE *buffer);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
        BYTE buffer;
    float sum;
    
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            // calc greyscale value for pixel
            sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            buffer = round(sum / 3); 
            //apply greyscale value to all color values in pixel
            for (int k = 0; k < 3; k++)
                *(&image[i][j].rgbtBlue + k) = buffer;
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[2][width]; /* buffer stores 2 scanlines worth of calculated pixels.
                                   The 1st dimension denotes a figurative waiting list for being 
                                   written to the image:
                                   buffer[0] are the pixels who are first in line.
                                   buffer[1] are pixels next in line.
                                   When a pixel is calculated, it is added to buffer[1].
                                   When a pixel in buffer[0] is written to image, the corresponding 
                                   pixel below in buffer[1] moves up the line to take its place. */


    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i > 0) // Move edge pixel from previous cycle up the waiting list. (i.e pixel[i-1][j])
                buffer[0][j] = buffer[1][j];
            if (i > 1) // Write edge pixel from 2 cycles ago to image (i.e value first in line: pixel[i-2][j])
                image[i-2][j] = buffer[0][j];
            if (i < height) //calc and write pixel[i][j]'s edge value to buffer[1][j]
                edge_pixel(i, j, height, width, image, &buffer[1][j].rgbtBlue);
        }
    }

    return;
}

void edge_pixel(int y, int x, int height, int width, RGBTRIPLE image[height][width], BYTE *buffer)
{
    float Gx, GxSum, Gy, GySum, G;

    for (int c = 0; c < 3; c++)
    {
        GySum = 0; //reset x and y axis sum for each color
        GxSum = 0;
        for (int i = -1; i < 2; i++)
        {
            if ((y+i) > 0 && (y+i) < (height-1))
            {
                for (int j = -1; j < 2; j++)
                {
                    if ((x+j) > 0 && (x+j) < (width-1))
                    {
                        Gy = i;
                        Gx = j;
                        if (i == 0)
                            Gx *= 2;
                        if (j == 0)
                            Gy *= 2;
                        GySum += Gy * *(&(image[y+i][x+j].rgbtBlue) + c);
                        GxSum += Gx * *(&(image[y+i][x+j].rgbtBlue) + c);
                    }
                }
            }
        }
        // calc G
        G = round(pow((pow(GySum, 2) + pow(GxSum, 2)), 0.5));
        // cap G at 0xff
        if (G > 0xff)
            G = 0xff;
        //set corresponding buffer value to G
        *(buffer + c) = G;
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


