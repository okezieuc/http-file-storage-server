# HTTP File Storage Service in C

A simple HTTP file storage service written in C.

This program starts a static file server on port 8000. This server responds to `GET` requests with the relevant file in the `static` folder. And it uploads files contained in the body of `POST` requests to the static folder. 

## Usage Guide
If you have `make` installed, you can run the program with the command
```bash
make run
```

If you do not have `make`` installed, you can run the program by running the following commands:
```
gcc -o server.out server.c
./server.out
```

### Making GET requests
When you make a GET request, the server returns the file if it exists and a 404 error otherwise.

```bash
curl 127.0.0.1:8000/index.html 

--- OUTPUT ---       

<html>
    Static File Server
</html>
```

```bash
curl 127.0.0.1:8000/non-existing-file

--- OUTPUT ---

404: Resource Not Found
```

### Making POST requests
> The program does not handle POST requests from CURL correctly, so the instructions below will not work exactly as expected. File upload requests made as binary with Postman work fine, though. No other clients have been tested.

```bash
curl -X POST -F "file=@/path/to/your/file.txt" http://example.com/upload

--- OUTPUT ---

File uploaded
7Zcd2WFpvf2rsuTI
```


## Project Limitations & Restrictions
- The concept for this project was to create a server that could serve files and upload files, similar to Filebin or Imgur. Because of this, it is not designed to respond to paths endwith with a `/` with the relevant `index.html` file in the way a regular web server would.
- The server does not handle POST requests from some clients correctly.
- The server is vulnerable to path traversal attacks, which could allow a negative actor to access an arbitrary file on the running systems file system.
- The server does not append the correct file extensions to uploaded files. When a file is uploaded to the server, it is stored as an extensionless file. This could potentially be done by looking at the `Content-Type` header of requests.

## Credits
This project is inspired by and built with knowledge from the static file server example in the book ['Hacking: The Art of Exploitation'](https://nostarch.com/hacking2.htm).