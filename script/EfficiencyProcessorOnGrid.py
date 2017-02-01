#!/usr/bin/env python

import lfc2 as lfc

import os
import sys
from os import path
import time

sys.path.insert(0 , '/gridgroup/ilc/garillot/SDHCALMarlinProcessor/script')

import EfficiencyProcessor

if len(sys.argv) < 4 :
	sys.exit('Error : too few arguments')

os.environ["LFC_HOST"] = 'grid-lfc.desy.de'

qbar = sys.argv[1]
delta = sys.argv[2]
d = sys.argv[3]

dir = '/grid/calice/SDHCAL/garillot/PolyaStudies/CalorimeterHit'

print ('Searching files in ' + dir)

lfcDir = lfc.lfc_opendir(dir)

counter = 0

while not lfcDir and counter < 50 :
	time.sleep(5)
	lfcDir = lfc.lfc_opendir(dir)
	counter = counter + 1

if not lfcDir :
	print 'Dir not ok or not accessible'
	sys.exit(1)


fileList = []

while True :
	entry = lfc.lfc_readdir(lfcDir)

	if entry == None :
		break

	if qbar + '_' + delta + '_' + d in entry.d_name :
		fileList.append(entry.d_name)

print 'File List :'
print fileList

#download files

for file in fileList :
	counter = 0
	success = False

	while not success and counter < 50 :
		p = Popen( ['/gridgroup/ilc/garillot/downloadOnGrid.py' , file , dir] )
		output, error = p.communicate()
		counter = counter + 1
		if p.returncode != 0 :
			time.sleep(5)
		else :
			success = True
			break

	if not success :
		print 'Access to /gridgroup/ilc/garillot/downloadOnGrid.py has failed'
		sys.exit(1) 



#source('/home/garillot/ilcsoft/v01-17-08/init_ilcsoft.sh')
os.environ["MARLIN"] = '/gridgroup/gridsoft/ipnls/ilc/v01-17-08/Marlin/v01-07'
os.environ["PATH"] = '/gridgroup/gridsoft/ipnls/ilc/v01-17-08/Marlin/v01-07/bin' + ':' + os.environ["PATH"]
os.environ["MARLIN_DLL"] = '/gridgroup/ilc/garillot/SDHCALMarlinProcessor/lib/libsdhcalMarlin.so'


a = EfficiencyProcessor.Params()
a.collectionName = 'HcalEndcapAnalog'
a.thresholds = '0.114 0.14 0.155714 0.171429 0.187143 0.202857 0.218571 0.234286 0.25 0.265714 0.281429 0.298571 0.314286 0.33 0.345714 0.361429 0.377143 0.392857 0.408571 0.424286 0.4 0.6125 0.825 1.0375 1.2375 1.45 1.6625 1.875 2.0875 2.3 2.5 2.7125 2.925 3.1375 3.35 3.5625 3.7625 3.975 4.1875 4.29448 5.33742 6.38037 7.48466 8.52761 9.57055 10.6135 11.6564 12.6994 13.8037 14.8466 15.8896 16.9325 17.9755 19.0184 20.0613 21.1656 22.2086 23.2515'

EfficiencyProcessor.launch(a , fileList)


outputDir = '/grid/calice/SDHCAL/garillot/PolyaStudies/MulResults'

output = 'map_' + qbar + '_' + delta + '_' + d + '.root'


os.system('mv map.root ' + output)

#Upload files

counter = 0
success = False
while not success and counter < 50 :
	p = Popen( ['/gridgroup/ilc/garillot/uploadOnGrid.py' , file , dir] )
	output, error = p.communicate()
	counter = counter + 1
	if p.returncode != 0 :
		time.sleep(5)
	else :
		success = True
		break

if not success :
	print 'Access to /gridgroup/ilc/garillot/uploadOnGrid.py has failed'
	sys.exit(1) 

#os.system('/gridgroup/ilc/garillot/uploadOnGrid.py ' + output + ' ' + outputDir)


os.system('rm ' + output)
for file in fileList :
	os.system('rm ' + file)








