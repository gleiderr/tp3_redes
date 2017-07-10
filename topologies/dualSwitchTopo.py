#!/usr/bin/python                                                                            
                                                                                             
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel

class DualSwitchTopo(Topo):
    "Dual switch connected to 3 hosts."
    def build(self):
        switch1 = self.addSwitch('s1')
        # Python's range(N) generates 0..N-1
        for h in range(3):
            host = self.addHost('h1%s' % (h + 1))
            self.addLink(host, switch)

        switch2 = self.addSwitch('s2')
        # Python's range(N) generates 0..N-1
        for h in range(3):
            host = self.addHost('h2%s' % (h + 1))
            self.addLink(host, switch)

        self.addLink(switch1, switch2);

def simpleTest():
    "Create and test a simple network"
    topo = DualSwitchTopo()
    net = Mininet(topo)
    net.start()
    print "Dumping host connections"
    dumpNodeConnections(net.hosts)
    print "Testing network connectivity"
    net.pingAll()
    net.stop()

if __name__ == '__main__':
    # Tell mininet to print useful information
    setLogLevel('info')
    simpleTest()