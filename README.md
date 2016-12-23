# Generates PDF files filled with QR codes 

A tool to store data encoded as QR codes to PDF documents.

## CLI tool

### Synopsis

    qr2pdf [-m margin] [-g gaps] [-w workers] [SIZE]


### Description

Options are as follows:

    `-m margin`
        Margin of a page in results file;
    `-g gaps`
        Gap between codes;
    `-w workers`
        Number of workers processing QR codes

    SIZE Size of QR code image in millimeters.

The CLI tool responds with a PDF file name.
Main thread starts reading input from standard input.
Worker threads start to generate QR codes from each line in the input file.
Once input ends and all workers have finished their tasks,
the PDF file is saved. 

## Development

Install build deps:

    sudo apt install librsvg2-dev libgdk-pixbuf2.0-dev libqrencode-dev
