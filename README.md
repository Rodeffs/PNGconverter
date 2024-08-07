# PNG Converter

## Description

This program can convert any file to PNG and then convert it back to original without losing data

## Why is it useful

Some websites, say imageboards, don't allow to post any files besides images. If you wanted to share some other file via them, you'd have to upload it somewhere and then share a link and those could be banned too

By utilising this program, you can convert any file to PNG, send it and anybody can convert it back without data loss. Since PNG format utilises compression, the resulting PNG might even be smaller in size than input

Also it's just a cool data visualization tool

## Usage

```
-e [file]       encode the given file

-d [file]       decode the given file

-o [file]       output the encoded/decoded data to a given file

-h [integer]    the height of the PNG to be encoded

-w [integer]    the width of the PNG to be encoded

NOTE: if no height or width were entered, the program will automatically pick the best resolution

--help          display this help message
```

## Compilation

You will need to install libpng and zlib libraries (if you're using Linux then by default your package manager should install zlib as a dependancy of libpng). I also suggest installing cmake and using a GCC compiler for Linux and MinGW compiler for Windows

Then the compilation process would look something like this:

```
cd <path to build directory>

cmake <path to source directory>

cmake --build .

./PNGconverter [options]
```

In case you're using Windows and variables PNG_LIBRARY, PNG_PNG_INCLUDE_DIR, ZLIB_LIBRARY and ZLIB_INCLUDE_DIR are not defined yet:

```
cd <path to build directory>

cmake <path to source directory> -D PNG_LIBRARY:FILEPATH=<path to libpng dll> -D PNG_PNG_INCLUDE_DIR:PATH=<path to libpng directory> -D ZLIB_LIBRARY:FILEPATH=<path to zlib dll> -D ZLIB_INCLUDE_DIR:PATH=<path to zlib directory>

cmake --build .

PNGconverter.exe [options]
```

## Example

Let's say, I've got some file named example_input.txt and I want to encode it. The command options in this case are:

```
PNGconverter -e example_input.txt -o example_output.png
```

The resulting image:

![Image](example_output.png "Example")

Next, if I wanted to convert it back, I'd run:

```
PNGconverter -d example_output.png -o example_output.txt
```

If I wanted the output image to be a different resolution, say 500 by 155, then the input would look like this:

```
PNGconverter -e example_input.txt -o example_output.png -w 500 -h 155
```

Now the resulting image is:

![Image](example_output2.png "Example 2")

It can also be decoded back to the original file with no problems

## Limitations

The default limit for PNG size in libpng is 10^6 by 10^6 pixels. I decided to not change the limit for the sake of safety

This means that the maximum possible size of input file is ~2.7 TiB, which is good enough for most cases

Besides, I doubt that any site or image viewer would be able to display such an image without issues anyway

The only major problem that this program has is that the input file is stored in RAM during both encoding and decoding. And while there is a way to fix that by utilizing progressive reading, due to the lack of decent documentation and my lack of motivation, I'm not going to bother with it

So it means that don't try to encode/decode files that exceed your RAM capacity :P

## P.S.

This project was inspired by [this video](https://youtu.be/Y2L7_2Wvycc?si=zESPOYajcYqceoiB "YouTube link")
