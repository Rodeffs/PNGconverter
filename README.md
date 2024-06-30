# PNG Converter

### Description

This program can convert any file to PNG and then convert it back to original without losing data

### Why is it useful

Some websites, say imageboards, don't allow to post any files besides images. If you wanted to share some other file via them, you'd have to upload it somewhere and then share a link and those could be banned too

By utilising this program, you can convert any file to PNG, send it and anybody can convert it back without data loss. Since PNG format utilises compression, the resulting PNG might even be smaller in size than input

Also it's just a cool data visualization tool

### Compilation

You need to have installed libpng library prior to compilation

If you also have cmake installed, then the compilation process would look something like this:

```
git clone https://github.com/Rodeffs/PNGconverter.git /where/to/clone/repo

cd /your/build/directory

cmake /where/to/clone/repo

cmake --build .

./PNGconverter [options]
```

### Usage

```
-e [file]       encode the given file

-d [file]       decode the given file

-o [file]       output the encoded/decoded data to a given file

-h [integer]    the height of the PNG to be encoded

-w [integer]    the width of the PNG to be encoded

NOTE: if no height or width were entered, the program will automatically pick the best resolution

--help          display this help message
```

### Example

Let's say, I've got some file named example_input.txt and I want to encode it

First, I need compile the program, then run:

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

### Limitations

The default limit for PNG size in libpng is 10^6 by 10^6 pixels. I decided to not change the limit for the sake of safety

This means that the maximum possible size of input file is ~2.7 TiB, which is good enough for most cases

Besides, I doubt that any site or image viewer would be able to display such an image without issues anyway

The only major problem that this program has is that the input file is stored in RAM during both encoding and decoding. And while there is a way to fix that by utilizing progressive reading, due to the lack of decent documentation and my lack of motivation, I'm not going to bother with it

So it means that don't try to encode/decode files that exceed your RAM capacity :P

#### P.S.

This project was inspired by [this video](https://youtu.be/Y2L7_2Wvycc?si=zESPOYajcYqceoiB "YouTube link")
