# HTTP File Storage Service in C

In this project, I will build a simple file storage service in C. This service will allow a user to upload files and fetch files over HTTP requests.

## How users will use this appliction

- To use this application, a user will send a POST HTTP request to the server with the contents of the file in the body of the applcation. It will then create a blob for the created file on the serverside.
- When a user sends a GET request to the server, we will return the file that the user wants to receive. Else, we will return a 404 error.
- When a user sends a HEAD request, we return what a HEAD request is normally supposed to return.
- This will run on the HTTP/1.1 protocol

## Further Improvements

Future versions of this application can support passwording files: When a user uploads a file, they can append a password to the file. When the file is being requested for from a client, the client will need to have entered the same password that was sent when the file was created for the user to receive access to the file.

Future versions might also implement an API that returns a password-like key that a user can send to the server when they wish to delete an item.

We might also implement a thing that looks at the extension of a filename and, based on the extension, adds the appropriate file type header to the response that is sent.

The first version of this application will work in HTTP.  If I am able to figure out a way to make the application to work over HTTPS, that will be great.
