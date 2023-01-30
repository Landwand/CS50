#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            //average all the rgbtriple vals
            int rgbt = round((image[row][col].rgbtRed + image[row][col].rgbtGreen + image[row][col].rgbtBlue) / 3.0);
            image[row][col].rgbtRed = rgbt;
            image[row][col].rgbtGreen = rgbt;
            image[row][col].rgbtBlue = rgbt;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int midpoint = width / 2;
    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < midpoint; col++)
        {

            // why is col +1 needed?
            RGBTRIPLE otherSide = image[row][width - (col + 1)];
            //RGBTRIPLE temp = image[row][col];
            image[row][width - (col + 1)] = image[row][col];
            image[row][col] = otherSide;
        }
    }
}



// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // temp array to copy the image for processing
    RGBTRIPLE temp[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            temp[row][col] = image[row][col];
        }
    }

    // for each px, take the avg of all 9 surrounding px's values of rgbt

    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col++)
        {
            int row_start;
            int row_end;
            int col_start;
            int col_end;

            float total_blue = 0, total_green = 0, total_red = 0;
            float count = 0;


            // set start-end values for all rows
            if (row == 0)
            {
                row_start = row;
                row_end = row + 2;
            }
            else if (row == (height - 1))
            {
                row_start = row - 1;
                row_end = row + 1;
            }
            else
            {
                row_start = row - 1;
                row_end = row + 2;
            }


            // set start-end values for all cols
            if (col == 0)
            {
                col_start = col;
                col_end = col + 2;
            }
            else if (col == (width - 1))
            {
                col_start = col - 1;
                col_end = col + 1;
            }
            else
            {
                col_start = col - 1;
                col_end = col + 2;
            }


            //avg all RGBTRIPLE vals, from Temp
            for (; row_start < (row_end); row_start ++)
            {
                for (int col_count = col_start ; col_count < (col_end); col_count ++)

                {
                    total_red += temp[row_start][col_count].rgbtRed;
                    total_blue += temp[row_start][col_count].rgbtBlue;
                    total_green += temp[row_start][col_count].rgbtGreen;
                    count += 1;
                }

            }

            // write avg into the actual image
            image[row][col].rgbtRed = round(total_red / count);
            image[row][col].rgbtGreen = round(total_green / count);
            image[row][col].rgbtBlue = round(total_blue / count);
        }

    }

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //copy entire image into Temp
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            temp[i][j] = image[i][j];
        }
    }


    // 2-D matrix for values
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col ++)
        {
            float Gx_red = 0;
            float Gx_green = 0;
            float Gx_blue = 0;

            float Gy_red = 0;
            float Gy_green = 0;
            float Gy_blue = 0;

            // now iterate surrounding pixels
            for (int r = -1; r < 2; r ++)
            {
                for (int c = -1; c < 2; c++)
                {
                    // check for invalid px; if found, skip
                    // check to see if px is beyond row
                    if (row + r >= height || row + r < 0)
                    {
                        continue;
                    }
                    // check to see if px is beyond col
                    if (col + c >= width || col + c < 0)
                    {
                        continue;
                    }

                    // for valid px

                    Gx_red += temp[row + r][col + c].rgbtRed * Gx[r + 1][c + 1];
                    Gx_green += temp[row + r][col + c].rgbtGreen * Gx[r + 1][c + 1];
                    Gx_blue += temp[row + r][col + c].rgbtBlue * Gx[r + 1][c + 1];

                    Gy_red += temp[row + r][col + c].rgbtRed * Gy[r + 1][c + 1];
                    Gy_green += temp[row + r][col + c].rgbtGreen * Gy[r + 1][c + 1];
                    Gy_blue += temp[row + r][col + c].rgbtBlue * Gy[r + 1][c + 1];
                } // c
            } // r

            int red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));
            int green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));


            //check for max values
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

            // apply to image
            image[row][col].rgbtRed = red;
            image[row][col].rgbtGreen = green;
            image[row][col].rgbtBlue = blue;
        } // col

    } // row

    return;
}


