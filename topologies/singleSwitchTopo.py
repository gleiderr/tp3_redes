#!/usr/bin/python                                                                            
                                                                                             
from mininet.cli import CLI
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.node import OVSController

class SingleSwitchTopo(Topo):
    "Single switch connected to n hosts."
    def build(self):
        switch = self.addSwitch('s1')
        # Python's range(N) generates 0..N-1
        for h in range(6):
            host = self.addHost('h%s' % (h + 1))
            self.addLink(host, switch)

def simpleTest():
    #c = RemoteController('c', '0.0.0.0', 6633)
    "Create and test a simple network"
    topo = SingleSwitchTopo()
    net = Mininet(topo=topo, controller = OVSController)
    #net.addController(c)
    net.start()

    CLI(net)

    print "Dumping host connections"
    dumpNodeConnections(net.hosts)
    print "Testing network connectivity"
    net.pingAll()
    net.stop()

if __name__ == '__main__':
    # Tell mininet to print useful information
    setLogLevel('info')
    simpleTest()