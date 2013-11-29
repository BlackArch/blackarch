#!/usr/env/python

# Version 0.1
# http://pykismetearth.google.pages.com
# Send email to pykismetearth@gmail.com

# Usage: python pykismetearth.py Kismet-Sep-07-2006 

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import sys
import random
import urllib
from xml.sax import make_parser, handler


class KismetFileHandler(handler.ContentHandler):
    """This class parses kismet .gps and .xml files.
    The idea is to produce a folder for each ssid with a signal strength map, metadata about the AP, and a best guess location (currently placed at location of strongest power).

    """

    def __init__(self):

        self.signal_strength={}
        self.color={}
        self.max_signal_strength={}
        self.max_signal_strength_coords={}
        self.net={}
        self.path=""
        self.output=""
        self.tag="none"
        self.interested=False

    def startElement(self, name, attrs):
        
        if name=="gps-run":
            #This is the first thing in our gps file.
            self.output="""<?xml version='1.0' encoding='UTF-8'?>
            <kml xmlns='http://earth.google.com/kml/2.0'>
            <Folder>
            <name>"""+sys.argv[1]+"""</name>
            <description>"""+attrs["start-time"]+"""</description>
            <visibility>1</visibility>
            <open>1</open>"""

        elif name=="wireless-network":
            if attrs["type"]=="infrastructure":
                #We only care about infrastructure networks.
                self.interested=True
                self.net["cloaked"]=attrs["cloaked"]
                self.net["first-seen"]=attrs["first-time"]
                self.net["last-seen"]=attrs["last-time"]

        elif name=="SSID" and self.interested:
            self.tag="ssid"
        elif name=="BSSID" and self.interested:
            self.tag="bssid"
        elif name=="channel" and self.interested:
            self.tag=name
        elif name=="maxrate" and self.interested:
            self.tag=name
        elif name=="maxseenrate" and self.interested:
            self.tag=name
        elif name=="carrier" and self.interested:
            self.tag=name
        elif name=="encryption" and self.interested:
            self.tag=name
        elif name=="LLC" and self.interested:
            self.tag="llc"
        elif name=="data" and self.interested:
            self.tag=name
        elif name=="crypt" and self.interested:
            self.tag=name
        elif name=="weak" and self.interested:
            self.tag=name
        elif name=="dupeiv" and self.interested:
            self.tag=name
        elif name=="total" and self.interested:
            self.tag=name
        elif name=="datasize" and self.interested:
            self.tag=name
        elif name=="min-lat" and self.interested:
            self.tag=name
        elif name=="min-lon" and self.interested:
            self.tag=name
        elif name=="min-alt" and self.interested:
            self.tag=name
        elif name=="min-spd" and self.interested:
            self.tag=name
        elif name=="max-lat" and self.interested:
            self.tag=name
        elif name=="max-lon" and self.interested:
            self.tag=name
        elif name=="max-alt" and self.interested:
            self.tag=name
        elif name=="max-spd" and self.interested:
            self.tag=name

        elif name=="gps-point":
            
            #Storlat/long/altwarpath track
            self.path = self.path + attrs["lon"] + "," + attrs["lat"] + ",0 "
            if attrs["bssid"] != "GP:SD:TR:AC:KL:OG":
                try:
                    #Create a point for each gps measurement.  Build an array
                    #of measurements per bssid. 
                    self.signal_strength[attrs["bssid"]]+="""
                        <Placemark>
                        <visibility>0</visibility>
                        <open>0</open>
                        <Style>
                            <LineStyle>
                                <color>""" + self.color[attrs["bssid"]] + """</color>
                                <width>4</width>
                            </LineStyle>
                        </Style>
                        <LineString>
                            <extrude>1</extrude>
                            <tessellate>1</tessellate>
                            <altitudeMode>relativeToGround</altitudeMode>
                            <coordinates>""" + attrs["lon"]+","+attrs["lat"]+","+attrs["signal"]+"""
                        </coordinates>
                        </LineString>
                    </Placemark>"""
                    
                    try: 
                        if int(attrs["signal"]) > int(self.max_signal_strength[attrs["bssid"]]):
                            self.max_signal_strength[attrs["bssid"]]=attrs["signal"]
                            self.max_signal_strength_coords[attrs["bssid"]]=[attrs["lon"],attrs["lat"]]
                    except KeyError:
                        self.max_signal_strength[attrs["bssid"]]=attrs["signal"]
                        self.max_signal_strength_coords[attrs["bssid"]]=[attrs["lon"],attrs["lat"]]

                except KeyError:
                    #This is a new BSSID, assign it a color and add in the first point
                    self.color[attrs["bssid"]]=hex(random.randrange(0xFF000000,0xFFFFFFFF))[2:-1]
                    self.signal_strength[attrs["bssid"]]="""
                        <Placemark>
                        <visibility>0</visibility>
                        <open>0</open>
                        <Style>
                            <LineStyle>
                                <color>""" + self.color[attrs["bssid"]] + """</color>
                                <width>4</width>
                            </LineStyle>
                        </Style>
                        <LineString>
                            <extrude>1</extrude>
                            <tessellate>1</tessellate>
                            <altitudeMode>relativeToGround</altitudeMode>
                            <coordinates>""" + attrs["lon"]+","+attrs["lat"]+","+attrs["signal"]+"""
                        </coordinates>
                        </LineString>
                    </Placemark>"""

        else:
            self.tag="none"

    def endElement(self, name):
        
        if name=="gps-run":
            temp=self.path.split(" ",1)[0]
            startlat=temp.split(",")[1]
            startlong=temp.split(",")[0]

            #Write out the warpath route
            self.output+="""
                
                <Placemark>
                    <description>Warpath Route Taken</description>
                    <name>Route</name>
                    <LookAt>
                        <longitude>"""+startlong+"""</longitude>
                        <latitude>"""+startlat+"""</latitude>
                        <range>1000</range>
                        <tilt>54</tilt>
                        <heading>-35</heading>
                    </LookAt>
                    <visibility>1</visibility>
                    <open>0</open>
                    <Style>
                        <LineStyle>
                            <color>BB0000FF</color>
                            <width>4</width>
                        </LineStyle>
                    </Style>
                    <LineString>
                        <extrude>0</extrude>
                        <tessellate>1</tessellate>
                        <altitudeMode>clampedToGround</altitudeMode> 
                        <coordinates>""" + self.path + """
                    </coordinates>
                    </LineString>
                </Placemark>"""

        if name=="wireless-network" and self.interested:

            try:
                #People have ssid's with lots of funky chars that google earth doesn't like.
                ssid=urllib.quote(self.net["ssid"])
            except KeyError:
                #Must have been no ssid
                ssid="no-ssid"

            try:
                sig_str=self.signal_strength[self.net["bssid"]]
                max_sig_lon=self.max_signal_strength_coords[self.net["bssid"]][0]
                max_sig_lat=self.max_signal_strength_coords[self.net["bssid"]][1]
                lookat="""
                        <LookAt>
                            <longitude>%s</longitude>
                            <latitude>%s</latitude>
                            <range>100</range>
                            <tilt>54</tilt>
                            <heading>-35</heading>
                        </LookAt>
                """ % (max_sig_lon,max_sig_lat)

            except KeyError:
                #We don't have gps data for this MAC for whatever reason
                sig_str="<description>No GPS Data Available!!</description>"
                lookat=""
                max_sig_lon=0
                max_sig_lat=0
                

            try:
                if (self.net["encryption"].index("WPA")>=0) or self.net["encryption"].contains("TKIP"):
                    encryption="<font color=\"green\">%s</font>" % self.net["encryption"]
                    icon="node_closed.png"
            except ValueError:
                if self.net["encryption"]=="WEP":
                    encryption="<font color=\"blue\">%s</font>" % self.net["encryption"]
                    icon="node_open.png"
                elif self.net["encryption"]=="None":
                    encryption="<font color=\"red\">%s</font>" % self.net["encryption"]
                    icon="node_open.png"

            
            self.output+="""
                <Folder>
                <name>%s</name>
                %s
                <description><![CDATA[First-seen:%s<br>
                                Last-seen:%s<br><hr>
                                BSSID:%s<br>
                                channel:%s<br>
                                encryption:%s<br>
                                cloaked:%s<br>
                                max rate:%s<br><hr>
                                <b>GPS Coordinates</b><br>
                                Min lat/long/alt:%s lat, %s lon, %s alt<br>
                                Max lat/long/alt:%s lat, %s lon, %s alt<br><hr>
                                <b>Captured Packets</b><br>
                                llc:%s<br>
                                data:%s<br>
                                crypt:%s<br>
                                weak:%s<br>
                                dupeiv:%s<br>
                                total:%s<br>
                                total datasize captured:%s<br>
                                ]]></description>
                    <Placemark>
                        <name>%s</name>
                        <description>Location of max AP signal strength</description>
                        <visibility>0</visibility>
                        <open>0</open>
                        %s                           
                        <Style>
                            <icon>%s</icon>
                        </Style>
                        <Point>
                            <altitudeMode>clampedToGround</altitudeMode>
                            <extrude>0</extrude>
                            <tessellate>0</tessellate>
                            <coordinates>%s,%s,0</coordinates>
                        </Point>
                    </Placemark>
                <Folder>
                <name>Signal Strength Map</name>
                %s
                %s
                </Folder></Folder>""" % (ssid,lookat,self.net["first-seen"],self.net["last-seen"],self.net["bssid"],self.net["channel"],encryption,self.net["cloaked"],self.net["maxrate"],self.net["min-lat"],self.net["min-lon"],self.net["min-alt"],self.net["max-lat"],self.net["max-lon"],self.net["max-alt"],self.net["llc"],self.net["data"],self.net["crypt"],self.net["weak"],self.net["dupeiv"],self.net["total"],self.net["datasize"],ssid,lookat,icon,max_sig_lon,max_sig_lat,lookat,sig_str) 
            
            self.tag="none"
            self.net={}
            self.interested=False
            

    def characters(self,chars):

        #The characters output is a bit messy - throw away whitespace only stuff
        if self.interested and self.tag !="none" and len(chars.strip())!=0:
            try:
                if self.net[self.tag]:
                    #We already have a value for this key so append
                    self.net[self.tag]+=" "+str(chars)
            except KeyError:
                self.net[self.tag]=str(chars)
            except:
                print "Couldn't process string: %s" % chars
                    
    def endDocument(self):


        handle = open(sys.argv[1]+".kml","w+")
        handle.write(self.output)
        handle.write("</Folder>\n</kml>")


kismetparser = make_parser()
kismetparser.setContentHandler(KismetFileHandler())
kismetparser.parse(sys.argv[1]+".gps")
kismetparser.parse(sys.argv[1]+".xml")
