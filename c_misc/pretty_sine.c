//
// Pretty Sine program, written in C99.
//
// This little program generate a pretty Bitmap .BMP file that you can use as
// a background image.
//
// Pretty Sine uses the trigonometric function sine to generate said image,
// hence its name. (Don't ask me what the colors mean; I don't know.)
//
// Feel free to change the color formula (line 227) and create more prettiness!
//

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

///
/// @brief Default values for image width and height, in pixels, in case
///  the user doesn't specify custom values.
///
#define DEFAULT_WIDTH               512
#define DEFAULT_HEIGHT              512

///
/// @brief Prints help information.
///
static void print_help(void)
{
    puts("\nPretty Sine usage:");
    puts("\tpretty_sine [-h|--help]");
    puts("\tpretty_sine output_image [width [height]]");
    puts("\nExamples:");
    puts("\tpretty_sine.exe background.bmp 1024 768");
    puts("\tpretty_sine.exe square.bmp 100");
}

///
/// @brief Converts given floating point number to a color byte.
/// @pre -1.0 <= `d` <= 1.0
/// @param [in] d                   Floating point number to be converted.
/// @returns Corresponding byte code as needed in RGB encoding.
///
static uint8_t get_color(double d)
{
    return (d + 1.0) / 2.0 * 255.0;
}

#pragma pack(1)

///
/// @brief Bitmap file header.
///
typedef struct
{
    uint8_t     magic[2];           ///< Magic field: set "BM".
    uint32_t    size;               ///< Size of the .BMP file in bytes.
    uint16_t    res0;               ///< First reserved value: set "0".
    uint16_t    res1;               ///< Second reserved value: set "0".
    uint32_t    offset;             ///< Offset of the pixel array.
} bitmap_file_t;

///
/// @brief Bitmap information header.
///
typedef struct
{
    uint32_t    size;               ///< Size of this header.
    uint32_t    width;              ///< Bitmap width in pixels.
    uint32_t    height;             ///< Bitmap height in pixels.
    uint16_t    ncp;                ///< Number of color planes, "must be 1".
    uint16_t    bpp;                ///< Bits per pixel: set "24".
    uint32_t    comp;               ///< Compression method: set "0".
    uint32_t    isize;              ///< Image size: set "0".
    uint32_t    ppmx;               ///< Pixels per meter width: set "0".
    uint32_t    ppmy;               ///< Pixels per meter height: set "0".
    uint32_t    ncpal;              ///< Number of colors in palette: set "0".
    uint32_t    nicol;              ///< Number of important colors: set "0".
} bitmap_info_t;

#pragma pack()

///
/// @brief Enters the program.
/// @param [in] argc                Number of arguments.
/// @param [in] argv                Argument strings.
/// @returns Whether or not the operation was successful.
/// @retval EXIT_SUCCESS            Operation success.
/// @retval EXIT_FAILURE            Operation failure.
///
int main(int argc, char *argv[])
{
    //
    // intent: print help if there is at most 1 or more than 4 arguments, or
    // if there are exactly 2 arguments and the second one is
    // either "-h" or "--help"
    //
    if ((argc <= 1 || argc > 4) ||
        (argc == 2 &&
        (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)))
    {
        print_help();
        return EXIT_SUCCESS;
    }

    // start preparing the Bitmap file and information (DIB) headers

    bitmap_file_t bmp_file = {
        .magic      = {'B', 'M'},
        //.size     = 0, // to be determined
        .res0       = 0,
        .res1       = 0,
        .offset     = sizeof (bitmap_file_t) + sizeof (bitmap_info_t)
    };

    bitmap_info_t bmp_info = {
        .size       = sizeof (bitmap_info_t),
        .width      = DEFAULT_WIDTH,
        .height     = DEFAULT_HEIGHT,
        .ncp        = 1,
        .bpp        = 24,
        .comp       = 0,
        .isize      = 0,
        .ppmx       = 0,
        .ppmy       = 0,
        .ncpal      = 0,
        .nicol      = 0
    };

    // attempt to use the arguments given by the user

    const char *usr_filename    = NULL;
    const char *usr_width       = NULL;
    const char *usr_height      = NULL;

    if (argc >= 2)
        usr_filename = argv[1];

    if (argc >= 3)
        usr_width = argv[2];

    if (argc >= 4)
        usr_height = argv[3];

    // this should never happen, unless the help printing code above is changed
    if (usr_filename == NULL)
    {
        fputs("error: no output filename was given", stderr);
        return EXIT_FAILURE;
    }

    if (usr_width != NULL)
    {
        const unsigned long int width = strtoul(usr_width, NULL, 10);

        if (width != 0 && width <= UINT32_MAX)
        {
            bmp_info.width  = width;
            bmp_info.height = width;
        }
        else
            fputs("warning: bad value for width", stderr);
    }

    if (usr_height != NULL)
    {
        const unsigned long int height = strtoul(usr_height, NULL, 10);

        if (height != 0 && height <= UINT32_MAX)
            bmp_info.height = height;
        else
            fputs("warning: bad value for height", stderr);
    }

    // attempt to allocate memory for the image's pixel data

    // total size of the Bitmap's pixel array, measured in bytes
    const size_t bmp_img_bytes = bmp_info.width * bmp_info.height *
        (bmp_info.bpp / CHAR_BIT) * sizeof (uint8_t);

    uint8_t * const bmp_pixels = malloc(bmp_img_bytes);

    if (bmp_pixels == NULL)
    {
        fputs("error: malloc(): could not allocate memory for Bitmap", stderr);
        return EXIT_FAILURE;
    }

    // attempt to open the output file

    FILE *output_file = fopen(usr_filename, "wb");

    if (output_file == NULL)
    {
        perror("error: fopen()");
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    // finish preparing the Bitmap file and information (DIB) headers
    bmp_file.size = sizeof (bmp_file) + sizeof (bmp_info) + bmp_img_bytes;

    // calculate Bitmap pixels

    for (size_t x=0; x < bmp_info.height; ++x)
    {
        for (size_t y=0; y < bmp_info.width; ++y)
        {
#define xy_offset   ((x * bmp_info.width + y) * bmp_info.bpp / CHAR_BIT)
            uint8_t * const red     = bmp_pixels + xy_offset + 0;
            uint8_t * const green   = bmp_pixels + xy_offset + 1;
            uint8_t * const blue    = bmp_pixels + xy_offset + 2;
#undef xy_offset

            const double pi     = acos(-1.0);
            const double tau    = 2.0 * pi;

            const double xr     = (double)x / bmp_info.height;  // X Ratio
            const double yr     = (double)y / bmp_info.width;   // Y Ratio
            // XY (Diagonal) Ratio
            const double xyr    = (double)(x + y) /
                (bmp_info.height + bmp_info.width);

            // mysterious magic of forgotten high school math, go!
            *red    = get_color(sin(pi * xr));
            *green  = get_color(sin(pi * yr));
            *blue   = get_color(sin(tau * xyr));
        }
    }

    // write Bitmap data

    if (fwrite(&bmp_file, sizeof bmp_file, 1, output_file) != 1)
    {
        fputs("error: fwrite(): could not write Bitmap file header", stderr);
        fclose(output_file);
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    if (fwrite(&bmp_info, sizeof bmp_info, 1, output_file) != 1)
    {
        fputs("error: fwrite(): could not write Bitmap info header", stderr);
        fclose(output_file);
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    if (fwrite(bmp_pixels, sizeof bmp_pixels[0], bmp_img_bytes, output_file) !=
        bmp_img_bytes)
    {
        fputs("error: fwrite(): could not write Bitmap pixel data", stderr);
        fclose(output_file);
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    fclose(output_file);
    free(bmp_pixels);
    return EXIT_SUCCESS;
}
