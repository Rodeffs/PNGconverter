# PNG Converter

### Description

This program can convert any file to PNG and then convert it back to original

### Compiling

You need to have installed libpng library prior to compilation

### Usage

```
-i [file] or --input-file [file]        select the input file to read from

-o [file] or --output-file [file]       select the output file to write to

-e or --encode                          encoding, assumed by default

-d or --decode                          decoding
```

### Why is it useful

Some websites, say imageboards, don't allow to post any files besides images. If you wanted to share some other file via them, you'd have to upload it somewhere and then share a link (those could be banned too).

By utilising this program, you can convert this file to PNG, send it, and anybody can convert it back to original and since PNG format is lossless no data is lost

Also, again due to lossless compression, the resulting PNG is smaller than the input

### Limitations

The default limit for PNG size in libpng is 10^6 by 10^6 pixels. I decided to not change the limit for the sake of safety.

This means that the maximum possible size of input file is ~2.7 TiB, which is good enough for most cases 

### Example

I'll use libpng official documentation as an example input

Here, I named it example_input.txt

Then I compile the program and run it:

```
PNGconverter -i example_input.txt -o example_output.png
```

The resulting image:

![Image](example_output.png "Example")

Next, I want to convert it back:

```
PNGconverter -i example_output.png -o example_output.txt -d
```

The resulting output file is identical to the initial input
 