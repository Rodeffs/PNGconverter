# PNG Converter

### Description

This program can convert any file to PNG and then convert it back to original

### Compiling

You need to have installed libpng library prior to compilation

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

### Why is it useful

Some websites, say imageboards, don't allow to post any files besides images. If you wanted to share some other file via them, you'd have to upload it somewhere and then share a link (those could be banned too)

By utilising this program, you can convert this file to PNG, send it, and anybody can convert it back to original and since PNG format is lossless no data is lost

Also, again due to lossless compression, the resulting PNG is smaller than the input

### Limitations

The default limit for PNG size in libpng is 10^6 by 10^6 pixels. I decided to not change the limit for the sake of safety.

This means that the maximum possible size of input file is ~2.7 TiB, which is good enough for most cases 

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
