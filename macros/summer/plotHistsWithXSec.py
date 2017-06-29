#!/usr/bin/env python

from optparse import OptionParser

p = OptionParser(usage='usage: <path:ROOT file directory>')

p.add_option('-o', '--outdir',     type='string', default='plots',       dest='outdir')
p.add_option('--hkey',             type='string', default=None,          dest='hkey')
p.add_option('--rkey',             type='string', default=None,          dest='rkey')
p.add_option('--dir-path',         type='string', default='',            dest='dir_path')

p.add_option('--ilumi',        type='float',  default=1000.00,       dest='ilumi')
p.add_option('--rebin',        type='int',    default=None,          dest='rebin')

p.add_option('-b', '--batch',   action='store_true', default=False,   dest='batch')
p.add_option('-d', '--debug',   action='store_true', default=False,   dest='debug')
p.add_option('-w', '--wait',    action='store_true', default=False,   dest='wait')
p.add_option('-s', '--save',    action='store_true', default=False,   dest='save')
p.add_option('--logy',          action='store_true', default=False,   dest='logy')

(options, args) = p.parse_args()  

import os
import re
import sys
import logging
import operator
import time

import ROOT
import PhysicsTaus.PhysicsTausConfig as tausConfig

if options.batch or not options.wait:
    ROOT.gROOT.SetBatch(True)

#======================================================================================================        
def getLog(name, level='INFO', debug=False, print_time=False):

    if print_time:
        f = logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
    else:
        f = logging.Formatter('%(name)s: %(levelname)s - %(message)s')
        
    sh = logging.StreamHandler(sys.stdout)
    sh.setFormatter(f)
    
    fh = logging.FileHandler('job.log')
    fh.setFormatter(f)

    log = logging.getLogger(name)
    log.addHandler(sh)
    log.addHandler(fh)
    
    if debug:
        log.setLevel(logging.DEBUG)
    else:
        if level == 'DEBUG':   log.setLevel(logging.DEBUG)
        if level == 'INFO':    log.setLevel(logging.INFO)
        if level == 'WARNING': log.setLevel(logging.WARNING)    
        if level == 'ERROR':   log.setLevel(logging.ERROR)

    return log

log = getLog(os.path.basename(__name__), print_time=False)

canvases = {}

#================================================================================
class XSecProc:

    ''' Class to hold cross-section data for one MC process dataset id (dsid)'''

    def __init__(self, line):

        parts = line.rstrip('\n').split()

        if len(parts) < 9:
            raise Exception('XSecProc - failed to parse line: %s' %line)

        self.dsid    = int  (parts[0])
        self.xsec    = float(parts[1])
        self.kfactor = float(parts[2])
        self.feff    = float(parts[3])
        self.process = parts[8]

        self.sum_weight_total = 0.0
        self.sum_nevent_total = 0

    def PrintAsStr(self):
        text  = '%-40s '           %(        self.process)
        text += '%6s '             %(        self.dsid)
        text += 'xsec=%13s '      %('%f'   %self.xsec)
        text += 'k-factor=%8s '   %('%f'   %self.kfactor)
        text += 'filter eff=%5s ' %('%f'   %self.feff)
        text += 'N event=%7s '    %('%.0f' %self.sum_nevent_total)
        text += 'weight all=%9s'  %('%f'   %self.sum_weight_total)

        return text

    def GetNormFactor(self):        
        return options.ilumi*self.xsec*self.kfactor/(self.feff*self.sum_weight_total)

#================================================================================
def filterList(vals, key, val):

    if type(val) == type(''):
        return val.split(',')

    if key == None:
        return vals

    out = []

    for v in vals:
        if re.search(key, getattr(v, 'name', v)) != None:
            out += [v]

    return out

#================================================================================
def getOutName(name):

    if name == None:
        return None

    if options.outdir != None:
        if not os.path.isdir(options.outdir):
            os.makedirs(options.outdir)

        name = '%s/%s' %(options.outdir.rstrip('/'), name)
        
    return name

#================================================================================
def getHistMinMax(hist, use_error=False):
    
    ymin = None    
    ymax = None

    if hist.Class().GetName().count('THStack'):
        return (hist.GetMinimum(), hist.GetMaximum())
    
    for ibin in range(1, hist.GetNbinsX()+1):
        val = hist.GetBinContent(ibin)
        err = hist.GetBinError  (ibin)

        if use_error:
            vmin = val - err
            vmax = val + err
        else:
            vmin = val
            vmax = val
        
        if vmax > 0.0:
            
            if ymax == None:
                ymax = vmax
            else:
                ymax = max([ymax, vmax])

        if vmin:
            
            if ymin == None:
                ymin = vmin
            else:
                ymin = min([ymin, vmin])

    return (ymin, ymax)

#================================================================================
def getYaxisMinMax(hists, use_error):
    
    ymin = None
    ymax = None    

    for h in hists:
        hmin, hmax = getHistMinMax(h, use_error)
        
        if ymax == None:
            ymax = hmax
        else:
            ymax = max([hmax, ymax])

        if ymin == None:
            ymin = hmin
        else:
            ymin = min([hmin, ymin])
    
    return (ymin, ymax)

#================================================================================
def updateCanvas(can, name=None):
    
    if not can:
        sys.exit(0)

    can.Modified()        
    can.Update()

    if options.wait:
        log.info('updateCanvas - double click on %s to continue...' %can.GetName())
        can.WaitPrimitive()

    if not can:
        sys.exit(0)
        
    if options.save and name != None:
        name = getOutName(name)
        can.Print('%s.pdf' %name, 'pdf')        

#================================================================================
def makeCanvas(name):
    
    global canvases

    if name in canvases:
        can = canvases[name]
        can.cd()
        return can

    ROOT.gStyle.SetPadRightMargin (0.15)         
    ROOT.gStyle.SetPadBottomMargin(0.18)
    ROOT.gStyle.SetPadLeftMargin  (0.15)
    
    ROOT.TGaxis.SetExponentOffset(0.035, -0.055, 'x')
    
    can = ROOT.TCanvas(name, name, 700, 500)
    
    if options.logy:
        can.SetLogy(True)
            
    canvases[name] = can
    return can

#================================================================================
def normHists(hists, can=None, scale=1.05):
    
    ymax = None
    ymin = None    

    for h in hists:
        hmin, hmax = getHistMinMax(h, True)

        if ymax == None:
            ymax = hmax
        else:
            ymax = max([ymax, hmax])

        if ymin == None:
            ymin = h.GetMinimum()
        else:
            ymin = min([ymin, h.GetMinimum()])

    if can and can.GetLogy():
        scale = scale*3.0

        if not ymin > 0.0:
            ymin = 0.01

    for h in hists:
        if ymax:
            h.SetMaximum(scale*ymax)
        if ymin:            
            h.SetMinimum(ymin)
            
#================================================================================
def getLegend(x1, y1, x2, y2):

    leg = ROOT.TLegend(x1, y1, x2, y2)
    leg.SetBorderSize(0)
    leg.SetFillStyle (0)
    leg.SetNColumns  (1)

    return leg

#================================================================================
def getColorDSID(dsid):

    colors = {361510: 2,
              361511: 3,
              361512: 4,
              361513: 6,
              361514: 7,
              }

    if dsid in colors:
        return colors[dsid]

    return 1

#================================================================================
def isNumericStr(val):

    nums = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    
    for s in val:
        if s not in nums:
            return False

    return True

#================================================================================
def readNorms(rfile, xsecs, dirname='Counts', histname='Count'):

    ''' Read sum of weights over all MC events before any selections '''
    
    for x in xsecs:
        cname = '%s/%s/%s' %(dirname, x.dsid, histname)
        chist = rfile.Get(cname)
        
        if not chist:
            raise Exception('readNorms - failed to find histogram: %s' %cname)

        x.sum_weight_total = chist.GetBinContent(1)
        x.sum_nevent_total = chist.GetBinContent(3)

    log.info('readNorms - process %d active MC channels' %len(xsecs))

    for x in sorted(xsecs, key = operator.attrgetter('process')):
        print x.PrintAsStr()

#================================================================================
def readXSecs(dsids=None):
    
    ''' Read cross-section info from text cross-section file '''

    testarea = os.getenv('TestArea')
    xsecfile = '%s/PhysicsNtuple/PhysicsTaus/data/Xsection13TeV_tth_bkg_v1.txt' %(testarea)

    if not os.path.isfile(xsecfile):
        raise Exception('readXSecs - invalid xsec file: %s' %xsecfile)

    xfile = open(xsecfile, 'r')
    xsecs = []

    for line in xfile.readlines():
        if line.count('#') == 0 and len(line.split()) > 8:
            xs = XSecProc(line)
            
            if dsids == None or xs.dsid in dsids:
                xsecs += [xs]

    return xsecs

#================================================================================
def readDSIDs(rfile):

    ''' Determine which MC channels IDs are save in input ROOT file '''

    dlist = []

    for dkey in rfile.GetListOfKeys():
        if len(dkey.GetName()) == 6 and isNumericStr(dkey.GetName()):
            dlist += [int(dkey.GetName())]

    return dlist

#================================================================================
def listHists(rfile, xsecs, dirpath):

    ''' Find all unique histograms at path: dsid/dirpath'''

    hists = []

    for x in xsecs:
        rdir = rfile.Get('%s/%s' %(x.dsid, dirpath))
        
        if not rdir:
            continue

        for dkey in rdir.GetListOfKeys():
            if dkey.GetClassName().count('TH1') == 0:
                continue
            
            hname = dkey.GetName()

            #
            # If set, apply regular expression filter - otherwise select all histograms
            #
            if options.hkey == None or re.search(options.hkey, hname):
                hists += [hname]
  
    return list(set(hists))

#================================================================================
def mergeHists(rfile, xsecs, histpath):
    
    hout = None

    for x in xsecs:
        h = rfile.Get('%s/%s' %(x.dsid, histpath))
        
        if not h:
            continue

        h.SetDirectory(0)
        h.Scale(x.GetNormFactor())

        if hout == None:
            hout = h
        else:
            hout.Add(h)

    if hout:
        hout.SetStats(False)

    return hout

#================================================================================
def makeStack(rfile, xsecs, histpath):
    
    sname = histpath.replace('/', '_')
    stack = ROOT.THStack(sname, sname)
    hists = []

    legen = getLegend(0.70, 0.65, 0.85, 0.85)
    
    for x in sorted(xsecs, key = operator.attrgetter('dsid'), reverse=True):
        h = rfile.Get('%s/%s' %(x.dsid, histpath))
        
        if not h:
            continue

        icolor = getColorDSID(x.dsid)

        h.SetDirectory(0)
        h.Scale(x.GetNormFactor())
        h.SetLineColor(icolor)
        h.SetFillColor(icolor)

        legen.AddEntry(h, '%s' %x.dsid, 'f')
        stack.Add(h)

        hists += [h]

    return (stack, legen, hists)

#================================================================================
def plotHists(rfile, xsecs, dirpath):

    ''' Plot all unique histograms at path: dsid/dirpath'''

    can = makeCanvas('can')

    #
    # List TH1 histograms at requested path
    #
    hists = listHists(rfile, xsecs, dirpath)

    for hist in hists:
        histpath = '%s/%s' %(dirpath, hist)
        histname = histpath.replace('/', '_')

        hplot = mergeHists(rfile, xsecs, histpath)
        hplot.Draw()
        
        normHists([hplot], can, 1.30)

        updateCanvas(can, name=histname)

        #
        # Make and plot stack histogram
        #
        (stack, legen, hists) = makeStack(rfile, xsecs, histpath)
        stack.Draw('HIST')
        legen.Draw()

        normHists([stack], can, 1.30)

        stack.GetHistogram().GetXaxis().SetTitle(hplot.GetXaxis().GetTitle())
        stack.GetHistogram().GetYaxis().SetTitle(hplot.GetYaxis().GetTitle())

        updateCanvas(can, name='%s_stack' %histname)

#================================================================================
def main():

    #
    # Check inputs
    #
    if len(args) != 1: 
        log.info('Need exactly one argument: %s' %(args))
        sys.exit(1)

    if len(args) == 1 and not os.path.isfile(args[0]):
        log.error('Invalid input file: %s' %args[0])
        sys.exit(1)

    tausConfig.setPlotDefaults(ROOT)

    rfile = ROOT.TFile(args[0], 'READ')

    #
    # Check which dataset ids (MC channel ids) are active for the input file
    #
    dsids = readDSIDs(rfile)

    #
    # Read x-section data
    #
    xsecs = readXSecs(dsids)

    #
    # Read sum of MC event weights computed before selections, using all events
    #
    readNorms(rfile, xsecs)

    #
    # Plot TH1 histograms at requested path
    #
    plotHists(rfile, xsecs, options.dir_path)
    
#================================================================================
if __name__ == '__main__':
    main()

    
