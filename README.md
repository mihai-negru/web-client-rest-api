# Negru Mihai 323CD: PCom Assignment No. 3


## `Getting Started`


This `README` tries to explain how the assignment was done.
* Mainly here we will discuss the implementations and the motivation for choosing one idea over
  another.
* The main implementation can be found in the following **Classes**:
    * [TcpSocket](./src/tcpsocket.cpp) - implements simple sockets functions as openning a
      connection, sending or receiving a stream of bytes. Futher details will be discussed below.
    * [HttpHandler](./src/httphandler.cpp) - implements a handler for parsing and generating http
      request or responses. This class does not contain all the neccessary fields for a full *HTTP*
      request/response, because it is a minimal implementation of this protocol.
    * [InputCommand](./src/httphandler.cpp) - A class following the command design pattern, that
      extracts input commands from the user. Futher details below.
    * [Errors](./src/errors.cpp) - Simple error class in order to show messages for the user and for
      debug purpouses.
    * [Client](./src/client.cpp) - This class encapsulated the usage of all of the classes above and
      handles the requests to the server and responses from the server.
    * [Main](./src/main.cpp) - Initializes the `client` class and starts the session for the user.

In the following sections we will go through the following ideas:
* How to compile and run the program.
* C++ Language over C Language.
* Each class in details and what functionalities bring on the table.
* Work flow of the main thread.


### `Compilation`


In order to compile the program and to make it run, make sure that you meet the requirements:
* You are on a Linux environment.
* You have installed **g++** compiler for C++ Language **(no clangd)**.
* For checking memory leaks make sure you have installed **valgrind**.

After you checked the requirements open the project root and execute the following commands in a
terminal:

```bash
    make        # Compile the program, generates *main* elf file
    make run    # In order to run the program
```

You can dirrectly can `make run` the program, however make sure to wait a bit in order to get
program compiled.

If you want to check the memory leaks, which will be my recommandation, you will have to run:

```bash
    make val    # Run the program under valgrind program
```

>**NOTE:** The object files will be found under the *build* folder.

>**NOTE:** For more information about compilation, running and so on, check the
>[Makefile](./build/Makefile).

>**NOTE:** This assignment was solved on the **USO** virtual machine for MAC ARM, if something does
>not work properly please refer to compile it on the virtual machine.


### `Motivation`


Even though this assignment could be solved in *C Language*, I have choosed C++ because of:
* Possibility to program in an **Object Oriented Style**, in order to encapsulate the
  functionalities for each class and in order to be easy in the future to add new functionalities or
  to extend the current api.
* Very huge standard library.
* `std::string`, I am really borded of using **`char *`** instead of strings.
* References, don't get me wrong, but raw pointers are not a way to go for styled and bug-free code.
* Destructors which take care of freeing heap memory.


### `TcpSocket`


This class maintain the functioanlity of sockets. Basically we will use **tcp sockets** and will
build our application over the TCP Protocol. This class contain 2 main methods:
* [send_stream](./src/tcpsocket.cpp)
* [recv_stream](./src/tcpsocket.cpp)

Both methods get as a input a **HttpHandler**, where each handler has to have the **byte_stream()**
method, which will serialize the http request into a stream of bytes in order to send over the
network.

Talking about receiving messages, each handler has a method **add_bytes()**, which takes a stream of
bytes and a length and adds more information to the current http response.

The `send_stream()` and `recv_stream()`, know nothing about how a http request/response is build or
which fields should fill in. They just know how to send a stream of bytes over the network and how
to receive a stream of bytes from the network. This behaviour is very welcomed, because the
functionality can be extended to other handlers, for example we can have a interface called
`NetHandler`, where `HttpHandler` and maybe `HttpsHandler` and so on implement this interface in
order to send or receive any request or responses.

>**NOTE:** Talking about receiving, the single mention is that every response from the server should
>have at least a valid **header** in order to be parsed successfully.


### `HttpHandler`


This class follows the **Builder design pattern**, because it has a lot of optional fields that can
be set or not in order to construct a valid http message.

The `HttpHandler` contains mainly three important methods:
* [generate_get_request](./src/httphandler.cpp) - constructs a get request based on the setted
  options.
* [generate_delete_request](./src/httphandler.cpp) - constructs a delete request based on the setted
  options.
* [generate_post_request](./src/httphandler.cpp) - constructs a post request based on the setted
  options.

The class contains mainly a header and a body in order to split the functionalities into **header**
functions and **body** functions as setting a new address or maybe to set a new host.

As discussed above the **HttpHandler** in order to be send or received from the network it has to
implement two methods:
* [add_bytes](./src/httphandler.cpp) - in order to add data to a current http response.
* [byte_stream](./src/httphandler.cpp) - in order to serialize a http request for sending.

>**NOTE:** The methods discussed above are just an interpretation of the functions from the
>laboratory No. 9 from Pcom course.


### `InputCommand`


This class follows the **Command design pattern**, it takes input from the standard input, validates
it and if validation has passed then the command is send forward in order to be processed and send
to the server.


The `InputCommand` class has a handler method for each available command.

The validations are as follows:
* `username and password` - the server allows username and password with spaces and a lot of other
  characters, my validation consist of the following characters:
  **qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_-.**

The method `is_plain_word` checks the username and password upon other incompatible characters.

* `Any other strings` - the server also permit a lot of things, however for simple strings, I permit
  to contain spaces and other special characters as digits and simbols (**._-+<>/|;:!?`~[]{}()**),
  not just letters.

The method `is_text` checks the string values upon other incompatible characters.

* `Numbers` - as follows every number sent to standart input is first read as a string, and then the
  program decides if it is a valid number or not, the number should contain just digits and nothing
  else.

The method `is_number` checks if the string number is indeed a number.

>**NOTE:** If any of above checks fail, then an error is thrown and printed on the console.

The class contains a special method which is `listen_for_command`, which waits for user input then
calls other methods in order to process the command, if the function does not throw any errors it is
considered that parsing the input went successfully and other classes can perform actions regarding
the read command.

Also the command class reads input in a json object, each `handler` sets the fields for the json
object. This is a convinience, because the httpformat request for this task a JSON format in order
to send over the network.


### `Errors`


This class extends the standard interface `std::exception` and the functionality that is added is
just to add a "*[ERROR]*" header to any error message, also adds the RED color to the message in
order to emphasize the idea of an error message.


This error is thorwn by any of the classes discussed above when a error is found.

The errors can be:
* Wrong user input.
* Neccessary fields for creating sockets or httprequest are empty.
* Server returns status codes like 404 or 400 or any other error status codes.
* Sending or receiving from the network has failed.
* Parsing messages in a http request/response failed.

>**NOTE:** Error messages are quite self explanatories and appear right after the error was spotted.


### `Client`


This class is the core of this program, it has a method names `start_session`, that calls the
command handler in order to parse the user input. Then the command handler returs the type of the
command, if the command is unknown an error message is thrown and catched by the `Client` class and
printed out. If the command handler returns a valid command type, then the `Client` calls the
handler for each request, where for each handler, the methods call the `http handler` in order to
create and fill out the command header and body.

After the http message is created and generated, the Client calls the `TcpSocket` class in order to
send the message over the network and waits for an response.

When the response is received, the handler methods look up at the status code and print an `OK`
message to the console in order to insform the user that everything went alright, or throw an error
which is catched by the `start_session` method.

This behaviour is the same for each request handler from the `Client` class.

The Clients handlers work as follows:
* `register` - if the status code is not **201** it means that the username is already registered.
* `login` - if the status code is not **200** it means that the user insetred wrong credentials. If
  the user tries to login the second time while it is logged in, an error message will be thrown
  announcing the user that he/she is already logged in. In order to log with another account he/she
  has to logout first (which happens naturally).
* `enter_library` - if the status code is not **200** it means that the session cookie is
  depricated. This behaviour cannot happen in normal conditions, because the `Client` does not
  permit to enter the library if the user is not logged in, which is tested by a boolean variable.
  So in order to enter library, you will always have a valid cookie, but who knows what can happen.
* `get_books` - works the same as the enter_library command. When the user receives the http
  response with all the books it is asked how many books to print out of the total number, which has
  to be a valid number, otherwise an error is thrown. Also if server returns an empty array of books
  an error is thrown by the `handler`.
* `get_book` - works the same as get_books, if the input id is valid then the book is printed,
  otherwise an error is thrown to the `start_session` command.
* `add_book` - first the information about the book are insrted by the user and then the client
  processes the data. If the server returns a code of **400** or anything else an error will be
  thrown, else the user will be informed that his/her book was inserted successfully in the library.
  Also if the status code is not 400, or not 200 it means that the library token is depricated.
* `delete_book` - works just as `get_book`, if all went good the book will be removed, else an error
  will be thrown.
* `logout` - can happen just when the user is already logged, which is tested by a boolean value. It
  clears the session cookie, the library access and sets the client as logged out and a request for
  logging out is send to the server. 

The `Client` will not throw any errors to the main caller of the `Client` class, the one case can be
if the client cannot construct itself, which by default means that the program cannot continue. 

### `Main`

This is the file that contain the `main` function that initializes a client and starts the session
for reading user inputs.

The main function first, defines a `mapping` for the valid commands that the `Client` class knows
with the **url** paths regarding the server side, it is the responsability of the main function to
set the correct urls for every command, because the client does not know what is the api for the
selected server, it just know how to handle the desired command and how to generate http requests
and how to get http responses.


### `Feedback`

I really enjoyed solving this assignment, because at least it teaches us to work with really servers
which will be usefull in the future.

I did not like the way that is was ambiguous how should we handle some user input errors, or how to
handle some of the server responses.

For any details or clarifications be free to contact me to the following email address
"determinant289@gmail.com"
