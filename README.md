# Create an IRC server in C++, fully compatible with an official client.

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network. </br> </br>

Rules </br>
• The server must be capable of handling multiple clients at the same time and never
hang. </br>
• Forking is not allowed. All I/O operations must be non-blocking. </br>
• Only 1 poll() (or equivalent) can be used for handling all these operations (read,
write, but also listen, and so forth).
Because you have to use non-blocking file descriptors, it is
possible to use read/recv or write/send functions with no poll()
(or equivalent), and your server wouldn’t be blocking.
But it would consume more system resources.
Thus, if you try to read/recv or write/send in any file descriptor
without using poll() (or equivalent), your grade will be 0. </br>
• Several IRC clients exist. You have to choose one of them as a reference. Your
reference client will be used during the evaluation process. </br>
• Your reference client must be able to connect to your server without encountering
any error. </br>
• Communication between client and server has to be done via TCP/IP (v4 or v6). </br>
◦ You must be able to authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client. </br>
◦ All the messages sent from one client to a channel have to be forwarded to
every other client that joined the channel. </br>
◦ You must have operators and regular users. </br>
◦ Then, you have to implement the commands that are specific to channel
operators
