# tcp-server

A very basic TCP echo server that accepts a single client.

## How to use:
0. If you don't have PuTTY or similar software, you can download PuTTY from [here](https://www.chiark.greenend.org.uk/~sgtatham/putty/), or use my TCP client [here](https://github.com/molnar-david/tcp-client)
1. Compile and run using Visual Studio (I used Visual Studio Community 2019)
2. In PuTTY, set Connection type to Raw, then connect to your own IP address through port 54000
3. Send any message you want to the server, it will echo it back for you

*Made with the help of the [Networking in C++](https://www.youtube.com/playlist?list=PLZo2FfoMkJeEogzRXEJeTb3xpA2RAzwCZ) YouTube series by [Sloan Kelly](https://www.youtube.com/c/sloankelly)*  
*The tutorial series didn't include a lot of error checking, so most of it I had to look up and write myself*
