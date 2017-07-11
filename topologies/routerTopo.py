#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Node
from mininet.log import setLogLevel, info
from mininet.cli import CLI
from mininet.node import OVSController


class LinuxRouter( Node ):
    "A Node with IP forwarding enabled."

    def config( self, **params ):
        super( LinuxRouter, self).config( **params )
        # Enable forwarding on the router
        self.cmd( 'sysctl net.ipv4.ip_forward=1' )

    def terminate( self ):
        self.cmd( 'sysctl net.ipv4.ip_forward=0' )
        super( LinuxRouter, self ).terminate()


class NetworkTopo( Topo ):
    "A LinuxRouter connecting three IP subnets"

    def build( self, **_opts ):

        defaultIP = '192.168.1.1/24'  # IP address for r0-eth1
        router = self.addNode( 'r0', cls=LinuxRouter, ip=defaultIP )

        s1, s2, s3 = [ self.addSwitch( s ) for s in ( 's1', 's2', 's3' ) ]

        self.addLink( s1, router, intfName2='r0-eth1',
                      params2={ 'ip' : defaultIP } )  # for clarity
        self.addLink( s2, router, intfName2='r0-eth2',
                      params2={ 'ip' : '172.16.0.1/12' } )
        self.addLink( s3, router, intfName2='r0-eth3',
                      params2={ 'ip' : '10.0.0.1/8' } )

        for h in range(3):
            h = self.addHost('h1%s' % (h + 1), ip='192.168.1.%s/24' % (h + 100),
                           defaultRoute='via 192.168.1.1' )
            self.addLink(h, s1)

        for h in range(3):
            h = self.addHost('h2%s' % (h + 1), ip='172.16.0.%s/12' % (h + 100),
                           defaultRoute='via 172.16.0.1' )
            self.addLink(h, s2)

        for h in range(3):
            h = self.addHost('h3%s' % (h + 1), ip='10.0.0.%s/8' % (h + 100),
                           defaultRoute='via 10.0.0.1' )
            self.addLink(h, s3)


def run():
    "Test linux router"
    topo = NetworkTopo()
    net = Mininet( topo=topo, controller = OVSController )  # controller is used by s1-s3
    net.start()
    info( '*** Routing Table on Router:\n' )
    info( net[ 'r0' ].cmd( 'route' ) )
    CLI( net )
    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    run()