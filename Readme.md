# Network Simulation Programs


## Router Simulation Program

### Description

The router simulation program allows users to create a network of routers and analyze routing tables to find the shortest path between routers using Dijkstra's algorithm.

### Prerequisites

- C++ compiler supporting C++11 standard or later

### How to Run

1. Clone the repository or download the source code files.
2. Compile the source code using a C++ compiler.
3. Run the compiled executable.

### Usage

Upon running the program, users are presented with a menu to perform various actions:

- **Create Router and Add Neighbors**: Create routers and specify incoming and outgoing neighbors.
- **Find Path**: Find the shortest path between two routers.
- **Print Routing Tables**: Display routing tables of all routers in the network.
- **Delete Router**: Delete a router from the network.
- **Exit**: Exit the program.

Follow the on-screen instructions to perform desired actions.



## CSMA Simulation Program

### Description

The CSMA simulation program models the behavior of nodes contending for access to a shared communication channel using the Carrier Sense Multiple Access (CSMA) protocol.


### Usage

Upon running the program, users are prompted to input various configuration parameters for the simulation:

- Number of nodes in the network
- Maximum backoff value
- Backoff value for the channel being idle
- Simulation duration
- Maximum transmission time
- Transmission probability (a value between 0 and 1)

Follow the on-screen instructions to input these parameters.

### CSMA Simulation

The program simulates nodes contending for access to the communication channel over the specified duration. It performs backoff procedures and adjusts transmission attempts based on collisions using the CSMA algorithm.

After the simulation completes, the program displays statistics for each node, including successful transmissions and collisions, as well as more detailed statistics such as collisions before successful transmission and time before successful transmission.


## Stop-and-Wait Protocol Simulation

### Description

The stop-and-wait protocol simulation program emulates a simple reliable data transmission protocol between a sender and a receiver over a network connection. It demonstrates the process of sending data frames from the sender to the receiver and acknowledging them using a stop-and-wait mechanism.


### Usage

Upon running the sender program, users are prompted to input the number of frames to send. The sender then transmits the frames to the receiver over the network connection. The receiver simulates various scenarios, such as packet loss, delay, or successful transmission, and sends acknowledgments back to the sender accordingly.

The sender handles acknowledgment responses and retransmits frames as necessary until all frames are successfully received and acknowledged by the receiver.


Upon running the client program, it establishes a connection with the server and sends data frames to the server. The server receives the frames, processes them, and sends acknowledgment responses back to the client.

The communication between the server and the client is based on a basic stop-and-wait protocol, ensuring reliable transmission of data over the TCP/IP connection.

