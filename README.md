# XML to TLV converter
#### A simple project showing how to create a Text-Length-Value binary from an XML dataset.

## Installation

This package is distributed as source code, so you need to build it for further use.

- You can simply open this project in [CLion](https://www.jetbrains.com/clion/) and build using the build button
- Also, if desired, you can build it yourself using CMake

## Tests

`xml_to_tlv` is covered by tests. If you want to run them, you should install [ceedling](http://www.throwtheswitch.org/ceedling).
To run tests use this command in project root:

```sh
ceedling test
```

## Usage

You should specify input file name by using --f attribute:

```sh
./xml_to_tlv --f input_file_name.xml
```
And it will create `out.tlv` file for you. If you want specify output file name, please, add:

```sh
--o output_file_name.tlv
```
 
## Limitations

This application is only an example of working with TLV files, so the following assumptions are used in it:
- Dynamic memory is used to store data. This code is not expected to run on an MCU. If you need to run on hardware, it is recommended to modify the code to use only statically allocated memory
- It is assumed that the size of the `long int` type is the same on the machine that converts the XML and on the machine that receives the resulting TLV
- The number of tags for the `tag` field is set "at the discretion of the developer"
