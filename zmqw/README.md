# zmqw/libzmqw
Subscribe to hooks in agents to provide json based zmq output

# description
The zmq writer module subscribes to hooks provided in agents, to allow for notifications when interesting events occur.  Messages are published into a zmq queue running on a port specified at startup time on the command line.

# command line example
pimd --log=stdout --log-level=debug -M libfrrzmqw.so:"port=17171"

