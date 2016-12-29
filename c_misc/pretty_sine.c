//
// Pretty Sine program, written in C99.
//
// This little program generates a Bitmap .BMP file that is pretty enough to
// be used as a background image, in my opinion.
//
// Pretty Sine uses the trigonometric function Sine to generate said image,
// hence its name. (Don't ask me what the colors mean: I don't know.)
//
// Feel free to change the color formula (line 228) and create new prettiness!
//

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
/// @brief Converts a floating point number to a color byte.
/// @details Maps the interval `[-1.0, 1.0]` to `[0, 255]`.
/// @note Define the `NDEBUG` macro for release builds to disable `assert()`.
/// @pre -1.0 <= `d` <= 1.0
/// @param [in] d                   Floating point number to be converted.
/// @returns Corresponding byte code as needed in RGB encoding.
///
static uint8_t get_color(double d)
{
    assert(d >= -1.0 && d <= 1.0);
    return (d + 1.0) / 2.0 * 255.0;
}

#pragma pack(1)

///
/// @brief Bitmap file header.
/// @warning Assumming a "byte" is an octet.
///
typedef struct
{
    uint8_t     magic[2];   ///< Magic field, set `"BM"`.
    uint32_t    fsize;      ///< Size of the .BMP file, measured in bytes.
    uint16_t    res0;       ///< First reserved value, set `0`.
    uint16_t    res1;       ///< Second reserved value, set `0`.
    uint32_t    offset;     ///< Offset of the pixel array.
} bitmap_file_t;

///
/// @brief Bitmap information header.
/// @warning Assumming a "byte" is an octet.
///
typedef struct
{
    uint32_t    hsize;      ///< Size of this header.
    int32_t     width;      ///< Bitmap width, measured in pixels.
    int32_t     height;     ///< Bitmap height, measured in pixels.
    uint16_t    ncp;        ///< Number of color planes, "must be `1`".
    uint16_t    bpp;        ///< Bits per pixel, set `24`.
    uint32_t    comp;       ///< Compression method, set `0`.
    uint32_t    isize;      ///< Size of image, set `0`.
    int32_t     ppmx;       ///< Pixels per meter width, set `0`.
    int32_t     ppmy;       ///< Pixels per meter height, set `0`.
    uint32_t    ncpal;      ///< Number of colors in palette, set `0`.
    uint32_t    nicol;      ///< Number of important colors, set `0`.
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
    // print help if there is at most 1 or more than 4 arguments, or
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
        //.fsize    = 0, // to be determined
        .res0       = 0,
        .res1       = 0,
        .offset     = sizeof (bitmap_file_t) + sizeof (bitmap_info_t)
    };

    bitmap_info_t bmp_info = {
        .hsize      = sizeof (bitmap_info_t),
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

    if (argc >= 2) usr_filename = argv[1];
    if (argc >= 3) usr_width    = argv[2];
    if (argc >= 4) usr_height   = argv[3];

    if (usr_filename == NULL) // this should never happen
    {
        fputs("error: no output filename was given\n", stderr);
        return EXIT_FAILURE;
    }

    if (usr_width != NULL)
    {
        const unsigned long int width = strtoul(usr_width, NULL, 10);

        if (width != 0 && width <= INT32_MAX)
        {
            bmp_info.width  = width;
            bmp_info.height = width;
        }
        else
            fputs("warning: bad value for width\n", stderr);
    }

    if (usr_height != NULL)
    {
        const unsigned long int height = strtoul(usr_height, NULL, 10);

        if (height != 0 && height <= INT32_MAX)
            bmp_info.height = height;
        else
            fputs("warning: bad value for height\n", stderr);
    }

    // attempt to allocate memory for the image's pixel data

    // total size of the Bitmap's pixel array, measured in bytes
    const size_t bmp_img_bytes = bmp_info.width * bmp_info.height *
        (bmp_info.bpp / CHAR_BIT) * sizeof (uint8_t);

    uint8_t * const bmp_pixels = malloc(bmp_img_bytes);

    if (bmp_pixels == NULL)
    {
        fputs("error: malloc(): could not allocate memory for Bitmap\n",
            stderr);
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
    bmp_file.fsize = sizeof (bmp_file) + sizeof (bmp_info) + bmp_img_bytes;

    // calculate Bitmap pixels

    for (size_t y=0; y < (size_t)bmp_info.height; ++y)
    {
        for (size_t x=0; x < (size_t)bmp_info.width; ++x)
        {
#define xy_offset   ((y * bmp_info.width + x) * bmp_info.bpp / CHAR_BIT)
            uint8_t * const red     = bmp_pixels + xy_offset + 0;
            uint8_t * const green   = bmp_pixels + xy_offset + 1;
            uint8_t * const blue    = bmp_pixels + xy_offset + 2;
#undef xy_offset

            const double pi     = acos(-1.0);
            const double tau    = 2.0 * pi;

            const double xr     = (double)x / bmp_info.width;   // X Ratio
            const double yr     = (double)y / bmp_info.height;  // Y Ratio
            // XY (Diagonal) Ratio
            const double xyr    = (double)(x + y) /
                (bmp_info.width + bmp_info.height);

            // mysterious magic of forgotten high school math, go!
            *red    = get_color(sin(pi * xr));
            *green  = get_color(sin(pi * yr));
            *blue   = get_color(sin(tau * xyr));
        }
    }

    // write Bitmap data

    if (fwrite(&bmp_file, sizeof bmp_file, 1, output_file) != 1)
    {
        fputs("error: fwrite(): could not write Bitmap file header\n", stderr);
        fclose(output_file);
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    if (fwrite(&bmp_info, sizeof bmp_info, 1, output_file) != 1)
    {
        fputs("error: fwrite(): could not write Bitmap info header\n", stderr);
        fclose(output_file);
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    if (fwrite(bmp_pixels, sizeof bmp_pixels[0], bmp_img_bytes, output_file) !=
        bmp_img_bytes)
    {
        fputs("error: fwrite(): could not write Bitmap pixel data\n", stderr);
        fclose(output_file);
        free(bmp_pixels);
        return EXIT_FAILURE;
    }

    fclose(output_file);
    free(bmp_pixels);
    return EXIT_SUCCESS;
}
