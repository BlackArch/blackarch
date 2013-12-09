#!/usr/bin/env python

# 0trace.py
# Jon Oberheide <jon@oberheide.org>
# http://jon.oberheide.org/0trace/

import dnet, dpkt, pcap
import sys, sets, time, struct, select, threading

MAX_HOPS = 30
SEQ_DELTA = 3

class AckProbe(threading.Thread):
    def __init__ (self, ip, tcp):
        self.ip = ip
        self.tcp = tcp
        self.tcp.flags = dpkt.tcp.TH_ACK
        self.tcp.data = struct.pack('H', 0)
        self.sock = dnet.ip()
        threading.Thread.__init__(self)

    def run(self):
        for i in xrange(1, SEQ_DELTA+1):
            self.tcp.seq += i
            for j in xrange(MAX_HOPS):
                self.ip.id = j
                self.ip.ttl = j
                buf = dnet.ip_checksum(str(self.ip))
                self.sock.send(buf)
                time.sleep(0.001)

class Monitor(threading.Thread):
    def __init__ (self, filter):
        self.hops = []
        self.filter = filter
        self.reached = False
        threading.Thread.__init__(self)

    def run(self):
        pc = pcap.pcap()
        pc.setnonblock(True)
        pc.setfilter(self.filter)
        while True:
            rfd, wfd, efd = select.select([pc.fileno()], [], [], 3)
            if not rfd:
                break
            ts, pkt = pc.next()
            eth = dpkt.ethernet.Ethernet(pkt)
            ip = eth.data
            if ip.p == dpkt.ip.IP_PROTO_TCP:
                self.reached = True
            elif ip.p == dpkt.ip.IP_PROTO_ICMP:
                icmp = ip.data
                if icmp.type == dpkt.icmp.ICMP_TIMEXCEED and \
                   icmp.code == dpkt.icmp.ICMP_TIMEXCEED_INTRANS and \
                   icmp.data.ip.id < MAX_HOPS and \
                   icmp.data.ip.id > 0 and \
                   icmp.data.ip.ttl > 0:
                    self.hops.append((icmp.data.ip.id, ip.src))

class ZeroTrace:
    def main(self):
        if len(sys.argv) < 4:
            print 'Usage: %s interface target_ip target_port' % sys.argv[0]
            sys.exit(1)

        print '0trace.py by Jon Oberheide <jon@oberheide.org>'

        interface = sys.argv[1]
        target_ip = dnet.addr(sys.argv[2])
        target_port = sys.argv[3]
        filter = 'src host %s and src port %s and (tcp[13] & 0x17 == 0x10)' % \
                 (target_ip, target_port)

        pc = pcap.pcap(interface)
        pc.setfilter(filter)
        pc.setnonblock(True)

        print '[+] Waiting for traffic from target on %s...' % interface

        while True:
            rfd, wfd, efd = select.select([pc.fileno()], [], [])
            if rfd:
                ts, pkt = pc.next()
                break

        print '[+] Traffic acquired, waiting for a gap...'

        while True:
            rfd, wfd, efd = select.select([pc.fileno()], [], [], 3)
            if not rfd:
                break
            ts, pkt = pc.next()

        eth = dpkt.ethernet.Ethernet(pkt)
        ip = eth.data
        tcp = ip.data
        ip.src, ip.dst = ip.dst, ip.src
        tcp.seq, tcp.ack = tcp.ack, tcp.seq
        tcp.sport, tcp.dport = tcp.dport, tcp.sport

        print '[+] Target acquired: %s:%s -> %s:%s (%s/%s)' % \
              (dnet.ip_ntoa(ip.src), tcp.sport,
               dnet.ip_ntoa(ip.dst), tcp.dport, 
               tcp.seq, tcp.ack)

        print '[+] Setting up a sniffer...'

        m = Monitor('icmp or (%s)' % filter)
        m.start()
        time.sleep(1)

        print '[+] Sending probes...'

        p = AckProbe(ip, tcp)
        p.start()
        p.join()
        m.join(5)

        print
        print 'TRACE RESULTS'
        print '-------------'

        m.hops = list(sets.Set(m.hops))
        m.hops.sort()
        for id, hop in m.hops:
            print '%d %s' % (id, dnet.ip_ntoa(hop))

        if m.reached:
            print 'Target reached.'
        else:
            print 'Probe rejected by target.'
        print

if __name__ == '__main__':
    z = ZeroTrace().main()
